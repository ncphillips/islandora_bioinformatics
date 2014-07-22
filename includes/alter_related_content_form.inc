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

 t // Headers
  $header = array(
    'shortName' => t('Name'),
    'genus_species' => t('Genus species'),
  );

  // Merge with the original headers.
  $header = array_merge($header, $form['content_table_select']['table']['#header']) ;

  // Unset a couple we don't want.
  unset($header['id']);
  unset($header['label']);

  $options = &$form['content_table_select']['table']['#options'];

  foreach ($options as $specimen_id => &$rows) {
    $specimen = islandora_object_load($specimen_id);
    $eml = specimen_EML_to_array($specimen);

    $specimen_url = "<a href='/islandora/object/{$specimen_id}'>{$eml['shortName']}</a>";
    $genus_species = t($eml['taxonomicCoverage']['genus'] .
                     ' <em>'.strtolower($eml['taxonomicCoverage']['species']).'</em>');

    $rows['shortName'] = $specimen_url;
    $rows['genus_species'] = $genus_species;
    $rows['#attributes'] = array("style" => "max-height: 100px;");
  }

  $form['content_table_select']['table']['#type'] =  'tableselect';
  $form['content_table_select']['table']['#header'] =  $header;
  $form['content_table_select']['table']['#empty'] =  t("There are no related specimens.");

  return $form;
}