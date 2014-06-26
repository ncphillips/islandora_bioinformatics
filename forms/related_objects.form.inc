<?php
/**
 * @file
 */

function islandora_related_objects_form($form, &$formstate, $object_id, $cmodel_id, $relationships=null){
  module_load_include('inc', 'islandora_bioinformatics', 'includes/query_helper_functions');

  if(empty($related_objects)){
    $ro_args = array(
      'object' => $object_id,
      'cmodels' => array($cmodel_id),
      'relationships' => $relationships,
    );
    $related_objects = islandora_object_related_islandora_objects($ro_args);
  }

  $formstate['islandora']['object'] = $object_id;
  $formstate['islandora']['cmodel'] = $cmodel_id;
  $formstate['islandora']['relationships'] = $relationships;
  $formstate['islandora']['related_objects'] = $related_objects;

  // Fedora Objects
  $object = islandora_object_load($object_id);
  $cmodel = islandora_cmodel_load($cmodel_id);

  // Function Name Safe CModel ID
  $cmodel_id_fns = str_replace(':','_', $cmodel_id);

  // Label
  $cmodel_label = $cmodel->label;

  // Permissions
  $can_ingest = islandora_object_access(ISLANDORA_INGEST,            $object);
  $can_add    = islandora_object_access(ISLANDORA_METADATA_EDIT,     $object)
            and islandora_object_access(ISLANDORA_MANAGE_PROPERTIES, $object);


  // Header
  $form['header'] = array(
    '#markup' => $cmodel->label,
    '#prefix' => '<h1>',
    '#suffix' => '</h1>',
  );

  // Solr Filter
  $form['solr_filter'] = array(
    '#prefix'=> "<div id='solr_filter_$cmodel_id_fns'>",
    '#suffix'=> '</div>',
  );

  $form['solr_filter']['fields'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#options' => array(
      'PID' => 'PID',
      'RELS_EXT_hasModel_uri_mt' => 'Content Type',
    ),
  );

  $form['solr_filter']['textfield'] = array(
    '#type' => 'textfield',
    '#default_value' => '*',
    '#title' => 'Value'
  );

  $form['solr_filter']['apply_filter'] = array(
    '#type' => 'submit',
    '#value' => 'Apply Filter',
    '#ajax' => array(
      'callback' => 'add_filter_display',
      'wrapper' => "solr_filter_current_filters_$cmodel_id_fns",
    ),
  );

  $form['solr_filter']['current_filters'] = array(
    '#prefix'=> "<div id='solr_filter_current_filters_$cmodel_id_fns'>",
    '#suffix'=> '</div>',

  );



  if($can_ingest) {
    $form['ingest'] = array(
      '#type' => 'markup',
      '#markup' =>  l("Ingest {$cmodel_label}",
                      "islandora/object/{$object->id}/related_content/ingest/{$cmodel_id}"),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }

  if($can_add){
    $form['add link'] = array(
      '#type' => 'markup',
      '#markup' =>  l("Add Existing {$cmodel_label}",
                      "islandora/object/{$object->id}/related_content/add/{$cmodel_id}"),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }

  // From our form there should be a list of specific operations that can be performed
  // on the objects in the list.
  $form['actions'] = array();
  $form['actions']['selected'] = array(
    '#type' => 'select',
    '#title' => 'Actions',
    '#options' => array(
      'remove' => t('Remove'),
    ),
//    '#description' => t('Actions'),
  );

  // Submit button for form actions.
  $form['actions']['submit'] =  array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#submit' => array('islandora_related_objects_submit'),
  );


  // Related Objects Table
  $header = array(
    'id' => 'ID',
    'label' => 'Label',
    'cmodel' => 'Type',
  );

  $options = array();

  foreach ($related_objects as $related_object) {
    $options[$related_object->id] = array(
      'id' => $related_object->id,
      'label' => $related_object->label,
      'cmodel' => islandora_object_parent_model($related_object->id),
    );
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t("Null."),
    '#prefix' => "<div id='solr_filter_$cmodel_id_fns'>",
    '#suffix' => '</div>',

  );

  return $form;
}

function islandora_related_objects_submit(array $form, array &$form_state) {
  $object = $form_state['islandora']['object'];
  $cmodel = $form_state['islandora']['cmodel'];
  $relationships = $form_state['islandora']['relationships'];
//  var_dump($relationships); exit();

  // Checked values in the table.
  $table_values = $form_state['values']['table'];

  // Action selected in teh drop down.
  $selected_option = $form_state['values']['selected'];
  switch ($selected_option) {
    case "remove":
      foreach ($table_values as $lines => $selected_pid) {
        if ($selected_pid) {
          $related_object = islandora_object_load($selected_pid);

          foreach($relationships as $pred){
              $related_object->relationships->remove(null, $pred, $object->id);
          }
        }
      }
      break;
  }
}


function add_filter_display($form, &$form_state) {
  $cmodel = $form_state['islandora']['cmodel'];
  $cmodel_id_fns = str_replace(':', '_', $cmodel->id);
  return array(
    '#markup' => 'wat',
    '#prefix'=> "<div id='solr_filter_current_filters_$cmodel_id_fns'>",
    '#suffix'=> '</div>',

  );
}