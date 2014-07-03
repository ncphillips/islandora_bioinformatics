<?php



/**
 * Implements hook_preprocess_THEME()
 *
 * Where THEME = islandora_person
 */
function islandora_bioinformatics_preprocess_islandora_bioinformatics_person(array &$variables) {
  module_load_include('inc', 'islandora_bioinformatics', 'includes/mads_to_array');

  $islandora_object = $variables['islandora_object'];

  // Thumbnail.
  if (isset($islandora_object['IMG'])) {
    $thumbnail_size_url = url("islandora/object/{$islandora_object->id}/datastream/IMG/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $thumbnail_size_url,
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
    $variables['img_url'] = $thumbnail_size_url;
  }

  $variables["person"] = person_mads_to_array($islandora_object);

}


/**
 * Implements hook_preprocess_THEME()
 *
 * Where THEME = islandora_project
 */
function islandora_bioinformatics_preprocess_islandora_bioinformatics_project(array &$variables) {
  module_load_include('inc', 'islandora_bioinformatics', 'includes/mads_to_array');

  $islandora_object = $variables['islandora_object'];
  // Thumbnail.
  if (isset($islandora_object['IMG'])) {
    $thumbnail_size_url = url("islandora/object/{$islandora_object->id}/datastream/IMG/view");
    $params = array(
      'title' => $islandora_object->label,
      'path' => $thumbnail_size_url,
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
    $variables['img_url'] = $thumbnail_size_url;
  }

  $variables["project"] = project_mads_to_array($islandora_object);
}
