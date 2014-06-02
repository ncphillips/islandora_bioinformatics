<?php
/**
 * @file
 * Handles the assignment of person object to a  project object
 */

/**
 * Implements hook_form()
 */
function project_assign_person_form(array $form, array &$form_state) {
  $tuque = new IslandoraTuque();

  $project_object = $form_state['islandora']['objects'][0]; //this is NULL

  // Query fedora to retrieve a list of all person objects.
  $query = "SELECT ?pid
            FROM <#ri>
            WHERE {
              ?pid <fedora-model:hasModel> <info:fedora/islandora:person_cmodel> .
            }";

  $result = $tuque->repository->ri->sparqlQuery($query, 'unlimited');

  $people = array();
  $default_values = array();  // Determines which people are automatically selected.
  foreach ($result as $person) {

    //load the current person object
    $person_object = islandora_object_load($person["pid"]["value"]);
    $people[$person_object->id] = $person_object->label;

    $relationship = $person_object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOf', $project_object->id);

    if (!empty($relationship)) {  //if relationship exists, add it to $default_values
      $default_values[$person_object->id] = $person_object->id;
    }
  }

  $form['header'] = array(
    '#markup' => 'Assign people to this project',
    '#prefix' => '<h1>',
    '#suffix' => '</h1>',
  );


  $form['people'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $default_values,
    '#options' => $people,
  );

  return $form;
}

/**
 * Implements hook_form_submit()
 */
function project_assign_person_form_submit(array $form, array &$form_state) {
  $tuque = new IslandoraTuque();

  //retrieves the relevant fedora project object
  $project_object = $form_state['islandora']['objects'][0];

  //get the checkboxes submitted (only those checked are submitted)  
  $person_ids = array_filter($form_state['values']['people']);

  //iterate over every person and remove their relationship (if it exists) with the project.
  $query = "SELECT ?pid
            FROM <#ri>
            WHERE {
            ?pid <fedora-model:hasModel> <info:fedora/islandora:person_cmodel> .
        }";
  $result = $tuque->repository->ri->sparqlQuery($query, 'unlimited');

  foreach ($result as $person) {
    $person_object = islandora_object_load($person['pid']['value']);  //loads object
    $person_object->relationships->remove(FEDORA_RELS_EXT_URI, 'isMemberOf',
      $project_object->id);
  }

  //for each person checked, create a relationship between the person and the project
  foreach ($person_ids as $id)  {
    //create a relationship within the current person object to the project
    $person_object = islandora_object_load($id); //loads object
    $person_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf',
      $project_object->id);
  }
}
