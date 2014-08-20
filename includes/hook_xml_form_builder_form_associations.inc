<?php
/**
 * @file
 */
/**
 * Implements hook_islandora_xml_form_builder_form_associations()
 */
function islandora_bioinformatics_islandora_xml_form_builder_form_associations() {
  $form_associations = array(
    // Person MADS
    'person_mads' => array(
      'content_model' => 'islandora:person_cmodel',
      'form_name' => 'Person MADS Form',
      'dsid' => 'MADS',
      'title_field' => array('authority',  'titleInfo',  'title'),
      'transform' => 'No Transform',
      'template' => '',
    ),

    // Project MADS
    'project_mads' => array(
      'content_model' => 'islandora:project_cmodel',
      'form_name' => 'Project MADS Form',
      'dsid' => 'MADS',
      'title_field' => array('authority',  'titleInfo',  'title'),
      'transform' => 'No Transform',
      'template' => '',
    ),

    // Associate each type of specimen to the generic Specimen EML Form.
    'specimen_eml' => array(
      'content_model' => "islandora:specimen_cmodel",
      'form_name' => "Specimen EML Form",
      'dsid' => 'EML',
      'title_field' => array('dataset', 'title'),
      'transform' => '',
      'template' => FALSE,
    ),

    /** @todo Somehow inherit parent forms so these associations wouldn't be necessary. */

    'invertebrate_specimen_eml' => array(
      'content_model' => "islandora:invertebrate_cmodel",
      'form_name' => "Specimen EML Form",
      'dsid' => 'EML',
      'title_field' => array('dataset', 'title'),
      'transform' => '',
      'template' => FALSE,
    ),
    'microbe_specimen_eml' => array(
      'content_model' => "islandora:microbe_cmodel",
      'form_name' => "Specimen EML Form",
      'dsid' => 'EML',
      'title_field' => array('dataset', 'title'),
      'transform' => '',
      'template' => FALSE,
    ),

    // Fraction and Compound
    'fraction_form' => array(
      'content_model' => "islandora:fraction_run_cmodel",
      'form_name' => 'Assay Run Form',
      'dsid' => 'fraction',
      'transform' => '',
      'template' => FALSE,
      'title_field' => array('id'),
    ),
    'compound_form' => array(
      'content_model' => "islandora:compound_run_cmodel",
      'form_name' => 'Assay Run Form',
      'dsid' => 'fraction',
      'transform' => '',
      'template' => FALSE,
      'title_field' => array('id'),
    ),

    // LCMS
    'lcms_form' => array(
      'content_model' => 'islandora:lcms_cm',
      'form_name' => 'LCMS Form',
      'dsid' => 'LCMS',
      'title_field' => array('run_title'),
      'transform' => '',
      'template' => FALSE,
    ),
  );

  return $form_associations;
}