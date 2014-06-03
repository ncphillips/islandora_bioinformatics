<?php

/**
 * Implements hook_form_related_people_form_alter()
 */
function islandora_bioinformatics_form_related_people_form_alter(&$form, &$form_state) {
  module_load_include('inc', 'islandora_bioinformatics', 'includes/mads_to_array');

  $object = $form_state['object'];
  $people = $form_state['related_objects'];

  // Table Headers:
  //  NAME      |       Phone Number      |       Email
  //----------------------------------------------------------
  //  Test      |       333-444-555       | test@example.com
  $header = array(
    'name' => t('Name'),
    'phone' => t('Phone Number'),
    'email' => t('Email'),
  );

  unset($form['cmodel']);

  $form['add'] = array(
    '#type' => 'markup',
    '#markup' => l('Assign', "islandora/object/{$object->id}/people/add"),
    '#prefix' => '<div class="m-btn green">',
    '#suffix' => '</div>',
  );

  unset($form['ingest']);

  $people_entries = array();

  foreach ($people as $person) {
    // Default Values
    if (!isset($person['MADS'])) break;

    $person_mads = person_mads_to_array($person);
    $person_id = $person->id;

    $name = $person_mads['name']['first'] . ' ' . $person_mads['name']['last'];

    $people_entries[$person_id] = array(
      'name' => l($name, "islandora/object/$person_id"),
      'phone' => $person_mads['contact']['phone'],
      'email' => $person_mads['contact']['email'],
    );
  }

  $form['table']['#header'] = $header;
  $form['table']['#options'] = $people_entries;
  $form['table']['#empty'] = t('There is no people assigned to ' . $object->label . '.');

  return $form;
}
