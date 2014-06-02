<?php
/**
 * @file
 */

/**
 * image_upload_form
 *
 * A form which allows a user to upload an image to be stored in an
 * Islandora Object's IMG datastream.
 */
function image_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('gif png jpg jpeg');
  return array(
    'file' => array(
      '#title' => t('Image'),
      '#type' => 'managed_file',
      '#required' => FALSE,
      '#description' => t('Select image to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
      '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
      '#upload_location' => 'temporary://',
      '#upload_validators' => array(
        'file_validate_extensions' => $extensions,
        // Assume it's specified in MB.
        'file_validate_size' => array($upload_size * 1024 * 1024),
      ),
    ),
  );
}

/**
 * image_upload_form_submit
 *
 * Stores an uploaded image into the Islandora Object's IMG datastream.
 */
function image_upload_form_submit(array $form, array &$form_state) {
  $file = file_load($form_state['values']['file']);
  if (!empty($file)) {  //check if a file has been uploaded
    $object = $form_state['islandora']['objects'][0];
    if (empty($object['IMG'])) {
      $ds = $object->constructDatastream('IMG', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['IMG'];
    }

    $path = drupal_realpath($file->uri);
    $ds->setContentFromFile($path, FALSE);
    $ds->label = $file->filename;
    $ds->mimetype = $file->filemime;
  }
}
