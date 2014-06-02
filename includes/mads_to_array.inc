<?php
function person_mads_to_array($person_object) {
  /**
   * @author Brent Mitton
   * @param $person_object
   * @return An array with person information
   *
   * Takes in an islandora person object and returns a simple array of the MADS data
   */
  $ns = "http://www.loc.gov/mads/v2";

  $person_mads = array();

  $person_xml = new SimpleXMLElement($person_object["MADS"]->content);

  foreach ($person_xml->children($ns) as $part) {
    switch ((string) $part->getName()) {
      case "authority":
        // I'm just going to rely on position for now. Which is pretty awful
        $person_mads["name"]["first"] = ($part->name->namePart[0]) ? (string)$part->name->namePart[0] : "" ;
        $person_mads["name"]["last"] = ($part->name->namePart[1]) ? (string)$part->name->namePart[1] : "" ;
        $person_mads["name"]["birthdate"] = ($part->name->namePart[2]) ? (string)$part->name->namePart[2] : "" ;
        $person_mads["title"] = ($part->titleInfo->title) ? (string)$part->titleInfo->title : "";
        break;
      case "identifier":
        // I dont know if this is even working
        break;
      case "affiliation":
        if (!isset($person_mads['contact'])){

          $person_mads['contact']['phone'] = ($part->phone) ? (string)$part->phone : "";
          $person_mads['contact']['email'] = ($part->email) ? (string)$part->email : "";
          $person_mads['contact']['organization'] = ($part->organization) ? (string)$part->organization : "";
          $person_mads['contact']['position'] = ($part->position) ? (string)$part->position : "";
          $person_mads['contact']['address'] = ($part->address) ? (string)$part->address : "";
          $person_mads['contact']['startDate'] = ($part->dateValid[0]) ? (string)$part->dateValid[0] : "";
          $person_mads['contact']['endDate'] = ($part->dateValid[1]) ? (string)$part->dateValid[1] : "";

        }
        break;
      case "url":
        if (!isset($person_mads['contact']['url'])){

          $person_mads['contact']['url'] = ($part) ? (string)$part : "";
        }
        break;
      case "note":
        // I want to figure out how we want to deal with this before I do it
        break;
      case "fieldOfActivity":
        // What is this?
        break;
      default:
        throw new Exception("Unexpected XML Field.");
        break;
    }
  }
  return $person_mads;
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