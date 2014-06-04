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

// function islandora_object_related_islandora_objects($object_id, $cmodels=NULL, $relationships=NULL) {
//  /**
//   * @author Nolan Phillips ncphillips@upei.ca
//   *
//   * @param $object_id
//   *
//   * @param $cmodels
//   * An array of strings representing fedora IDs of Content Models.
//   *
//   * @param $relationships
//   * An associative array whose keys are namespaces, and whose values
//   * are arrays of relationships in that namespace.
//   *
//   * @return $related_objects
//   * An array of IslandoraFedoraObjects that are related to the $object_id.
//   *
//   */
//
//  $obj_id = $object_id;
//  $cmodel_query = '';  // A union of CModel tripples.
//
//  $pre = array();  // An associative array of namespace => prefix.
//  $pre_defs = array();  // An array of prefix definitions.
//
//  // Default values for $pre and $pre_defs
//  $pre[FEDORA_MODEL_URI] = 'fedora-model:';
//
//  // Besides 'fedora-model', the first prefix being used is 'a'.
//  $prefix = 'a';$pre_defs[] = 'PREFIX fedora-model: <info:fedora/fedora-system:def/model#>';
//
//  /**
//   * ## CModel Query Construction
//   * If an Fedora Object's CModel set is a subset of $cmodels then the object_id
//   * may be returned; however, if the set of CModels passed in is empty, then
//   * an object_id is accepted regardless of it's CModel set.
//   */
//  if ($cmodels) {
//    $cmodel_tripples= array();
//    foreach ($cmodels as $cmodel)
//      $cmodel_tripples[] = "{?pid fedora-model:hasModel <info:fedora/$cmodel>}";
//
//    // Union all the cmodel tripple queries.
//    $cmodel_query = '{' . implode(' UNION ', $cmodel_tripples) . '}';
//  }
//
//  // Construct the set of prefix definitions.
//  /**
//   * ## Namespace Prefix Definitions
//   * ### Without Namespaces
//   * A Fedora Object may be accepted if it has any relationship to the target $object_id.
//   *
//   * ### Namespaces without relationships
//   * A Fedora Object may be accepted if it has any relationship to the
//   * target $object_id that is defined within any of the namespaces provided.
//   *
//   * ### Namespaces with relationships
//   * A Fedora Object may be accepted if it has a relationship to the
//   * target $object_id that is listed in that namespaces associative array.
//   */
//  if ($relationships) {
//    // Define PREFIX statements
//    foreach ($relationships as $ns => $rel) {
//      $pre[$ns] = $prefix . ':';
//      $pre_defs[] = 'PREFIX ' . $prefix++ . ": <$ns>";
//    }
//    $pre_def = implode(' ', $pre_defs);
//
//    // Define relationship tripples
//    foreach ($relationships as $ns => $rels) {
//      foreach ($rels as $rel) {
//        $nsrel = $pre[$ns] . $rel;
//        $rel_tripples[] = "{?pid $nsrel <info:fedora/$obj_id>}";
//      }
//    }
//    // Union all the relationship tripples
//    $rel_query = '{' . implode(' UNION ', $rel_tripples) . '}';
//  }
//  else {
//    $pre_def = $pre_defs[0];
//    $rel_query = "{?pid ?any_rel <info:fedora/$obj_id>}";
//  }
//
//  $query = "
//    $pre_def
//    SELECT ?pid
//    FROM <#ri>
//    WHERE {
//        $cmodel_query
//        $rel_query
//    }
//  ";
//
//  // For testing porpoises.
//  //file_put_contents('/tmp/sparqlquery.txt', $query);
//
//  // Run the query.
//  $tuque = new IslandoraTuque();
//  $result = $tuque->repository->ri->sparqlQuery($query);
//
//  $related_objects = array();
//  foreach ($result as $related_object) {
//    $obj = islandora_object_load($related_object['pid']['value']);
//    $related_objects[] = $obj ;
//  }o


  /**
   * ## Example Query
   * Here is an example of query that may be produced within this function.
   *
   *      PREFIX a: <info:fedora/fedora-system:def/relations-external#>
   *      PREFIX b: <info:fedora/fedora-system:def/model#>
   *  	 SELECT ?pid
   *  	 FROM <#ri>
   *  	 WHERE
   *      {
   *        {
   *          {?pid b:hasModel <info:fedora/islandora:person_cmodel>}
   *          UNION
   *          {?pid b:hasModel <info:fedora/islandora:child_cmodel>}
   *        }
   *        {
   *          {?pid a:isMemberOf <info:fedora/islandora:524>}
   *        }
   *      }
   */

//  return $related_objects;
