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

//  /** Upload a thumbnail image for this Person. */
//  $ingest_steps['image_upload'] = array(
//    'weight' => 10,
//    'type' => 'form',
//    'form_id' => 'image_upload_form',
//    'module' => 'islandora_bioinformatics',
//    'file' => 'forms/image_upload.form.inc',
//  );

  return $ingest_steps;
}

/**
 * Implements hook_islandora_ingest_steps() for the project_cmodel.
 */
function islandora_bioinformatics_islandora_project_cmodel_islandora_ingest_steps(array $form_state) {
  $ingest_steps = array();

//  /** Upload a thumbnail image for this Project.  */
//  $ingest_steps['image_upload'] = array(
//    'weight' => 10,
//    'type' => 'form',
//    'form_id' => 'image_upload_form',
//    'module' => 'islandora_bioinformatics',
//    'file' => 'forms/image_upload.form.inc',
//  );
//
//  /** Assign people to this Project. */
//  $ingest_steps['assign_people'] = array(
//    'weight' => 15,
//    'type' => 'form',
//    'form_id' => 'project_assign_person_form',
//    'module' => 'islandora_bioinformatics',
//    'file' => 'forms/project_assign_person.form.inc',
//  );

  return $ingest_steps;
}


function islandora_bioinformatics_islandora_specimen_cmodel_islandora_ingest_steps(array $configuration) {
  return array(
    // I think this was to upload images, but we're not going to store images as DS anymore.
//    'islandora_lab_object_specimen_file_upload' => array(
//      'weight' => 10,
//      'type' => 'form',
//      'form_id' => 'islandora_lab_object_specimen_upload_form',
//      'module' => 'islandora_lab_object_specimen',
//      'file' => 'includes/islandora_lab_object_specimen_upload.form.inc'
//    ),

  // This should be done through the Related Content interface, not the ingest steps.
//    'islandora_lab_object_specimen_project_relate' => array(
//      'weight' => 15,
//      'type' => 'form',
//      'form_id' => 'islandora_lab_object_specimen_project_form',
//      'module' => 'islandora_lab_object_specimen',
//      'file' => 'includes/islandora_lab_object_specimen_project.form.inc'
//    )
  );
}

function islandora_bioinformatics_islandora_genesequence_cmodel_islandora_ingest_steps(array $configuration) {
  return array(
    'islandora_bioinformatics_genesequence_fasta_file_upload' => array(
      'weight' => 8,
      'type' => 'form',
      'form_id' => 'upload_genesequence_fasta_form',
      'module' => 'islandora_bioinformatics',
      'file' => 'forms/upload_fasta.form.inc'
    ),
  );
}


function islandora_bioinformatics_islandora_lcms_cmodel_islandora_ingest_steps(array $configuration) {
  return array(
    'islandora_bioinformatics_lcms_csv_file_upload' => array(
      'weight' => 8,
      'type' => 'form',
      'form_id' => 'islandora_bioinformatics_lcms_upload_csv_form',
      'module' => 'islandora_bioinformatics_lcms',
      'file' => 'includes/islandora_bioinformatics_lcms_upload_csv.form.inc'
    ),
    'islandora_bioinformatics_lcms_raw_file_upload' => array(
      'weight' => 9,
      'type' => 'form',
      'form_id' => 'islandora_bioinformatics_lcms_upload_raw_form',
      'module' => 'islandora_bioinformatics_lcms',
      'file' => 'includes/islandora_bioinformatics_lcms_upload_raw.form.inc'
    )
  );
}


