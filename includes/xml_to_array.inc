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


function person_mads_to_array($person_object) {
  $ns = "http://www.loc.gov/mads/v2";
  $xml = (new SimpleXMLElement($person_object["MADS"]->content))->children($ns);

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

function project_mads_to_array($project_object) {
  /**
   * @author Brent Mitton
   * @param $project_object
   *  An islandora object for the project
   * @return array
   *  an array of information needed pertaining to the projcet
   *
   * Takes a project object and returns a simple array of the data in the MADS stream.
   * I'm not sure what data we need from this right now.
   */
  $ns = "http://www.loc.gov/mads/v2";

  $project_xml = new SimpleXMLElement($project_object["MADS"]->content);

  $project_mads = array();

  foreach ($project_xml->children($ns) as $part) {

    switch ((string) $part->getName()) {
      case "authority":
        $project_mads["title"] = ((string)$part->titleInfo->title) ? (string)$part->titleInfo->title:"";
        break;
      case "affiliation":
        $project_mads["organization"] = ((string)$part->organization) ? (string)$part->organization:"";

        $project_mads["address"]["city"] = ((string)$part->address->city) ? (string)$part->address->city:"";
        $project_mads["address"]["street"] = ((string)$part->address->street) ? (string)$part->address->street:"";
        $project_mads["address"]["country"] = ((string)$part->address->country) ? (string)$part->address->country:"";
        $project_mads["address"]["postcode"] = ((string)$part->address->postcode) ? (string)$part->address->postcode:"";

        $project_mads["email"] = ((string)$part->email) ? (string)$part->email:"";
        $project_mads["phone"] = ((string)$part->phone) ? (string)$part->phone:"";

        $project_mads["startdate"] = ((string) $part->dateValid[0]) ? (string)$part->dateValid[0]:"";
        $project_mads["enddate"] = ((string) $part->dateValid[1]) ? (string)$part->dateValid[1]:"";

        break;
      case "note":
        // add these in when needed
        break;
      case "fieldOfActivity":
        break;
    };
  }
  return $project_mads;
}