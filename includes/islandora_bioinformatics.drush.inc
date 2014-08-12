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
  $query = 'RELS_EXT_hasModel_uri_ms:"info:fedora/islandora:specimenCModel" OR RELS_EXT_hasModel_uri_ms:"islandora:specimenCModel"';
  $qp->buildAndExecuteQuery($query);

  $sr = $qp->islandoraSolrResult;

  $i = 0;

  drush_print("Hello");
}

