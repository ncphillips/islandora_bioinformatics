<?php


function related_lab_objects_page_callback($object) {
  /**
   * @author Dalton Mackenzie <demackenzie@upei.ca>
   *
   * @param $object - We want to find all objects related to this target object.
   *
   * @return $render_array - A renderable array, containing forms for each content
   * model with objects related to the target object.
   *
   * Page Callback related_lab_objects_action($object)
   *
   * This page callback takes an object and determines
   * which lab object content models have a relationship
   * with its content model via SPARQL query.
   *
   * From here it should use drupal_get_form once for each
   * content model to generate a form populated with the lab
   * objects of the specified content model related to the given
   * object.
   *
   * These forms will be stored as a elements in a
   * renderable array, which the function will then return.
   *
   *
   * @todo Rename this function to related_lab_objects_action
   */
  $render_array = array();
  $target_pid = $object->models[0]; //Get the pid for the input object's content model, we'll want to see if any lab objects have an isPartOf relationship with this pid.

  $tuque = new IslandoraTuque(); //Establish a tuque object

  //Construct a query which will find all pids adhering to the following two conditions:
  //Condition 1: pid corresponds to a content model that has a data relationship with $object

  $query = "
    PREFIX fedora-model: <info:fedora/fedora-system:def/model#>
    PREFIX fedora: <info:fedora/fedora-system:def/relations-external#>
    PREFIX islandora: <" . ISLANDORA_RELS_EXT_URI . ">
    SELECT ?pid
    FROM <#ri>
    WHERE {
      ?pid islandora:isSubtypeOf <info:fedora/islandora:lab_object_cmodel> .
      ?pid ?rel <info:fedora/$target_pid>
    }";

  $results = $tuque->repository->ri->sparqlQuery($query); //Now get the results for this query.

  $related_cmodels = array();
  foreach ($results as $cmodel) {
    $related_cmodels[] = islandora_object_load($cmodel['pid']['value']);
  }

  foreach ($related_cmodels as $related_cmodel_object) { //For each of the content models returned by the query,

    $object_name = islandora_cmodel_name($related_cmodel_object);

    $form_id = 'related_' . $object_name . '_form';
    if (function_exists($form_id)) {
      $render_array[$object_name] = drupal_get_form($form_id, $object);
    }
    else {
      $args = array($object);
      $forms = module_invoke_all('forms', $form_id, $args );
      if (array_key_exists($form_id, $forms)) {
        $render_array[$object_name] = drupal_get_form($form_id, $object);
      }
      else {
        $render_array[$object_name] = array();
      }
    }
  }
  //Return the renderable array
  return $render_array;
}

function islandora_related_lab_object_ingest_action($object, $cmodel) {
  /**
   * @author Dalton Mackenzie <demackenzie@upei.ca>
   *
   * @param $object
   * This is an object which we wish to relate a new object to. The new object will be of type cmodel, and will need to be ingested.
   *
   * @param $cmodel
   * This is the actual content model object of the type of object we would like to create (and ingest) and then establish some form of relationship with $object.
   *
   * Page Callback callback_related_lab_objects_ingest
   *
   * This callback will take an object and a cmodel object as parameters. The callback will then determine what type of content model
   * the cmodel is, and it will call callback_rel_lab_object_SUBTYPE_ingest($object) where SUBTYPE is the type of the content model.
   */
  $object_name = islandora_cmodel_name($cmodel);

  $module_name = 'islandora_lab_object_' . $object_name;
  $callback_to_call = "{$module_name}_ingest";
  module_load_include('module', $module_name, $module_name);
  if (function_exists($callback_to_call)) {
    return $callback_to_call($object);
  }
  else
    return drupal_not_found();
}


function islandora_cmodel_name($cmodel) {
  /**
   * @param $cmodel
   * @return string
   */
  $pattern = '/islandora:(.*)_cmodel/';

  $content_model_pid = $cmodel->id;
  preg_match($pattern, $content_model_pid, $match);
  if (sizeof($match) >= 1)
    return $match[1];
  else
    return '';
}