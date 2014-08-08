<?php
/**
 * Created by PhpStorm.
 * User: nolan
 * Date: 2014-08-05
 * Time: 10:35 AM
 */

function islandora_bioinformatics_drush_command(){
  $items = array();

  $items['fix-critters'] = array(
    'description' => 'Ingests an existing FOXML 1.1 file into the repository.',
    'arguments' => array(
      'file_path' => 'The path to the FOXML 1.1 file.',
    ),
    'options' => array(),
  );
  return $items;
}

function islandora_bioinformatics_drush_help($section){
  switch ($section) {
    case 'drush:islandora-ingest-foxml':
       return t('This is a command.');
  };
}

function drush_islandora_bioinformatics_COMMAND($file_path){
  print "Hello";
  $query = "
  PREFIX fedora: ".FEDORA_RELS_EXT_URI. "
  PREFIX fedora-model: ".FEDORA_MODEL_URI."
  PREFIX islandora: ".ISLANDORA_RELS_EXT_URI."
  PREFIX islandora-model: ".ISLANDORA_MODEL_URI. "
  SELECT ?pid FROM <#ri> WHERE {


  }
  ";

}
