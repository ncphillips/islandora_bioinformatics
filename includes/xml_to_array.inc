<?php
/**
 * @file
 */

/**
 * @param $specimen_object
 * @return array
 *
 * Returns an array containing information gathered from a Specimen objects EML datastream.
 */
function specimen_EML_to_array($specimen_object) {
  $ns = "eml://ecoinformatics.org/eml-2.1.0";

  $eml = array();

  $xml = new SimpleXMLElement($specimen_object["EML"]->content);
  $attributes = is_object($xml->attributes()) ? $xml->attributes() : false;

  $xml = is_object($xml->dataset) ? $xml->dataset : false;

  $eml['abstract'] = $xml ? (string)$xml->abstract->para : '';
  $eml['packageId'] = $attributes ? (string)$attributes['packageId'] : '';
  $eml['shortName'] = (string)$xml->shortName;
  $eml['system'] = (string)$attributes['system'];
  $eml['title'] = (string)$xml->title;


  // Specimen Keywords
  $eml['keywords'] = array();
  $keywordSet = is_object($xml->keywordSet) ? $xml->keywordSet->children() : array();
  foreach($keywordSet as $keyword){
    if((string)$keyword)
      $eml['keywords'][] = (string) $keyword;
  }

  // Taxonomic Coverage
  $eml['taxonomicCoverage'] = array(
    'phylum' => '',
    'subphylum' => '',
    'class' => '',
    'order' => '',
    'family' => '',
    'genus' => '',
    'species' => '',
  );
  $eml['geographicCoverage'] = array(
    'description' => '',
    'coordinates' => array(
      'east' => '',
      'west' => '',
      'north' => '',
      'south' => '',
    ),
    'altitudes' => array(
      'max' => '',
      'min' => '',
      'units' => '',
    )
  );

//  var_dump((is_object($xml->coverage) ? ($coverage = $xml->coverage) : false));

  if (is_object($xml->coverage) ? ($coverage = $xml->coverage) : false){

    eml_taxonomicCoverage_to_array($coverage->taxonomicCoverage->taxonomicClassification, $eml);

    // Geographic Coverage
    if (is_object($coverage->geographicCoverage))
      eml_geographicCoverage_to_array($coverage->geographicCoverage, $eml);

    /** @todo Temporal Coverage */
  }

  return $eml;
}

/**
 * @param SimpleXMLElement $xml
 * The SimpleXMLElement containing the taxonomicClassification tags.
 *
 * @param array $array
 * The array in which to put the results.
 */
function eml_taxonomicCoverage_to_array(SimpleXMLElement $classifications, array &$array){
  foreach($classifications as $classification){

    $taxonRankName = (string)$classification->taxonRankName;
    switch($taxonRankName){
      case 'Phylum':
        break;
      case 'Subphylum':
        break;
      case 'Class':
        break;
      case 'Order':
        break;
      case 'Family':
        break;
      case 'Genus':
        $array['taxonomicCoverage']['genus'] = (string)$classification->taxonRankValue;
        break;
      case 'Species':
        $array['taxonomicCoverage']['species'] = (string)$classification->taxonRankValue;
        break;
    }
  }

}


function eml_geographicCoverage_to_array(SimpleXMLElement $coverage, array &$array){
  $coord = $coverage->boundingCoordinates;
  $alt = $coord->boundingAltitudes;
  $array['geographicCoverage'] = array(
    'description' => (string)$coverage->geographicDescription,
    'coordinates' => array(
      'east' => (string)$coord->eastBoundingCoordinate,
      'west' => (string)$coord->westBoundingCoordinate,
      'north' => (string)$coord->northBoundingCoordinate,
      'south' => (string)$coord->southBoundingCoordinate,
    ),
    'altitudes' => array(
      'max' => $alt->altitudeMaximum,
      'min' => $alt->altitudeMinimum,
      'units' => $alt->altitudeUnits,
    )
  );
}

/**
 * Takes in an fraction object and returns an array.
 * @param $fraction_object
 *  An islandora fraction object
 * @return array
 *  A simple array containing the fraction information
 */
function fraction_object_to_array($fraction_object) {

  $assays = array();
  // query the assays and store them
  $result = db_select('labobject_assay_assay', 'a')
    ->fields('a', array('abbreviation', 'name'))
    ->execute();

  foreach ($result as $row) {
    $assays[$row->abbreviation] = $row->name;
  }


  $ns = "http://www.upeikerrlab.ca";
  $fraction_array = array();

  $fraction_xml = new SimpleXMLElement($fraction_object["fraction"]->content);

  $i = 0;
  foreach ($fraction_xml->children($ns) as $part) {

    switch ((string) $part->getName()) {
      case "type":
        $fraction_array["type"] = ((string)$part) ? (string)$part:"";
        break;
      case "labid":
        $fraction_array["labid"] = ((string)$part) ? (string)$part:"";
        break;
      case "weight":
        $fraction_array["weight"] = ((string)$part) ? (string)$part:"";
        break;
      case "plate":
        $fraction_array["plate"] = ((string)$part) ? (string)$part:"";
        break;
      case "location":
        $fraction_array["location"] = ((string)$part) ? (string)$part:"";
        break;
      case "notes":
        $fraction_array["notes"] = ((string)$part) ? (string)$part:"";
        break;
      case "inhibitor":

        foreach ($assays as $key => $value) {
          if ((string) $part->name == $value) {
            $fraction_array["fraction"][$key]["name"] = $value;
            $fraction_array["fraction"][$key]["result"] = ((string)$part->result) ? (string)$part->result:"";
            $fraction_array["fraction"][$key]["comment"] = ((string)$part->comment) ? (string)$part->comment:"";
          }
        }
        break;
    }
  }

  return $fraction_array;
}


/**
 * @param $person
 * @return array
 *
 * This function creates an array with information from the MADS datastream from
 * a Person object.
 */
function person_mads_to_array($person) {
  $ns = "http://www.loc.gov/mads/v2";
  $xml = (new SimpleXMLElement($person["MADS"]->content))->children($ns);

  $name = $xml->authority->name;
  $affiliation = $xml->affiliation;

  $mads = array(
    'authority' => array(
      'given' => is_object($name) ? (string)$name->namePart[0]: '',
      'family' => is_object($name)? (string)$name->namePart[1]: '',
      'birth_date' => is_object($name) ? (string)$name->namePart[2]: '',
    ),
    'title' => is_object($xml->authority->titleInfo) ? (string)$xml->authority->titleInfo->title : '',
    'id' => (string)$xml->identifier,
    'affiliation' => array(
      'phone' => is_object($affiliation) ? (string)$affiliation->phone : '',
      'email' => is_object($affiliation) ? (string)$affiliation->email : '',
      'organization' => is_object($affiliation) ? (string)$affiliation->organization : '',
      'position' => is_object($affiliation) ? (string)$affiliation->position : '',
      'address' => is_object($affiliation) ? (string)$affiliation->address : '',
      'start_date' => is_object($affiliation) ? (string)$affiliation->dateValid[0] : '',
      'end_date' => is_object($affiliation) ? (string)$affiliation->dateValid[1] : '',
    ),
    'url' => (string)$xml->url,
    'note' => array(),
    'fieldOfActivity' => array(),
  );

  return $mads;
}

/**
 * @param $project
 * @return array
 *
 * This function creates an array with information from the MADS datastream from
 * a Project object.
 */
function project_mads_to_array($project) {
  $ns = "http://www.loc.gov/mads/v2";

  if (!is_object($project))
    $project = islandora_object_load($project);

  $xml = (new SimpleXMLElement($project['MADS']->content))->children($ns);

  // Here is the full array with default values.
  $mads = array(
    'authority' => array(
      'title_info' => array(
        'title' => '',
      ),
      'identifier' => '',
    ),
    'affiliation' => array(
      'organization' => '',
      'address' => array(
        'city' => '',
        'street' => '',
        'country' => '',
        'postcode' => '',
      ),
      'email' => '',
      'phone' => '',
      'date_valid' => array(
        'start_date' => '',
        'end_date' => '',
      ),
      'url' => ''
    ),
    'note' => array(
      'status' => '',
    ),
    'field_of_activity' => '',
  );

  if(is_object($xml)){
    // Title
    $authority = $xml->authority;
    if (is_object($authority)){
      $mads['authority']['title_info']['title'] = (string)$authority->titleInfo->title;
      $mads['authority']['identifier'] = (string)$authority->identifier;
    }

    $affiliation = $xml->affiliation;
    if (is_object($affiliation)){

      // Contact Info
      $mads['affiliation']['organization'] = (string)$affiliation->organization;
      $mads['affiliation']['email'] = (string)$affiliation->email;
      $mads['affiliation']['phone'] = (string)$affiliation->phone;
      $mads['affiliation']['url'] = (string)$affiliation->url;

      // Address
      $address = $affiliation->address;
      if (is_object($address)){
        $mads['affiliation']['address'] = array(
          'city' => (string)$address->city,
          'street' => (string)$address->street,
          'country' => (string)$address->country,
          'postcode' => (string)$address->postcode,
        );
      }

      // Date Valid
      $date = $affiliation->dateValid;
      if (is_object($date)){
        $mads['affiliation']['date_valid']['start_date'] = (string)$date[0];
        $mads['affiliation']['date_valid']['end_date'] = (string)$date[1];
      }
    }
  }

  return $mads;
}


function fraction_to_array($fraction_id){
  $fraction = islandora_object_load($fraction_id);

  $ns = "http://www.upeikerrlab.ca";
  $ds = empty($fraction['FRACTION']) ? $fraction['fraction'] : $fraction['FRACTION'];

  $obj = new SimpleXMLElement($ds->content);
  $xml_objects = $obj->children($ns);

  $array = array();
  $array['id'] = $xml_objects->id;
  $array['lab_id'] = $xml_objects->labid;
  $array['weight'] = $xml_objects->weight;
  $array['plate'] = $xml_objects->plate;
  $array['location'] = $xml_objects->location;
  $array['notes'] = $xml_objects->notes;

  $array['assays'] = array();
  $i = 0;
  foreach ($xml_objects->inhibitor as $assay) {
    $array['assays'][$i]['name'] = (string) $assay->name;
    $array['assays'][$i]['result'] = (string) $assay->result;
    $array['assays'][$i]['comment'] = $assay->comment;
    $i++;
  }

  return $array;
}


function critter_to_array($specimen_id){
  $specimen = islandora_object_load($specimen_id);

  $ns = "http://vre.upei.ca/critters/";
  $ds = $specimen['CRITTER'];

  $obj = new SimpleXMLElement($ds->content);
  $xml_objects = $obj->children($ns);
  $array = array(
    'type' => $xml_objects->type,
  );
  return $array;
}


function assay_run_protocol_to_array($arp){
  if(!is_object($arp)){
    $arp = islandora_object_load($arp);
  }

  $ns = "eml://ecoinformatics.org/eml-2.1.1";

  $eml = array(
    'title' => '',
    'creator' => '',
    'contact' => '',
    'methods' => array(),
    'project' => array(),
  );
  $xml = new SimpleXMLElement($arp['PROTOCOL']->content);
  $xml = $xml->children($ns);


  if(is_object($xml)){
    $xml = $xml->dataset;
    $eml['title'] = (string)$xml->title;

    if (is_object($xml->creator)){
      $eml['creator'] = (string)$xml->creator->organizationName;
    }

    if (is_object($xml->contact)){
      $eml['creator'] = (string)$xml->creator->organizationName;
    }

    if (is_object($xml->methods)){
      foreach($xml->methods->methodStep as $method_step){
        $eml['methods'][] = array(
          'title' => (string)$method_step->description->section->title,
          'description' => (string)$method_step->description->section->para,
        );
      }
    }
  }
  return $eml;
}
