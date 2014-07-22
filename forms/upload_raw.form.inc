<?php

  /*
    @file
    handles uploading a raw file to an lcms as part of ingest
  */

function upload_lcms_raw_form(array $form, array &$form_state) {

  //sets the max upload size
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('raw');
  $form = array();

  $form['raw_file'] = array(
    '#title' => t('LCMS RAW File'),
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#description' => t('Upload the raw file.'),
    '#default_value' => isset($form_state['values']['raw_file']) ? $form_state['values']['raw_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      'file_validate_size' => array($upload_size * 1048576),
    ),
  );
  return $form;
}

function upload_lcms_raw_form_submit(array $form, array &$form_state) {
  $object = $form_state['islandora']['objects'][0];
  if (empty($object['RAW'])) {
    $ds = $object->constructDatastream('RAW', 'M');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['RAW'];
  }
  $raw_file = file_load($form_state['values']['raw_file']);
  $raw_path = drupal_realpath($raw_file->uri);
  $ds->setContentFromFile($raw_path, FALSE);
  $ds->label = $raw_file->filename;
  $ds->mimetype = $raw_file->filemime;
}

