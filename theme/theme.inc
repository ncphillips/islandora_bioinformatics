<?php
/**
 * @file
 */

/**
 * Implements hook_preprocess_THEME()
 *
 * Where THEME = islandora_person
 */
function islandora_bioinformatics_preprocess_islandora_bioinformatics_person(array &$variables) {
  module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');

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
  module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');

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

  $variables['mads'] = project_mads_to_array($islandora_object);
}





/**
 * The preprocess hook. It is responsible for putting anything that the template
 * might need into an array called variables. Doesn't technically return
 * this $variables array. This must just be included before theme stuff
 *
 * @param variables
 * Reference to the variables array that we'll populate
 */
function islandora_bioinformatics_preprocess_islandora_bioinformatics_specimen(array &$variables) {

  $islandora_object = $variables['islandora_object'];

  // get some of the EML data
  module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');
  $variables['eml'] = specimen_EML_to_array($islandora_object);

  //get projects associated with the specimen
  $tuque = new IslandoraTuque();
  $project_query = "SELECT ?pid
                    FROM <#ri>
                    WHERE {
                    <info:fedora/{$islandora_object->id}> ?rel ?pid .
                    ?pid <fedora-model:hasModel> <info:fedora/islandora:project_cmodel> .
                    }";
  $result = $tuque->repository->ri->sparqlQuery($project_query, 'unlimited');

  $related_projects = array();
  foreach ($result as $project) {
    $project_object = islandora_object_load($project['pid']['value']);
    $related_projects[$project_object->id] = $project_object->label;
  }
  $variables['related_projects'] = $related_projects;

  $variables['object_id'] = $islandora_object->id;
}

// here's where you can change the $variables array before the page is loaded
function islandora_bioinformatics_preprocess_islandora_bioinformatics_fraction(array &$variables) {

  $islandora_object = $variables['islandora_object'];

  $variables['edit_url'] = 'http://' . $_SERVER['SERVER_NAME'] . '/islandora/edit_form/' . urlencode($islandora_object->id) . '/fraction';

  $ns = "http://www.upeikerrlab.ca";

  $obj = new SimpleXMLElement($islandora_object['fraction']->content);

  $xml_objects = $obj->children($ns);

  $variables['type'] = $xml_objects->type;
  $variables['id'] = $xml_objects->id;
  $variables['lab_id'] = $xml_objects->labid;
  $variables['weight'] = $xml_objects->weight;
  $variables['plate'] = $xml_objects->plate;
  $variables['location'] = $xml_objects->location;
  $variables['notes'] = $xml_objects->notes;

  $variables['assays'] = array();
  $i = 0;
  foreach ($xml_objects->inhibitor as $assay) {
    $variables['assays'][$i]['name'] = (string) $assay->name;
    $variables['assays'][$i]['result'] = (string) $assay->result;
    $variables['assays'][$i]['comment'] = $assay->comment;
    $i++;
  }

  $variables['test_var'] = $xml_objects;

  //get specimen associated with the fraction
  $tuque = new IslandoraTuque();
  $specimen_query = "SELECT ?pid
                    FROM <#ri>
                    WHERE {
                    ?pid <fedora-model:hasModel> <info:fedora/islandora:specimen_cmodel> .
                    }";
  $result = $tuque->repository->ri->sparqlQuery($specimen_query, 'unlimited');

  $related_specimens = array();
  foreach ($result as $specimen) {
    $specimen_object = islandora_object_load($specimen['pid']['value']);
    if ($islandora_object->relationships->get(FEDORA_RELS_EXT_URI, 'isPartOf', $specimen_object->id)) {
      $related_specimens[$specimen_object->id] = $specimen_object->label;
    }
  }

  $variables["related_specimens"] = $related_specimens;
}

/**
 * Preprocess hook for fraction table. this will take in an arbitrary number of fraction
 * objects and add them to the variables array. They will then be printed out in the tpl.
 *
 * @param array $variables
 *  The variables array for modification
 */
function islandora_bioinformatics_preprocess_islandora_bioinformatics_fraction_table(array &$variables) {
  $i = 0;
  foreach ($variables['islandora_objects'] as $fraction) {

    $variables['fractions'][$i] = fraction_object_to_array($fraction);
    $variables['fractions'][$i]['pid'] = $fraction->id;

    $i++;
  }

}

function islandora_bioinformatics_lcms_preprocess_islandora_bioinformatics_lcms_upload_form(array &$variables) {

  $islandora_object = $variables['islandora_object'];
  // CSV and RAW
  if (isset($islandora_object['LCMS'])){

    //See if this is actually reached.
    echo "Did this work?!";

    $lcms_data = url("islandora/object/{$islandora_object->id}/datastream/LCMS/view");
    $variables["LCMS_DATA"] = $lcms_data;
  }

}