<?php




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

/**
 *
 */

function islandora_object_related_islandora_objects($object=null, $cmodels=null,  $relationships=null) {
  $object = str_replace(':', '\:', $object);

  // CModel
  if (empty($cmodels)){
    $cmodels[] = '*';
  }

  $cmodel_subquery = array();
  foreach($cmodels as $cmodel){
    $cmodel = str_replace(':', '\:', $cmodel);
    $cmodel_subquery[] = "RELS_EXT_hasModel_uri_mt:$cmodel";
  }

  $cmodel_query = '(' . implode(' OR ', $cmodel_subquery) . ')';

  // Relationship
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

  $rel_subqueries = array();
  foreach($relationships as $rel){
    $rel_subqueries[] = "RELS_EXT_{$rel}_uri_mt:{$object}";
  }

  $rel_query = '(' . implode(' OR ',$rel_subqueries) . ')';

  // Whole Query
  $query = $cmodel_query . ' AND ' . $rel_query;


  $query_processor = new IslandoraSolrQueryProcessor();


  // This function accepts an IslandoraSolrQueryProcessor as a variable.
  islandora_xacml_api_islandora_solr_query($query_processor);

  $query_processor->buildAndExecuteQuery($query);

  $solr_results = $query_processor->islandoraSolrResult;

  echo '<pre>';

  $results = array();
  foreach($solr_results['response']['objects'] as $object){
    var_dump($object['PID']);
    $results[] = islandora_object_load($object['PID']);
  }


  echo '</pre>';
  return $results;
}

