<?php

function islandora_related_lab_objects_access($permissions, AbstractObject $object) {
  /**
   * @author Quintin Dawson
   * @param $permissions
   * @param AbstractObject $object
   * @return bool
   * Checks if the user has permission to view lab objects and that the lab objects exist for $object.
   */
  //check if repository is available
  module_load_include('inc', 'islandora', 'includes/utilities');
  if (!islandora_describe_repository()) {
    islandora_display_repository_inaccessible_message();
    return FALSE;
  }

  //if there are no lab objects for the $object, there is nothing to see
  if (!islandora_object_has_lab_objects($object)) {
    return FALSE;
  }

  $has_access = FALSE;
  for ($i = 0; $i < count($permissions) && !$has_access; $i++) {
    $has_access = $has_access || islandora_object_access($permissions[$i], $object);
  }

  return $has_access;
}

function islandora_related_people_access($permissions, AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  if (!islandora_describe_repository()) {
    islandora_display_repository_inaccessible_message();
    return FALSE;
  }


  if (!islandora_object_has_people($object)) {
    return FALSE;
  }

  for ($i = 0; $i < count($permissions); $i++) {
    if (!islandora_object_access($permissions[$i], $object)) {
      return FALSE;
    }
  }
  return TRUE;
}

function islandora_object_has_people($object) {
  /**
   * @author Nolan Phillips
   * @param $object
   *
   * This function returns true if the $object passed in might
   * have Lab Objects related to it.
   */
  $cmodel_id = $object->models[0];



  $query = "
    PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
    SELECT ?datarel
    FROM <#ri>
    WHERE
    {
      {<info:fedora/islandora:person_cmodel> ?datarel <info:fedora/$cmodel_id>} .
    }
  ";

  $tuque = new IslandoraTuque();
  $result = $tuque->repository->ri->sparqlQuery($query, 'unlimited');

  if (sizeof($result) >0)
    return TRUE;
  else return false;
}