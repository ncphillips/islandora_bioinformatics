<?php

function islandora_object_related_islandora_objects($id=null, $cmodels=null,  $relationships=null, $subject=FALSE) {

  // The default query finds objects that are targeting $id. If subject is set
  // to true then it finds objects that $id is targeting.
  if($id)
    $id = "<info:fedora/$id>";
  else
    $id = "?id";
  $sub = "?relative";
  $obj = $id;



  if($subject){
    $sub = $id;
    $obj = "?relative";
  }

  $cm_query = '';
  $rel_query = '{' . "$sub ?relationship $obj" . '}';

  // CModels - The objects in question must be one of the listed CModels.
  if($cmodels){
    $cm_triples = array();
    foreach($cmodels as $cmodel)
      $cm_triples[] = "{ ?relative fedora-model:hasModel  <info:fedora/$cmodel>}";

    $cm_query = '{' . implode('UNION', $cm_triples) . '}';
  }

  // Prefices and Relationships
  $pre[FEDORA_MODEL_URI] = 'fedora-model:';
  $prefix_def_array[] = 'PREFIX fedora-model: <info:fedora/fedora-system:def/model#>';
  if($relationships) {
    // We define the fedora-model prefix by default.
    $prefix = 'a';
    $rel_tripples = array();

    foreach ($relationships as $namespace => $rels) {
      // Create a PREFIX for each namespace.
      $pre[$namespace] = $prefix . ':';
      $prefix_def_array[] = "PREFIX {$pre[$namespace]} <$namespace>";

      // Create a triple for each relationship in the namespace.
      foreach($rels as $rel){
        $nsrel = $pre[$namespace] . $rel;
        $rel_tripples[] = '{' . "$sub $nsrel $obj" . '}';
      }
    }
    $rel_query = implode('UNION', $rel_tripples);
  }
  $prefix_definitions = implode(' ', $prefix_def_array);

  $query = "
    $prefix_definitions
    SELECT *
    FROM <#ri>
    WHERE
    {
      $cm_query
      $rel_query
    }
  ";

  file_put_contents('/Users/nolan/Desktop/query.txt', $query);

  $tuque = new IslandoraTuque();
  $result = $tuque->repository->ri->sparqlQuery($query);

  $related_objects = array();
  foreach ($result as $related_object) {
    $obj = islandora_object_load($related_object['relative']['value']);
    $related_objects[] = $obj ;
  }

  return $related_objects;
}
