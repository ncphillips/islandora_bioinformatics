<?php

/**
 * islandora_related_objects_page
 *
 * @param $object
 */
function islandora_related_objects_page($object, $related_cmodel_id=null){
  $page_state['object'] = $object;
  $page_state['relationships'] = islandora_cmodel_relationships($object->models[0]);

  // Replace : with _ in CModel ID.
  $cmid = str_replace(':', '_', islandora_object_parent_model($object->id)[0]);

  $page_types = array(
    'islandora_related_objects_page',         // Alter any related objects page.
    "islandora_{$cmid}_related_objects_page");// Alter the related objects page for $cmid.

  drupal_alter($page_types, $page, $page_state);

  $related_content_form_args = array();
  foreach($page_state['relationships'] as $relationship){
    $c = $relationship['object']['value'];

    if($related_cmodel_id == null or $related_cmodel_id == $c){
      $ns = $relationship['predicate']['namespace'];
      $pred = $relationship['predicate']['value'];

      $related_content_form_args[$c][$ns][] = $pred;
    }
  }

  foreach($related_content_form_args as $c => $rels){
    $page['content forms'][] = drupal_get_form(
      'islandora_related_objects_form',
      $object->id,
      $c,
      $rels
    );
  }

  return $page;
}

function islandora_related_objects_ingest_callback(FedoraObject $parent, $cmodel){
  islandora_cmodel_create('islandora:sponge_cmodel', 'Sponge', '', 'islandora:specimen_cmodel');


  // Replace : with _ in CModel ID.
  $cmid = str_replace(':', '_', $cmodel->id);


  // Implement these hooks if you want to set the collection object.
  $hooks = array(
    "islandora_related_objects_ingest",
    "islandora_related_{$cmid}_objects_ingest",
  );
  drupal_alter($hooks, $data);
  $collection = islandora_object_load($data['collection']);

  // Config data for Ingestion.
  $config_data = array(
    /*
     * id
     * namespace
     * label
     * collections
     * models
     * parent
     */
  );

  if($collection){
    // Make sure $collection is a collection, and has a COLLECTION_POLICY.
    $is_collection_object = in_array('islandora:collectionCModel', $collection->models);
    $has_policy = isset($collection['COLLECTION_POLICY']);

    if (!$is_collection_object || !$has_policy) {
      return drupal_not_found();
    }
    $object = $collection->repository->constructObject();
    $config_data['collection'] = $collection;
    $config_data['parent'] = $collection->id;
  }
  else {
    $object = $parent->repository->constructObject();
//    $config_data['parent'] = $parent;
  }

  $object->label = 'New Object';

  $models = array_merge(islandora_cmodel_subtypes($cmodel->id), array($cmodel->id, 'islandora:person_cmodel'));
  var_dump($models);
  $config_data['models'] = $models;
  $config_data['object'] = $object;

  // This part should be a separate form...
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


  module_load_include('inc', 'islandora', 'includes/ingest.form');
  return drupal_get_form('islandora_ingest_form', $config_data);
}

function islandora_related_objects_add_callback($object, $cmodel) {
  return l('Back to object', $_SERVER['HTTP_REFERER']);
}



