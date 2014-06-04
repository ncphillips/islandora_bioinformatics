<?php

/**
 * @file cmodel_functions.php
 *
 * Provides a number of helper functions for when working with CModel objects.
 */


define('CMODEL_SUFFIX', '_cmodel');
define('REL_TO', 'to');
define('REL_FROM', 'from');
define('REL', 'any');

/**
 * Loads an Islandora Object if it is a CModel.
 *
 * @param String $cmodel_id
 * This is a partial ID of an Islandora Content Model.
 *
 * @return FedoraObject $object | null
 * If a FedoraObject is found, and it is a Content Model, return it;
 * else, return NULL.
 *
 */
function islandora_cmodel_load($cmodel_id) {
  /**
   * Load the object from the pid and then check to make sure it is
   * actually a CModel.
   */
  $cmodel = islandora_object_load($cmodel_id);
  if ($cmodel && !in_array('fedora-system:ContentModel-3.0', $cmodel->models)) {
    /** @todo Produce an error message explaining why this function failed. */
    return NULL;
  }
  return $cmodel;
}

// Relationships
/**
 * Add a relationship to the given CModel.
 *
 * Takes a subject (either a AbstractObject or a FedoraDatastream), as well as
 * the parameters for FedoraRelsExt::add() or FedoraRelsInt::add(), and adds
 * the represented relationship.
 *
 * @see FedoraRelsExt::add()
 */
function islandora_cmodel_add_relationship($sub, $pred_uri, $pred, $object, $type) {
  module_load_include('inc', 'islandora', 'islandora.rules');
  $sub_is_cmodel    = islandora_cmodel_load($sub);
  $object_is_cmodel = islandora_cmodel_load($object);
  $rel_exists = islandora_object_has_relationship($sub, $pred_uri, $pred, $object, $type);

  // If they're both CModels, and the relationship has not already been  defined.
  if ($sub_is_cmodel and $object_is_cmodel and !$rel_exists) {
    // Define the relationship.
    islandora_object_add_relationship($sub, $pred_uri, $pred, $object, $type);
    return TRUE;
  }
  /** @todo Produce error messages depending on why this function failed. */
  return FALSE;
}

/**
 * Removes a relationship to the given CModel.
 *
 * Takes a subject (either a AbstractObject or a FedoraDatastream), as well as
 * the parameters for FedoraRelsExt::remove() or FedoraRelsInt::remove(),
 * and removes the represented relationship.
 */
function islandora_cmodel_remove_relationship($sub, $pred_uri, $pred, $object, $type) {
  $sub_is_cmodel    = islandora_cmodel_load($sub);
  $object_is_cmodel = islandora_cmodel_load($object);
  $rel_exists = islandora_object_has_relationship($sub, $pred_uri, $pred, $object, $type);

  // If they're both CModels and the relationship is defined between them.
  if ($sub_is_cmodel and $object_is_cmodel and $rel_exists) {
    // Remove the relationship.
    islandora_object_remove_relationship($sub, $pred_uri, $pred, $object, $type);
    return TRUE;
  }
  /** @todo Produce error messages depending on why this function failed. */
  return FALSE;
}


/**
 * islandora_cmodel_relationships
 *
 * @param $cmodel_id
 *
 * @return array
 * The results of a SPARQL query.
 *
 * This function finds all objects related to the CModel or any of it's
 * supertypes. It excludes the _model_ and _subtype_ relationships.
 */
function islandora_cmodel_relationships($cmodel_id) {
  $cmodel_ids = array($cmodel_id);
  $cmodel_ids = array_merge($cmodel_ids, islandora_cmodel_supertypes($cmodel_id));

  $cm_query_array = array();
  foreach($cmodel_ids as $id){
    $cm_query_array[] = '{' . "?id ?relationship <info:fedora/$id>" . '}';
  }
  $cm_query = implode(' UNION ', $cm_query_array);

  $query = "
    PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
    SELECT *
    FROM <#ri>
    WHERE
    {
      {?id fedora-model:hasModel <info:fedora/fedora-system:ContentModel-3.0>}
      {
        $cm_query
      }
      FILTER(!REGEX(STR(?relationship), 'Subtype'))
      FILTER(!REGEX(STR(?relationship), 'Model'))
    }";

  file_put_contents('/Users/nolan/Desktop/query.txt', $query);

  $tuque = new IslandoraTuque();
  $results = $tuque->repository->ri->sparqlQuery($query);

  $relatives = array();
  foreach($results as $result){
    $relatives[$result['id']['value']][] = $result['relationship']['value'];
  }

  echo "<pre>";
  var_dump($relatives);
  echo "</pre>";

  return $relatives;
}


// Polymorphism
/**
 * Islandora CModel Parent
 *
 * @param $cmodel_id
 *
 * @return array
 * Contains the PIDs that are parents of $cmodel_id. This array should always
 * have only one PID.
 */
function islandora_cmodel_parent($cmodel_id) {
  $cmodel = islandora_cmodel_load($cmodel_id);
  $parents = array(); // This should be an array of length 1.

  if($cmodel){
    $rels = $cmodel->relationships->get(ISLANDORA_RELS_EXT_URI,  'hasParentModel');

    foreach($rels as $rel)
      $parents[] = $rel['object']['value'];
  }
  return $parents;
}

/**
 * Islandora CModel Supertypes
 * @param $cmodel_id
 *
 * @return array
 * Contains the PIDs of all CModels that $cmodel_id subtypes from.
 */
function islandora_cmodel_supertypes($cmodel_id) {
  $cmodel = islandora_cmodel_load($cmodel_id);
  $supertypes = array();

  if ($cmodel) {
    $rels = $cmodel->relationships->get(ISLANDORA_RELS_EXT_URI, 'isSubtypeOf');

    $supertypes = array();
    foreach($rels as $rel){
      $supertypes[] = $rel['object']['value'];
    }
  }
  return $supertypes;
}

/**
 * Creates and returns a Fedora CModel.
 *
 * @param      $cmodel_id
 * @param      $owner
 * @param      $label
 * @param null $parent_id
 *
 * @return AbstractObject|null
 *
 * @todo Write docs for `islandora_cmodel_create`
 */
function islandora_cmodel_create($cmodel_id, $label, $dscm_path = '', $parent_id = NULL, $owner = 'fedoraAdmin') {

  $tuque = new IslandoraTuque();

  // If a parent ID is given, but the Fedora Object doesn't exist, or
  // is not a CModel, return NULL.
  if ($parent_id && is_null(islandora_cmodel_load($parent_id))) {
    /** @todo Throw an error message. */
    return NULL;
  }

  // If the ID isn't being used by some Fedora Object.
  // Create Content Model
  $cmodel = $tuque->repository->constructObject($cmodel_id);
  $cmodel->owner = $owner;
  $cmodel->label = $label;
  $cmodel->models = 'fedora-system:ContentModel-3.0';

  if ($dscm_path){
    // DS-COMPOSITE-MODEL Datastream.
    $dscm = $cmodel->constructDatastream('DS-COMPOSITE-MODEL', 'X');
    $dscm->label = 'Datastreams';
    $dscm->mimetype = 'text/xml';
    $dscm->setContentFromFile($dscm_path, FALSE);
    $cmodel->ingestDatastream($dscm);
  }
  else {
    /** @todo Set up a skeleton dscm. */
  }

  // Inherit from parent
  if ($parent_id) {
    $cmodel->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasParentModel', $parent_id);

    // Supertypes
    $supertypes = array($parent_id);
    array_merge($supertypes, islandora_cmodel_supertypes($parent_id));


    foreach($supertypes as $supertype){
      $cmodel->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSubtypeOf', $supertype);
    }


  }

  return $cmodel;

}


/**
 * Adds a datastream entry to the DS-COMPOSITE-MODEL of this Fedora CModel.
 */
function islandora_cmodel_add_datastream($cmodel_id) {}

/**
 * Removes a datastream entry to the DS-COMPOSITE-MODEL of this Fedora CModel.
 */
function islandora_cmodel_remove_datastream($cmodel_id) {}

/**
 * Returns an array of datastream entries from this Fedora CModel.
 */
function islandora_cmodel_datastreams() {}




