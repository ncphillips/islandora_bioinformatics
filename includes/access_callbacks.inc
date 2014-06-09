<?php

function islandora_related_lab_objects_access($permissions, AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  if (!islandora_describe_repository()) {
    islandora_display_repository_inaccessible_message();
    return FALSE;
  }

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
  $cmodel_id = $object->models[0];

  $relationships = islandora_cmodel_relationships($cmodel_id);

  foreach($relationships as $rel){
    if($rel['object']['value'] == 'islandora:person_cmodel'){
      return true;
    }
  }
  return false;
}

function islandora_object_has_lab_objects($object){
  $cmodel_id = $object->models[0];

  $relationships = islandora_cmodel_relationships($cmodel_id);
//  echo '<pre>';
//  var_dump($relationships);
//  echo '</pre>';


  foreach($relationships as $rel){
    $id = $rel['object']['value'];
    $types = array_merge(array($id), islandora_cmodel_supertypes($id));

    if(in_array('islandora:lab_object_cmodel', $types)) {
      return true;
    }
  }
  return false;
}