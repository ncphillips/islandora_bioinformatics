<?php

/**
 * islandora_related_objects_page
 *
 * RELATED CONTENT
 */
function islandora_related_objects_page($object, $related_cmid_restriction=null){
  $page = array();

  // Replace : with _ in CModel ID.
  $object_parent_model = islandora_object_parent_model($object->id);
  echo $object_parent_model;
  $objects_models = islandora_object_models($object->id);


  // Page State
  $page_state['object'] = $object;
  $page_state['relationships'] = islandora_cmodel_relationships_towards($object_parent_model);

  // Hooks to call.
  $hooks = array('islandora_related_objects_page');

  // Related Content Page Alters are inherited.
  // It would be nice to make sure order is preserved.
  foreach($objects_models as $m){
    $o = str_replace(':', "_", $m);
    $hooks[] = "islandora_{$o}_related_objects_page";// Alter the related objects page for $cmid.

  }

  drupal_alter($hooks, $page, $page_state);

  // One Related Content Form per CModel.
  $related_content_form_args = array();
  foreach($page_state['relationships'] as $relationship){
    $related_cmodel_id = $relationship['object']['value'];

    if($related_cmid_restriction == null or $related_cmid_restriction == $related_cmodel_id){
      $pred = $relationship['predicate']['value'];

      $related_content_form_args[$related_cmodel_id][] = $pred;
    }
  }

  // Create the Related Content Forms
  foreach($related_content_form_args as $related_cmodel_id => $rels){
    $page['content forms'][] = drupal_get_form(
      'islandora_related_objects_form', // Form ID
      $object->id,                      // Object ID
      $related_cmodel_id,               // Related CModel ID
      $rels                             // Array of valid relationships
    );
  }

  return $page;
}

function islandora_related_objects_ingest_callback($object, $cmodel){
  // Replace : with _ in CModel ID.
  $cmid = str_replace(':', '_', $cmodel->id);


  // Implement these hooks if you want to set the collection object.
  $hooks = array(
    "islandora_related_objects_ingest",
    "islandora_related_{$cmid}_objects_ingest",
  );

  drupal_alter($hooks, $data);


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

  $collection = islandora_object_load($data['collection']);

  if($collection){
    // Make sure $collection is a collection, and has a COLLECTION_POLICY.
    $is_collection_object = in_array('islandora:collectionCModel', $collection->models);
    $has_policy = isset($collection['COLLECTION_POLICY']);

    if (!$is_collection_object || !$has_policy) {
      return drupal_not_found();
    }
    $new_object = $collection->repository->constructObject();
    $config_data['collection'] = $collection;
    $config_data['parent'] = $collection->id;
  }
  else {
    $new_object = $object->repository->constructObject();
    $config_data['parent'] = $object;
  }

  $new_object->label = 'New Object';

  $models = array_merge(                    // Models to choose from are
    array($cmodel->id),                     // the CModel passed in and
    islandora_cmodel_subtypes($cmodel->id)  // all of it's Subtypes.
  );

  $config_data['models'] = $models;
  $config_data['object'] = $new_object;

  $new_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'temp', $object);

  module_load_include('inc', 'islandora', 'includes/ingest.form');
  return drupal_get_form('islandora_ingest_form', $config_data);
}

function islandora_related_objects_add_callback($object, $cmodel) {
  return l('Back to object', $_SERVER['HTTP_REFERER']);
}



