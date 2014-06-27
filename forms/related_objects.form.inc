<?php
/**
 * @file
 */

/**
 * @param        $form
 * @param        $form_state
 * @param string $object_id
 * @param string $cmodel_id
 * @param array  $relationships
 *
 * @return mixed
 *
 *
 * # Form State
 * @code
 *  array(
 *    'islandora' = array(
 *      'object_id' = '',
 *      'cmodel_id' = '',
 *      'relationships' = array(''),
 *      'related_objects_ids' = array(''),
 *      'page' = 0
 *      'solr_filters' = array(
 *    ),
 *
 *    ),
 *  );
 * @endcode
 */
function islandora_related_objects_form($form, &$form_state, $object_id='', $cmodel_id='', $relationships=array()){
  !empty($form_state['islandora']['object_id']) ? : ($form_state['islandora']['object_id'] = $object_id);
  !empty($form_state['islandora']['cmodel_id']) ? : ($form_state['islandora']['cmodel_id'] = $cmodel_id);
  !empty($form_state['islandora']['relationships']) ? : ($form_state['islandora']['relationships'] = $relationships);
  !empty($form_state['islandora']['page']) ? : ($form_state['islandora']['page'] = 0);
  !empty($form_state['islandora']['solr_filters']) ? : ($form_state['islandora']['solr_filters'] = array());
  !empty($form_state['islandora']['refresh_query']) ? : ($form_state['islandora']['refresh_query'] = TRUE);

  $object = islandora_object_load($object_id);
  $cmodel = islandora_cmodel_load($cmodel_id);

  islandora_related_objects_ajax_response($form_state);

  // Execut Solr Query
  if($form_state['islandora']['refresh_query']){
    // Add Filters.
    if($form_state['islandora']['solr_filters']){
      $other_queries = '('.implode(' AND ', $form_state['islandora']['solr_filters']).')';
    }
    else {
      $other_queries = '';
    }

    // Execute Query
    $related_objects_ids = islandora_object_related_islandora_objects(array(
        'object' => $object_id,
        'cmodels' => array($cmodel_id),
        'relationships' => $relationships,
        'other' => $other_queries,
      )
    );

    // Add values to the $form_state
    $form_state['islandora']['related_objects_ids'] = $related_objects_ids;
  }
  else{
    // Otherwise fetch them.
    $related_objects_ids = $form_state['islandora']['related_objects_ids'];
  }

  // Some Strings
  $cmodel_id_fns = str_replace(':','_', $cmodel_id);
  $form_div_id = "islandora_related_{$cmodel_id_fns}_form";
  $ajax_callback = 'islandora_related_objects_ajax';

  // Permissions
  $can_ingest = islandora_object_access(ISLANDORA_INGEST,            $object);
  $can_add    = islandora_object_access(ISLANDORA_METADATA_EDIT,     $object)
            and islandora_object_access(ISLANDORA_MANAGE_PROPERTIES, $object);


  // Wrape the entire form.
  $form['#prefix'] = "<div id='$form_div_id'>";
  $form['#suffix'] = '</div>';

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

  $form['solr_filter']['solr_field'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#options' => array(
      'PID' => 'PID',
      'RELS_EXT_hasModel_uri_mt' => 'Content Type',
    ),
  );

  $form['solr_filter']['solr_value'] = array(
    '#type' => 'textfield',
    '#default_value' => '*',
    '#title' => 'Value',
  );

  $form['solr_filter']['apply_filter'] = array(
    '#type' => 'button',
    '#value' => 'Apply Filter',
    '#ajax' => array(
      'callback' => $ajax_callback,
      'wrapper' => $form_div_id,
      'method' => 'replace',
    ),
  );

  $form['solr_filter']['current_filters'] = array(
    '#prefix' => "<div id='solr_filter_current_filters_$cmodel_id_fns'>",
    '#suffix' => '</div>',
  );

  foreach($form_state['islandora']['solr_filters'] as $filter){
    $form['solr_filter']['current_filters'][$filter] = array(
      '#type' => 'button',
      '#value' => $filter,
      '#prefix' => '<div class="solr_filter">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => $ajax_callback,
        'wrapper' => $form_div_id,
        'event' => 'mousedown',
      ),
    );
  }

  // Ingest Objects
  $form['ingest'] = array(
    '#type' => 'markup',
    '#markup' =>  l("Ingest {$cmodel->label}",
      "islandora/object/{$object->id}/related_content/ingest/{$cmodel->id}"),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#access' => $can_ingest
  );

  // Add Objects
  $form['add link'] = array(
    '#type' => 'markup',
    '#markup' =>  l("Add Existing {$cmodel->label}",
      "islandora/object/{$object->id}/related_content/add/{$cmodel->id}"),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#access' => $can_add
  );

  // Actions
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
  $form['previous_page'] = array(
    '#type' => 'button',
    '#value' => 'previous',
    '#ajax' => array(
      'callback' => $ajax_callback,
      'wrapper' => $form_div_id,
      'method' => 'replace',
    ),
  );

  $form['page_num'] = array(
    '#markup' => $form_state['islandora']['page'],
  );

  $form['next_page'] = array(
    '#type' => 'button',
    '#value' => 'next',
    '#ajax' => array(
      'callback' => $ajax_callback,
      'wrapper' => $form_div_id,
      'method' => 'replace',
    ),
  );

  $header = array(
    'id' => 'ID',
    'label' => 'Label',
    'cmodel' => 'Type',
  );

  // Table Options/Rows
  $options = array();
  foreach ($related_objects_ids as $related_object_id) {
    $related_object = islandora_object_load($related_object_id);
    $options[$related_object_id] = array(
      'id' => $related_object_id,
      'label' => $related_object->label,
      'cmodel' => islandora_object_parent_model($related_object_id),
    );
  }

  // Table
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => array(
      'id' => 'ID',
      'label' => 'Label',
      'cmodel' => 'Type',
    ),
    '#options' => $options,
    '#empty' => t("Null."),
    '#prefix' => "<div id='solr_filter_$cmodel_id_fns'>",
    '#suffix' => '</div>',
  );

  return $form;
}

function islandora_related_objects_ajax_response(&$form_state){
  if(empty($form_state['triggering_element'])) return;

  // AJAX
  $i = &$form_state['islandora'];
  $trigger = &$form_state['triggering_element'];
  $values = &$form_state['values'];

  // Can't predict Filter button values, so we identify them based off the #prefix.
  if(!empty($trigger['#prefix']) and $trigger['#prefix'] == '<div class="solr_filter">'){
    $filter = $trigger['#value'];
    unset($i['solr_filters'][$filter]);
  }

  // Other buttons
  if(!empty($trigger['#value'])){

    // Apply Filter
    if($trigger['#value'] == 'Apply Filter'){
      // Field, Value, Query
      $filter =  $values['solr_field'] . ':' . $values['solr_value'];
      $i['solr_filters'][$filter] = $filter;
      $i['page'] = 0;
    }

    // Next Page
    elseif($trigger['#value'] == 'next'){
      $i['page']++;
    }

    // Previous Page
    elseif($trigger['#value'] == 'previous'){
      if($i['page'] > 0) {
        $i['page']--;
      }
      else {
        $i['refresh_query'] = FALSE;
      }
    }
  }

  $_GET['page'] = $i['page'];
}

function islandora_related_objects_submit(array $form, array &$form_state) {
  $object_id = $form_state['islandora']['object_id'];
  $object = islandora_object_load($object_id);
  $relationships = $form_state['islandora']['relationships'];

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
            $related_object->relationships->remove(NULL, $pred, $object_id);
          }
        }
      }
      break;
  }
}


function islandora_related_objects_ajax($form, &$form_state) {
  return $form;
}

