<?php

/**
 * @file cmodel_ingest_steps.inc
 *
 *       This file provides this modules implementations of hook_islandora_ingest_steps()
 */

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
    'file' => 'includes/image_upload.form.inc',
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
    'file' => 'includes/image_upload.form.inc',
  );

  /** Assign people to this Project. */
  $ingest_steps['assign_people'] = array(
    'weight' => 15,
    'type' => 'form',
    'form_id' => 'project_assign_person_form',
    'module' => 'islandora_bioinformatics_base',
    'file' => 'includes/project_assign_person.form.inc',
  );

  return $ingest_steps;
}