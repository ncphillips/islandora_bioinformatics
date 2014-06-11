<?php

/**
 * islandora_related_objects_page
 *
 * @param $object
 */
function islandora_related_objects_page($object, $cmodel_id=null){
  $page_state['object'] = $object;
  $page_state['relationships'] = islandora_cmodel_relationships($object->models[0]);

  // Replace : with _ in CModel ID.
  $cmid = str_replace(':', '_', $object->models[0]);

  $page_types = array(
    'islandora_related_objects_page',         // Alter any related objects page.
    "islandora_{$cmid}_related_objects_page");// Alter the related objects page for $cmid.

  drupal_alter($page_types, $page, $page_state);

  // With the page(state) altered, we generate the forms.
  foreach($page_state['relationships'] as $relationship){
    // The $relationship array is in the wrong format.
    $cm = $relationship['object']['value'];

    if($cmodel_id == null or $cmodel_id == $relationship['object']['value']) {
      $rels = array(
        $relationship['predicate']['namespace'] => array(
          $relationship['predicate']['value'],
        ),
      );

      $page['content forms'][]  = drupal_get_form(
        'islandora_related_objects_form',  // Form
        $object->id,                       // Object
        $relationship['object']['value'],  // CModel
        $rels);                            // Relationships
    }
  }

  return $page;
}


function islandora_related_objects_ingest_callback($parent, $cmodel){
  echo $cmodel->id;

  // Rplace : with _ in CModel ID.
  $cmid = str_replace(':', '_', $cmodel->id);

  // Change the parent collection.
  $data = array(
    'collection' => 'islandora:specimen_collection',
  );

  $hooks = array(
    "islandora_related_objects_ingest",
    "islandora_related_{$cmid}_objects_ingest",
  );

  drupal_alter($hooks, $data);

  $collection = islandora_object_load($data['collection']);

  if($collection !== FALSE) {
    // Make sure $collection is a collection, and has a COLLECTION_POLICY.
    $is_collection_object = in_array('islandora:collectionCModel', $collection->models);
    $has_policy = isset($collection['COLLECTION_POLICY']);

    if (!$is_collection_object || !$has_policy) {
      return drupal_not_found();
    }

    $object = $collection->repository->constructObject();
    $object->label = 'New Object';

    $results = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection->id);
    if (empty($results)) {
      $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection->id);
    }

    $relationships = array(
      FEDORA_RELS_EXT_URI => array('isPartOf')
    );

    foreach($relationships as $ns => $rels){
      foreach($rels as $rel){
        $results = $object->relationships->get($ns, $rel, $parent);
        if (empty($results)) {
          $object->relationships->add($ns, $rel, $parent);
        }
      }
    }

    $config_data = array(
      'collection' => $collection,
      'models' => array($cmodel->id),
      'objects' => array($object),
      'parent' => $collection->id, );


    module_load_include('inc', 'islandora', 'includes/ingest.form');
    return drupal_get_form('islandora_ingest_form', $config_data);
  }
  drupal_not_found();
}

function islandora_related_objects_add_callback($object, $cmodel) {
  return l('Back to object', $_SERVER['HTTP_REFERER']);
}



