<?php
/**
 * @file
 */

/**
 * @param $form
 * @param $form_state
 * @return mixed
 */
function islandora_bioinformatics_form_islandora_related_islandora_specimen_cmodel_objects_form_alter(&$form, &$form_state) {
  module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');

  $specimens = $form_state['islandora']['related_objects_ids'];

 // Headers
  $header = &$form['content_table_select']['table']['#header'];

  $header = array(
    'label' => 'Label',
    'genus_species' => 'Genus <em>species</em>',
    'cmodel' => 'Model',
  );
  // Merge with the original headers.
//  $header = array_merge($header, $form['content_table_select']['table']['#header']) ;

  // Unset a couple we don't want.
  unset($header['id']);

  $options = &$form['content_table_select']['table']['#options'];

  foreach ($options as $specimen_id => &$rows) {
    $specimen = islandora_object_load($specimen_id);
    $eml = specimen_EML_to_array($specimen);

    $specimen_label = $eml['shortName'] ? $eml['shortName'] :
                     ($eml['title']     ? $eml['title']     : $specimen_id);
    $specimen_url = "<a href='/islandora/object/{$specimen_id}'>{$specimen_label}</a>";
    $genus_species = t($eml['taxonomicCoverage']['genus'] .
                     ' <em>'.strtolower($eml['taxonomicCoverage']['species']).'</em>');

    $rows['label'] = $specimen_url;
    $rows['genus_species'] = $genus_species;
    $rows['#attributes'] = array("style" => "max-height: 100px;");

    $parent = islandora_object_parent_model($specimen_id);
    $c = islandora_cmodel_load($parent);
    $rows['cmodel'] = $c ? $c->label : 'NOT FOUND: '.$parent;
  }

  $form['content_table_select']['table']['#type'] =  'tableselect';
  $form['content_table_select']['table']['#header'] =  $header;
  if($form_state['islandora']['current_form'] == 'View')
    $form['content_table_select']['table']['#empty'] =  t("There are no related specimens.");
  elseif ($form_state['islandora']['current_form'] == 'Add') {
    $form['content_table_select']['table']['#empty'] =  t("The search found no specimen.");
  }

  return $form;
}


/**
 * Implements hook_form_FORM_ID_alter()
 *
 * WHERE  FORM_ID         = "islandora_related_RELATED_CMODEL_objects_form"
 * AND    RELATED_CMODEL  = "islandora:person_cmodel"
 */
function islandora_bioinformatics_form_islandora_related_islandora_person_cmodel_objects_form_alter(&$form, &$form_state) {
  module_load_include('inc', 'islandora_bioinformatics', 'includes/mads_to_array');

  unset($form['Ingest']);

  $object = !empty($form_state['islandora']['object']) ? $form_state['islandora']['object'] : null;
  $people = !empty($form_state['islandora']['related_objects_ids']) ? $form_state['islandora']['related_objects_ids'] : array();

  $form['content_table_select']['table']['#header'] = array(
    'name' => 'Name',
    'phone' => 'Phone',
    'email' => 'Email',
  );

  $people_entries = array();

  foreach ($people as $person_id) {
    $person = islandora_object_load($person_id);
    // Default Values
    if (!isset($person['MADS'])) break;

    module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');
    $mads = person_mads_to_array($person);

    $name = $mads['authority']['given'] . ' ' . $mads['authority']['family'];
    $label = $mads['title'] ? $mads['title']: ($name ? $name : ($mads['id']? $mads['id']: $person->id));

    $people_entries[$person->id] = array(
      'name' => l($label, "islandora/object/{$person->id}"),
      'phone' => $mads['affiliation']['phone'],
      'email' => $mads['affiliation']['email'],
    );
  }

  $form['content_table_select']['table']['#options'] = $people_entries;
  $form['content_table_select']['table']['#empty'] = t('There is no people related to ' . $object->label . '.');

  return $form;
}


function islandora_bioinformatics_form_islandora_related_islandora_genesequence_cmodel_objects_form_alter(&$form, &$form_state) {
  unset($form['Add']);
  unset($form['View']);
  $sequences= !empty($form_state['islandora']['related_objects_ids']) ? $form_state['islandora']['related_objects_ids'] : array();
  
  $form['content_table_select']['table']['#header'] = array(
    'label' => 'Sequence',
    'download' => '',
  );

  $seq_entries = array();
  foreach($sequences as $seq_id){
    $seq = islandora_object_load($seq_id);
    $label = $seq->label ? $seq->label: $seq_id;
    $seq_entries[$seq->id] = array(
      'label' => l($label, "islandora/object/{$seq->id}"),
      'download' => l('Download', "islandora/object/{$seq->id}/datastream/FASTA/download"),
    );
  }

  
  $empty_message = ($form_state['islandora']['current_form'] == 'View') ?
                   "There are no Genesequences associated with this object." :
                   "No Geneequences found.";

  $form['content_table_select']['table']['#options'] = $seq_entries;
  $form['content_table_select']['table']['#empty'] = $empty_message;

}


function islandora_bioinformatics_form_islandora_related_islandora_assay_run_cmodel_objects_form_alter(&$form, &$form_state) {
  unset($form['View']);
  unset($form['Ingest']);
  unset($form['Add']);
  $object = !empty($form_state['islandora']['object']) ? $form_state['islandora']['object'] : null;
  $assay_runs = !empty($form_state['islandora']['related_objects_ids']) ? $form_state['islandora']['related_objects_ids'] : array();

  // Get the Assays vocabulary and all it's terms.
  $vnames = taxonomy_vocabulary_get_names();
  $vocab = $vnames['assays'];
  $vid = $vocab->vid;
  $terms = taxonomy_get_tree($vid);

  $content_headers = array('label' => 'Label');
  $assays = array();

  foreach ($terms as $term){
    // Short and Long Names
    $short_name = $term->name;
    $long_name = $term->description;

    // Associative Array of Assays
    $assays[$short_name] = $long_name;

    // Table Header
    $content_headers[$short_name] = $short_name;

    // Default value for all cells. Using this structure so we can apply classes.
    $default_cells[$short_name] = array('data' => 'N');
  }

  module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');

  $assay_run_entries = array();
  foreach($assay_runs as $assay_run_id){
    $assay_run = islandora_object_load($assay_run_id);
    $label = $assay_run->label ? $assay_run->label: $assay_run;

    $assay_run_entries[$assay_run->id]= array_merge($default_cells,
      array('label' => l($label, "islandora/object/{$assay_run->id}")));

    $xml = fraction_to_array($assay_run_id);
    foreach($xml['assays'] as $assay){
      $short_name = $assay['name'];

      switch($assay['result']){
        case('inactive'): $result = 'I'; $bgcolor = ''; break;
        case('low'):      $result = 'L'; $bgcolor = 'grey'; break;
        case('medium'):   $result = 'M'; $bgcolor = 'orange'; break;
        case('strong'):   $result = 'S'; $bgcolor = 'yellow'; break;
        case('hit'):      $result = 'H'; $bgcolor = 'red'; break;
        default:          $result = '-'; $bgcolor = ''; break;
      }

      $assay_run_entries[$assay_run_id][$short_name] = array('data' => $result, 'bgcolor'=>$bgcolor);
    }
  }

  $empty_message = ($form_state['islandora']['current_form'] == 'View') ?
                   "There are no Fractions associated with this object." :
                   "No Fractions found.";

//  unset($form['content_table_select']['table']['#type']);
//  $form['content_table_select']['table']['#theme'] = 'table';
  $form['content_table_select']['table']['#header'] = $content_headers;
  $form['content_table_select']['table']['#options'] = $assay_run_entries;
  $form['content_table_select']['table']['#empty'] = $empty_message;



  // Get a list of protocols and put them in this variable.
  $solr = new IslandoraSolrQueryProcessor();

  $protocol = str_replace('cmodel', 'protocol_cmodel', $form_state['islandora']['cmodel']->id);
  $solr->buildAndExecuteQuery('RELS_EXT_hasModel_uri_ms:"info:fedora/' . $protocol .'"');



  foreach($solr->islandoraSolrResult['response']['objects'] as $sr){
    $protocols[$sr['PID']] = $sr['object_label'];
  }

  if (empty($protocols)){
    $protocols[] = "None";
    $form['content_table_select']['actions']['selected']['#disabled'] = TRUE;
    $form['content_table_select']['actions']['submit']['#disabled'] = TRUE;
  }

  $form['content_table_select']['actions']['selected']['#options'] = $protocols;
  $form['content_table_select']['actions']['selected']['#title'] = 'Protocols';
  $form['content_table_select']['actions']['submit']['#value'] = 'Run Protocol';
  $form['content_table_select']['actions']['submit']['#submit'][] = 'run_protocol';


}


function run_protocol(&$form, &$form_state){
  $protocol =  islandora_object_load($form_state['values']['selected']);
  if ($protocol){
    $object = $form_state['islandora']['object'];

    module_load_include('inc', 'islandora_bioinformatics', 'includes/xml_to_array');
    $arp = assay_run_protocol_to_array($protocol);

    $tuque = new IslandoraTuque();
    $cmodel = $form_state['islandora']['cmodel'];

    foreach($arp['methods'] as $step){
        $fraction = $tuque->repository->constructObject();
        $fraction->label = $step['title'];
        $fraction->models = array($cmodel);
        $ds = $fraction->constructDatastream('fraction');
        $ds->setContentFromString('<?xml version="1.0"?>
                                    <fraction:assay xmlns:fraction="http://www.upeikerrlab.ca" xmlns="http://www.upeikerrlab.ca">
                                    <fraction:type>fraction</fraction:type>
                                    <fraction:id>' . $fraction->label . '</fraction:id>
                                    <fraction:labid>???</fraction:labid>
                                    <fraction:weight>???</fraction:weight>
                                    <fraction:specimen>info:fedora/'. $object->id .'</fraction:specimen>
                                    <fraction:protocol>' . $protocol .'</fraction:protocol>
                                    <fraction:notes>NOT FILLED OUT</fraction:notes>
                                    <fraction:plate/>
                                    <fraction:location/>
                                    <fraction:notes/>
                                    </fraction:assay>');
        $fraction->ingestDatastream($ds);
        $tuque->repository->ingestObject($fraction);
        $fraction = islandora_object_load($fraction->id);
        $fraction->relationships->add(FEDORA_RELS_EXT_URI, 'isMetadataFor', $object);
    }
  }
}
