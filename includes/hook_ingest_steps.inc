<?php

/**
 * @file cmodel_ingest_steps.inc
 *
 *       This file provides this modules implementations of hook_islandora_ingest_steps()
 */

function islandora_bioinformatics_islandora_ingest_steps(){
  $ingest_steps = array();

  $ingest_steps['set_parent_relationship'] = array(
    'type' => 'callback',
    'weight' => 2,
    'do_function' => array(
      'function' => 'set_parent_relationships',
      'args' => array('object'),
    ),
    'undo_function' => array(
      'function' => 'unset_parent_relationships',
      'args' => array('object'),
    ),
  );

  return $ingest_steps;
}

function set_parent_relationships($args){

  $object = $args['build_info']['args'][0]['object'];
//  echo '<pre>';
//  var_dump($args);
//  var_dump(array_keys($args));
//  echo '</pre>';
  // The object which will be designated the parent.
  $has_parent = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'temp');

  if($has_parent){
    // Remove the temp relationship
    $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'temp');

    // Get teh ID of the Parent object
    $parent_id = $has_parent[0]['object']['value'];

    // Get all of the Parent object's CModels.
    $parent_cmodels = islandora_object_models($parent_id);

    // Get the New Object's CModel.
    $cmodel_id = $args['input']['models'];
    echo $cmodel_id;

    // Get the NO CModel's CMRELS
    $cmodel_rels = islandora_cmodel_relationships_away($cmodel_id);

    // For each of those CMRELS
    foreach($cmodel_rels as $rel){
      // If {the object of the relationship} INTERSECT {PARENT CMODELS} is a
      // non empty set, then create the relationship.
      if (array_intersect($parent_cmodels, array($rel['object']['value']))){
        $ns = $rel['predicate']['namespace'];
        $predicate = $rel['predicate']['value'];
        $object->relationships->add($ns, $predicate, $parent_id);
      }
    }
  }
}

function unset_parent_relationship($object){

}


/**
 * Implements hook_islandora_ingest_steps() for the person_cmodel.
 */
function islandora_bioinformatics_islandora_person_cmodel_islandora_ingest_steps(array $form_state) {

  $ingest_steps = array();

  /** Upload a thumbnail image for this Person. */
  $ingest_steps['image_upload'] = array(
    'weight' => 10,
    'type' => 'form',
    'form_id' => 'image_upload_form',
    'module' => 'islandora_bioinformatics',
    'file' => 'forms/image_upload.form.inc',
  );

  return $ingest_steps;
}

/**
 * Implements hook_islandora_ingest_steps() for the project_cmodel.
 */
function islandora_bioinformatics_islandora_project_cmodel_islandora_ingest_steps(array $form_state) {
  $ingest_steps = array();

  /** Upload a thumbnail image for this Project.  */
  $ingest_steps['image_upload'] = array(
    'weight' => 10,
    'type' => 'form',
    'form_id' => 'image_upload_form',
    'module' => 'islandora_bioinformatics',
    'file' => 'forms/image_upload.form.inc',
  );

  /** Assign people to this Project. */
  $ingest_steps['assign_people'] = array(
    'weight' => 15,
    'type' => 'form',
    'form_id' => 'project_assign_person_form',
    'module' => 'islandora_bioinformatics',
    'file' => 'forms/project_assign_person.form.inc',
  );

  return $ingest_steps;
}