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
  $eml['taxonomicCoverage']['genus'] = '';
  $eml['taxonomicCoverage']['species'] = '';

  if (is_object($xml->coverage) ? ($coverage = $xml->coverage) : false){

    $tc = is_object($coverage->taxonomicCoverage) ? $coverage->taxonomicCoverage->taxonomicClassification : array();

    foreach($tc as $classification){
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
    $gc = is_object($coverage->geographicCoverage) ? $coverage->geographicCoverage : array();
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
  }
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

