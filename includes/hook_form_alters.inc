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

  unset($form['Ingest']);

  $object = !empty($form_state['islandora']['object']) ? $form_state['islandora']['object'] : null;
  $people = !empty($form_state['islandora']['related_objects_ids']) ? $form_state['islandora']['related_objects_ids'] : array();

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
    $mads = person_mads_to_array($person);

    $name = $mads['authority']['given'] . ' ' . $mads['authority']['family'];
    $label = $mads['title'] ? $mads['title']: ($name ? $name : ($mads['id']? $mads['id']: $person_id));

    $people_entries[$person_id] = array(
      'name' => l($label, "islandora/object/$person_id"),
      'phone' => $mads['affiliation']['phone'],
      'email' => $mads['affiliation']['email'],
    );
  }

  $form['content_table_select']['table']['#options'] = $people_entries;
  $form['content_table_select']['table']['#empty'] = t('There is no people related to ' . $object->label . '.');

  return $form;
}


function islandora_bioinformatics_form_islandora_related_islandora_genesequence_cmodel_objects_form_alter(&$form, &$form_state) {
  $object = !empty($form_state['islandora']['object']) ? $form_state['islandora']['object'] : null;
  $sequences= !empty($form_state['islandora']['related_objects_ids']) ? $form_state['islandora']['related_objects_ids'] : array();
  
  $form['content_table_select']['table']['#header'] = array(
    'label' => 'Sequence',
    'download' => '',
  );

  foreach($sequences as $seq_id){
    $seq = islandora_object_load($seq_id);
    $label = $seq->label ? $seq->label: $seq_id;
    $seq_entries[$seq_id] = array(
      'label' => l($label, "islandora/object/$seq_id"),
      'download' => l('Download', "islandora/object/{$seq_id}/datastream/FASTA/download"),
    );
  }

  
  $empty_message = ($form_state['islandora']['current_form'] == 'View') ?
                   "There are no Genesequences associated with this object." :
                   "No Geneequences found.";

  $form['content_table_select']['table']['#options'] = $seq_entries;
  $form['content_table_select']['table']['#empty'] = $empty_message;

}


function islandora_bioinformatics_form_islandora_related_islandora_fraction_cmodel_objects_form_alter(&$form, &$form_state) {
  $object = !empty($form_state['islandora']['object']) ? $form_state['islandora']['object'] : null;
  $sequences= !empty($form_state['islandora']['related_objects_ids']) ? $form_state['islandora']['related_objects_ids'] : array();
  
  $form['content_table_select']['table']['#header'] = array(
    'label' => 'Sequence',
    'download' => '',
  );

  foreach($sequences as $seq_id){
    $seq = islandora_object_load($seq_id);
    $label = $seq->label ? $seq->label: $seq_id;
    $seq_entries[$seq_id] = array(
      'label' => l($label, "islandora/object/$seq_id"),
      'download' => l('Download', "islandora/object/{$seq_id}/datastream/FASTA/download"),
    );
  }

  
  $empty_message = ($form_state['islandora']['current_form'] == 'View') ?
                   "There are no Genesequences associated with this object." :
                   "No Geneequences found.";

  $form['content_table_select']['table']['#options'] = $seq_entries;
  $form['content_table_select']['table']['#empty'] = $empty_message;

}
