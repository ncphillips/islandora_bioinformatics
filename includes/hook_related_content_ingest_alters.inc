<?php
/**
 * @file
 */

function islandora_bioinformatics_islandora_related_islandora_specimen_cmodel_content_ingest_alter(&$data){
  $data['collection'] = 'islandora:specimen_collection';
}

function islandora_bioinformatics_islandora_related_islandora_genesequence_cmodel_content_ingest_alter(&$data){
  $data['collection'] = 'islandora:genesequence_collection';
}
