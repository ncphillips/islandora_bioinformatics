<?php

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
  // Use this function instead of $object->models[0] to make sure we get
  // the actual parent CModel.
  $cmodel_id = islandora_object_parent_model($object->id);

  // NOTE: This function will check all of $object's CModel's supertypes as well.
  $relationships = islandora_cmodel_relationships_towards($cmodel_id);

  foreach($relationships as $rel){
    if($rel['object']['value'] == 'islandora:person_cmodel'){
      return true;
    }
  }
  return false;
}
