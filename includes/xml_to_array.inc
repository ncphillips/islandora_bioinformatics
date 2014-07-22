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
  $specimen_object = islandora_object_load('islandora:691');

  $xml = new SimpleXMLElement($specimen_object["EML"]->content);
  $attributes = $xml->attributes();


  $xml = $xml->dataset;

  $eml['abstract'] = (string)$xml->abstract->para;
  $eml['packageId'] = (string)$attributes['packageId'];
  $eml['shortName'] = (string)$xml->shortName;
  $eml['system'] = (string)$attributes['system'];
  $eml['title'] = (string)$xml->title;

  // Specimen Keywords
  $eml['keywords'] = array();
  foreach($xml->keywordSet->children() as $keyword){
    if((string)$keyword)
      $eml['keywords'][] = (string) $keyword;
  }

  // Taxonomic Coverage
  $eml['taxonomicCoverage']['genus'] = '';
  $eml['taxonomicCoverage']['species'] = '';
  $classifications = $xml->coverage->taxonomicCoverage;
  foreach($classifications->taxonomicClassification as $classification){
    $taxonRankName = (string)$classification->taxonRankName;
    switch($taxonRankName){
      case 'Species':
        $eml['taxonomicCoverage']['species'] = (string)$classification->taxonRankValue;
        break;
      case 'Genus':
        $eml['taxonomicCoverage']['genus'] = (string)$classification->taxonRankValue;
        break;
    }
  }

  // Geographic Coverage
  $gc = $xml->coverage->geographicCoverage;
  $coord = $gc->boundingCoordinates;
  $alt = $coord->boundingAltitudes;
  $eml['geographicCoverage'] = array(
    'description' => (string)$gc->geographicDescription,
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

  /** @todo Temporal Coverage */

  return $eml;
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

