<?php
/**
 * @file
 */


function related_objects_form($form, &$form_state, $object, $cmodels=NULL, $relationships=NULL) {
  /**
   * ## related_islandora_objects_form
   *
   * @author Nolan Phillips ncphillips@upei.ca
   * @param $form
   * @param $form_state
   * @param $object
   * The Islandora Object whose relatives we are trying to find.
   *
   * @param $cmodels
   * An array of CModel PIDs
   *
   *      array('islandora:lcms_cmodel', 'islandora:book_cmodel')
   *
   * @param $relationships
   * An associative array in the form of:
   *
   *      array(
   *        'namespace 1' => array('rel1', 'rel2', 'rel3'),
   *        'namespace 2' => array('rel4'),
   *      )
   *
   *
   * Constructs a form which has a select table of islandora objects related to the
   * $object. A related object is listed iff it's cmodel is in $cmodels and the
   * relationship to $object is in $relationships.
   *
   * @todo This function isn't generic enough. It will list all CModels passed
   * in through the $cmodels array; but, the title of the form and the links to
   * add new objects will be based only off the first CModel provided. It might
   * be best if this link would link to a callback function that deals with this
   * in a way that is similar to how the Collection Solution Pack deals with
   * adding objects to collections that can contain multiple types of content.
   */


  echo islandora_cmodel_parent('islandora:project_cmodel');

  module_load_include('inc', 'islandora_bioinformatics', 'includes/sparql_helper_functions');
  $related_objects = islandora_object_related_islandora_objects($object, $cmodels, $relationships);

  $form_state['object'] = $object;
  $form_state['related_objects'] = $related_objects;
  if (isset($cmodels)) $form_state['cmodels'] = $cmodels;
  if (isset($relationships)) $form_state['relationships'] = $relationships;

  $cmodel = islandora_object_load($cmodels[0]);

  $cmodel_label = $cmodel->label;

  // Header form
  $form['cmodel'] = array(
    '#markup' => $cmodel_label,
    '#prefix' => '<h1>',
    '#suffix' => '</h1>',
  );

  // Add Link
  $form['ingest'] = array(
    '#type' => 'markup',
    '#markup' =>  l("Ingest $cmodel_label", "islandora/object/{$object->id}/ingest/{$cmodel_label}"),
    '#prefix' => '<div class="m-btn green">',
    '#suffix' => '</div>',
  );

  $form['add'] = array(
//    '#type' => 'type',
//    '#markup' =>  l("Add Existing $cmodel_label", "islandora/object/{$object->id}/add_lab_object_to_project"),
//'#prefix' => '<div class="m-btn green">',
//'#suffix' => '</div>',
  );

  // From our form there should be a list of specific operations that can be performed
  // on the objects in the list.
  $form['selected'] = array(
    '#type' => 'select',
    '#title' => 'Actions',
    '#options' => array(
      'remove' => t('Remove'),
    ),
    '#description' => t('Actions'),
  );

  // Submit button for form actions.
  $form['submit'] =  array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#submit' => array('related_islandora_objects_submit'),
  );

  $header = array(
    'id' => 'ID',
  );

  $options = array();

  foreach ($related_objects as $related_object) {
    $options[$related_object->id] = array(
      'id' => $related_object->id,
    );
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t("Null."),
  );

  //adds object to the form
  $form['object'] = array(
    '#type' => 'value',
    '#value' => $object,
  );

  return $form;
}

function related_islandora_objects_submit(array $form, array &$form_state) {

  /**
   * This function is the submit handler for the related_islandora_objects_form function
   * It performs $selected_object actions on the $table_values which have been selected
   * from the related objects table.
   *
   * @author Natasha Squires <nsquires@upei.ca>
   *
   * @param array $form
   * The form
   * @param array &$form_state
   * the form state
   *
   * */
  //which action was selected form the drop down menu
  $selected_option = $form_state['values']['selected'];
  //the object in question
  $object = $form_state['values']['object'];
  //the selected table values
  $table_values = $form_state['values']['table'];

  switch ($selected_option) {
    //in case of relationship removal from the object
    case "remove":
      //for each table row
      foreach ($table_values as $lines => $line) {
        //if $line exists (if it is a selected table row)
        if ($line) {
          //load the object in the table row
          $related_object = islandora_object_load($line);
          //remove its relationship with the object in question
          $related_object->relationships->remove(FEDORA_RELS_EXT_URI, 'isPartOf', $object->id);
        }
      }
      break;
  }
}