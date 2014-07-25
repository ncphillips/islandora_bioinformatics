<?php

/**
 * @file
 * Handles uploading a FASTA file to an genesequence as part of ingest.
 */
 
function upload_genesequence_fasta_form(array $form, array &$form_state) {

  $models = $form_state['islandora']['objects'][0]->models;
  //sets the max upload size
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('fasta', 'fas', 'fna', 'ffn', 'faa', 'frn');
  $form = array();

  $form['file'] = array(
    '#title' => t('FASTA File'),
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#description' => t('Upload a FASTA file.'),
    '#default_value' => isset($form_state['values']['fasta_file']) ? $form_state['values']['fasta_file'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      'file_validate_size' => array($upload_size * 1048576),
    ),
  );
  return $form;
}

function upload_genesequence_fasta_form_submit(array $form, array &$form_state) {
  $object = $form_state['islandora']['objects'][0];
  $models = $object->models;
  if(sizeof($models) == 1){
    $models = array_merge($form_state['islandora']['shared_storage']['models'], $models);
    $object->models = $models;
  }

  if (empty($object['FASTA'])) {
    $ds = $object->constructDatastream('FASTA', 'M');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['FASTA'];
  }
  $fasta_file = file_load($form_state['values']['file']);
  $fasta_path = drupal_realpath($fasta_file->uri);
  $ds->setContentFromFile($fasta_path, FALSE);
  $ds->label = $fasta_file->filename;
  $ds->mimetype = $fasta_file->filemime;
}
