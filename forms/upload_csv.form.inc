<?php

  /*
    @file
    handles uploading a csv file to an lcms as part of ingest
  */

function upload_lcms_csv_form(array $form, array &$form_state) {

  //sets the max upload size
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('csv');
  $form = array();

  $form['csv_file'] = array(
    '#title' => t('LCMS csv File'),
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#description' => t('Upload the sequence file.'),
    '#default_value' => isset($form_state['values']['csv_file']) ? $form_state['values']['csv_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      'file_validate_size' => array($upload_size * 1048576),
    ),
  );
  return $form;
}

function upload_lcms_csv_form_submit(array $form, array &$form_state) {
  $object = $form_state['islandora']['objects'][0];
  if (empty($object['CSV'])) {
    $ds = $object->constructDatastream('CSV', 'M');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['CSV'];
  }
  $csv_file = file_load($form_state['values']['csv_file']);
  $csv_path = drupal_realpath($csv_file->uri);
  $ds->setContentFromFile($csv_path, FALSE);
  $ds->label = $csv_file->filename;
  $ds->mimetype = $csv_file->filemime;
}
