<?php

function islandora_related_people_access($permissions, AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  if (!islandora_describe_repository()) {
    islandora_display_repository_inaccessible_message();
    return FALSE;
  }

  /**
  if (!islandora_object_has_people($object)) {
    return FALSE;
  }
   */

  for ($i = 0; $i < count($permissions); $i++) {
    if (!islandora_object_access($permissions[$i], $object)) {
      return FALSE;
    }
  }
  return TRUE;
}
