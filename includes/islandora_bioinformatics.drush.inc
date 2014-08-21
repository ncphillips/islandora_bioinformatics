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

  $items['islandora_bioinformatics-fix-critter-models'] = array(
    'description' => "Fixes the CModel's for the MNPL Fedora Repository.",
    'aliases' => array('biofix-critter-models'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN, // we can pass in users id on the command line using drush -u.
  );

  return $items;
}

//drush hook
function drush_islandora_bioinformatics_fix_critter_models() {

  module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');
  $qs = new IslandoraSolrQueryset();
  $qs->fields = array(
    'PID' => array(
      'vre:mnpl-specimen*',
//      '#not' => array(
//        'RELS_EXT_hasParentModel_uri_ms' => array('*'),
//      ),
    ),
  );
  $a = $qs->objects;
  $num_pages = $qs->num_pages;


  for ($i = 0; $i < $num_pages; $i++){
    $qs->page = $i;

    foreach($qs->objects as $id){

      drush_print($id);
      $object = islandora_object_load($id);
      if (isset($object['CRITTER'])){
        $critter = critter_to_array($object);
        switch ($critter['type']){
          case('Tunicate'):
          case('Sponge'):
          case('Coral'):
          case('Algae'):
            $collection = 'islandora:' . strtolower($critter['type']) . '_collection';
            $cmodel = 'islandora:invertebrate_cmodel';
            break;
          default:
            $cmodel = 'islandora:specimen_cmodel';
            $collection= 'islandora:specimen_collection';
            break;
        }

        $types = array_merge(islandora_cmodel_types($cmodel), $object->models);
        $object->models = $types;

        // Creates a hasParentModel relationship towards the CModel initially chosen.
        $object->relationships->registerNamespace('islandora-model', ISLANDORA_MODEL_URI);
        if (!$object->relationships->get(ISLANDORA_MODEL_URI, 'hasParentModel', $cmodel)){
          $object->relationships->add(ISLANDORA_MODEL_URI, 'hasParentModel', $cmodel);
        }
        if (!$object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection)){
          $object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', $collection);
        }
      }

//      drush_print("$j: $object");
//      drush_print("\tCModels");
//      foreach($types as $tid){
//        drush_print("\t\t$tid");
//      }
//      drush_print("\tCollections");
    }
  }
}


