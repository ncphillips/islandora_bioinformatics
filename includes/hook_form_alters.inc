<?php
/**
 * @file
 */

/**
 * @param $form
 * @param $form_state
 * @return mixed
 */
function islandora_bioinformatics_form_islandora_related_islandora_specimen_cmodel_objects_form_alter(&$form, &$form_state) {
  module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');

  $specimens = $form_state['islandora']['related_objects_ids'];

 // Headers
  $header = &$form['content_table_select']['table']['#header'];

  $header = array(
    'label' => 'Label',
    'genus_species' => 'Genus <em>species</em>',
    'cmodel' => 'Model',
  );
  // Merge with the original headers.
//  $header = array_merge($header, $form['content_table_select']['table']['#header']) ;

  // Unset a couple we don't want.
  unset($header['id']);

  $options = &$form['content_table_select']['table']['#options'];

  foreach ($options as $specimen_id => &$rows) {
    $specimen = islandora_object_load($specimen_id);
    $eml = specimen_EML_to_array($specimen);

    $specimen_label = $eml['shortName'] ? $eml['shortName'] :
                     ($eml['title']     ? $eml['title']     : $specimen_id);
    $specimen_url = "<a href='/islandora/object/{$specimen_id}'>{$specimen_label}</a>";
    $genus_species = t($eml['taxonomicCoverage']['genus'] .
                     ' <em>'.strtolower($eml['taxonomicCoverage']['species']).'</em>');

    $rows['label'] = $specimen_url;
    $rows['genus_species'] = $genus_species;
    $rows['#attributes'] = array("style" => "max-height: 100px;");
  }

  $form['content_table_select']['table']['#type'] =  'tableselect';
  $form['content_table_select']['table']['#header'] =  $header;
  $form['content_table_select']['table']['#empty'] =  t("There are no related specimens.");

  return $form;
}


/**
 * Implements hook_form_FORM_ID_alter()
 *
 * WHERE  FORM_ID         = "islandora_related_RELATED_CMODEL_objects_form"
 * AND    RELATED_CMODEL  = "islandora:person_cmodel"
 */
function islandora_bioinformatics_form_islandora_related_islandora_person_cmodel_objects_form_alter(&$form, &$form_state) {
  module_load_include('inc', 'islandora_bioinformatics', 'includes/mads_to_array');

  $object = !empty($form_state['islandora']['object_id']) ? $form_state['islandora']['object_id'] : null;
  $people = !empty($form_state['islandora']['related_objects_ids']) ? $form_state['islandora']['related_objects_ids'] : array();


  unset($form['ingest_link']);

  $form['content_table_select']['table']['#header'] = array(
    'name' => 'Name',
    'phone' => 'Phone',
    'email' => 'Email',
  );

  $people_entries = array();

  foreach ($people as $person_id) {
    $person = islandora_object_load($person_id);
    // Default Values
    if (!isset($person['MADS'])) break;

    module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');
    $person_mads = person_mads_to_array($person);

    $name = $person_mads['name']['first'] . ' ' . $person_mads['name']['last'];

    $people_entries[$person_id] = array(
      'name' => l($name, "islandora/object/$person_id"),
      'phone' => $person_mads['contact']['phone'],
      'email' => $person_mads['contact']['email'],
    );
  }

  $form['content_table_select']['table']['#options'] = $people_entries;
  $form['content_table_select']['table']['#empty'] = t('There is no people related to ' . $object->label . '.');

  return $form;
}