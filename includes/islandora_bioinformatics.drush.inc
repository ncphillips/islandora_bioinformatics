<?php

/*
 *This file should be put in the islandora modules plugins directory
 * typical usage: drush -u 1 islandora_bioinformatics collection:pid isMemberOf TRUE
 * the above would give you an interactive purge
 * if the last parameter is not TRUE then all the pids in the list will be purged without prompting
 * purging an object cannot be undone so use wisely (you have been warned)
 */

//drush hook

function islandora_bioinformatics_drush_command() {
  $items = array();

  $items['islandora_bioinformatics-fix'] = array(
    'description' => "Fixes the CModel's for the MNPL Fedora Repository.",
    'aliases' => array('bio-fix'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN, // we can pass in users id on the command line using drush -u.
  );

  return $items;
}

//drush hook
function drush_islandora_bioinformatics_fix() {
  $qp = new IslandoraSolrQueryProcessor();
  $query = 'PID:vre\:mnpl-specimen-Key*';
  drush_print("Query: \"$query\"");
  $qp->buildAndExecuteQuery($query);

  $per_page = $qp->solrLimit;
  drush_print("Matches per Page: $per_page");

  $num_found = $qp->islandoraSolrResult['response']['numFound'];
  drush_print("Matches: $num_found");

  $num_pages = $num_found / $per_page;
  drush_print("Pages: $num_pages");


  for ($i = 0; $i < $num_pages; $i++){
    $objects =  $qp->islandoraSolrResult['response']['objects'];

    foreach($objects as $id){
      $id = [$i];
    }
    drush_print("$i: $id");
    $_GET['page'] = ++$i;
    $qp->buildAndExecuteQuery($query);
  }
}

