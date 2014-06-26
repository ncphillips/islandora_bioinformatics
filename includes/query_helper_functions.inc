<?php
/**
 * @file query_helper_functions.inc
 *       Provides a couple generic functions for performing Solr/Sparql queries.
 */

/**
 * islandora_object_related_islandora_objects
 *
 * @param       $object_id
 * @param array $cmodel_ids
 * @param array $relationships
 * @code
 *  array(
 *    'object' => 'islandora:555',
 *    'cmodels' => array('islandora:specimen_cmodel'),
 *    'relationships' => array('isPartOf'),
 *    'start' => 10,
 *    'limit' => 25,
 * )
 *
 * @endcode
 *
 * @return array
 */
function islandora_object_related_islandora_objects($args){
  // Executes Solr Queries
  $query_processor = new IslandoraSolrQueryProcessor();
  islandora_xacml_api_islandora_solr_query($query_processor);

  // Object, CModels, Relationships
  $object_id = $args['object'] ? str_replace(':', '\:', $args['object']) : '*';
  $cmodel_ids = $args['cmodels'] ? $args['cmodels'] : array();
  $relationships = $args['relationships'] ? $args['relationships'] : array();

  $query_parts = array(
    // [0] => CModelSubquery
    // [1] => RelationshipSubquery
  );

  // CModel rels queries
  $cmodel_subquery = array();
  foreach($cmodel_ids as $cmodel_id){
    $cmodel_id = str_replace(':', '\:', $cmodel_id);
    $cmodel_subquery[] = "RELS_EXT_hasModel_uri_mt:$cmodel_id";
  }

  if ($cmodel_subquery)
    $query_parts[] = '(' . implode(' OR ', $cmodel_subquery) . ')';

  // Relationships
  // If the relationships field is empty then we use all Fedora rels.
  if(empty($relationships)){
    $FEDORA_RELS_EXT_RELATIONSHIPS =  array(
      'fedoraRelationship',
      'isPartOf',             'hasPart',
      'isConstituentOf',      'hasConstituent',
      'isMemberOf',           'hasMember',
      'isSubsetOf',           'hasSubset',
      'isMemberOfCollection', 'hasCollectionMember',
      'isDerivationOf',       'hasDerivation',
      'isDependentOf',        'hasDependent',
      'isDescriptionOf',      'hasDescription',
      'isMetadataFor',        'hasMetadata',
      'isAnnotationOf',       'hasAnnotation',
      'hasEquivalent'
    );
    $relationships = $FEDORA_RELS_EXT_RELATIONSHIPS;
  }

  // RELSEXT queries
  $rel_subqueries = array();
  foreach($relationships as $rel){
    $rel_subqueries[] = "RELS_EXT_{$rel}_uri_mt:{$object_id}";
  }

  $query_parts[] = '(' . implode(' OR ',$rel_subqueries) . ')';

  // Whole Query
  $query = implode(' AND ', $query_parts);

  // Execute Query
  $query_processor->buildAndExecuteQuery($query);

  $solr_results = $query_processor->islandoraSolrResult;

  // Generate list of PIDs
  $results = array();
  foreach($solr_results['response']['objects'] as $object_id){
    $results[] = islandora_object_load($object_id['PID']);
  }

  return $results;
}


/**
 * The following function does basically the same thing as the one above it,
 * except it does it using SPARQL instead of Solr. Probably not going to use it.
 * @param null $id
 * @param null $cmodels
 * @param null $relationships
 * @param bool $subject
 *
 * @return array
function i($id=null, $cmodels=null,  $relationships=null, $subject=FALSE) {

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
      $pre[$namespace] = $prefix++ . ':';
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
 */