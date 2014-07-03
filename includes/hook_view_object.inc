<?php

/**
 * Implements hook_CMODEL_PID_islandora_view_object() for the Person CModel.
 */
function  islandora_bioinformatics_islandora_person_cmodel_islandora_view_object($object) {
  $output = theme('islandora_bioinformatics_person', array('islandora_object' => $object));
  return array('islandora_bioinformatics_person' => $output);
}

/**
 * Implements hook_CMODEL_PID_islandora_view_object() for the Project CModel.
 */
function islandora_bioinformatics_islandora_project_cmodel_islandora_view_object($object) {
  $output = theme('islandora_bioinformatics_project', array('islandora_object' => $object));
  return array('islandora_project' => $output);
}

