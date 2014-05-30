<?php

function islandora_bioinformatics_user_insert(&$edit, $account, $category) {
  /**
   * @author Quintin Dawson
   * @param &$edit
   * @param $account
   * @param $category
   *
   * Implements hook_user_insert()
   *
   * When a new drupal user is created, this function will create a corresponding Person object in Fedora.
   */

  $tuque = new IslandoraTuque();

  //build person object

  //////You're overwriting the Person Content Model, not making a new object of type content model.

  $person_object = $tuque->repository->constructObject('islandora');
  $person_object->owner = 'fedoraAdmin';
  $person_object->label = $account->name;
  $person_object->models = 'islandora:person_cmodel';

  //build MADS xml from user information here
  $xml = '<?xml version="1.0"?>
            <mads:mads xmlns:mads="http://www.loc.gov/mads/v2" xmlns="http://www.loc.gov/mads/v2" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink">
              <mads:authority>
                <mads:name type="personal">
                  <mads:namePart type="given"></mads:namePart>
                  <mads:namePart type="family"></mads:namePart>
                  <mads:namePart type="date"/>
                </mads:name>
                <mads:titleInfo>
                  <mads:title>' . $account->name . ' </mads:title>
                </mads:titleInfo>
              </mads:authority>
              <mads:identifier type="u1"></mads:identifier>
              <mads:affiliation>
                <mads:phone></mads:phone>
                <mads:email>' . $account->mail . '</mads:email>
                <mads:organization></mads:organization>
                <mads:position></mads:position>
                <mads:address></mads:address>
                <mads:dateValid point="start"/>
                <mads:dateValid point="end"/>
              </mads:affiliation>
              <mads:url></mads:url>
              <mads:note type="history"/>
              <mads:fieldOfActivity></mads:fieldOfActivity>
            </mads:mads>';

  //ingest the MADS datastream to the object
  $datastream = $person_object->constructDatastream('MADS', 'X');
  $person_object->ingestDatastream($datastream);

  //build MADS datastream
  $datastream->label = 'MADS Record';
  $datastream->mimetype = 'text/xml';
  $datastream->setContentFromString($xml);

  //related person object to appriopriate collection
  $person_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'islandora:person_collection');

  //store uid of Drupal account in person object
  //$person_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isDrupalID', $account->uid, RELS_TYPE_STRING);
  $person_object->relationships->add(RELS_INT_NS, 'isDrupalId', $account->uid, RELS_TYPE_INT);


  //ingest object into fedora
  $tuque->repository->ingestObject($person_object);
  //$ingested_object = islandora_add_object($person_object);
}

function islandora_bioinformatics_user_update(&$edit, $account, $category) {
  /**
   * @author Quintin Dawson
   * @param &$edit
   * @param $account
   * @param $category
   *
   * Implements hook_user_update()
   *
   * When a Drupal user is modified, update the associated person object with the new values
   */
  //retrieve Person object related to the Drupal user
  $tuque = new IslandoraTuque();
  //this query should retrieve all person_cmodel objects
  $query = "SELECT ?pid
          FROM <#ri>
          WHERE {
          ?pid <fedora-model:hasModel> <info:fedora/islandora:person_cmodel> .
        }";

  $result = $tuque->repository->ri->sparqlQuery($query, 'unlimited');

  $account_person = NULL;

  //iterate over all people objects to find the one related to this drupal account
  foreach ($result as $person) {
    //load the current person object
    $person_object = islandora_object_load($person['pid']['value']);

    //check if relationship to Drupal user exists
    $relation = $person_object->relationships->get(RELS_INT_NS, 'isDrupalId', $account->uid, RELS_TYPE_INT);
    if (!empty($relation)) {
      $account_person = $person_object;
    }
  }

  /* //This query should retrieve all people objects related to the current drupal user [$account->id]
  $query = "SELECT ?uid
            FROM <#ri>
            WHERE {
              ?uid <" . ISLANDORA_RELS_EXT_URI . ":isDrupalID> <info:fedora/" . $account->id . "> .
            }";
  $result - $tuque->repository->ri->sparqlQuery($query, 'unlimited');

  //only one Fedora object should be related to the Drupal account, so take the first value in the $results array
  if (!empty($result[0])) {
    $account_person = result[0];
  }
  */
  //if the Fedora object related to the Drupal account is found, make the desired changes

  if (empty($account_person)) { //create the person object if missing when modified
    islandora_bioinformatics_user_insert($edit, $account, $category);
  }
  else {  //modify the person object if existing
    //change the label
    $account_person->label = $account->name;

    //Modify the xml
    $person_xml = new SimpleXMLElement($person_object["MADS"]->content);
    $ns = "http://www.loc.gov/mads/v2";
    foreach ($person_xml->children($ns) as $part) {
      switch ((string) $part->getName()) {
        case 'authority':
          $part->titleInfo->title = $account->name;
          break;
        case 'identifier':
          //$part = $account->name;   //NOT WORKING AT THE MOMENT
          break;
        case 'affiliation':
          $part->email = $account->mail;
          break;
      }
    }

    //Replace the old xml with the modified xml in Fedora
    $account_person['MADS']->setContentFromString($person_xml->asXML());
    $account_person['MADS']->label = 'MADS Record';
    $account_person['MADS']->mimetype = 'text/xml';
  }
}

function islandora_bioinformatics_user_cancel($edit, $account, $method) {
  /**
   * @author Quintin Dawson qdawson@upei.ca
   * @param $edit
   * @param $account
   * @param $method
   *
   * Implements hook_user_cancel()
   *
   * Called when a drupal user is deleted. This should find and delete the associated Fedora person object from the repository
   */


  //retrieve Person object related to the Drupal user
  $tuque = new IslandoraTuque();
  //this query should retrieve all person_cmodel objects
  $query = "SELECT ?pid
          FROM <#ri>
          WHERE {
          ?pid <fedora-model:hasModel> <info:fedora/islandora:person_cmodel> .
        }";

  $result = $tuque->repository->ri->sparqlQuery($query, 'unlimited');

  $account_person = NULL;

  //iterate over all people objects to find the one related to this drupal account
  foreach ($result as $person) {
    //load the current person object
    $person_object = islandora_object_load($person['pid']['value']);

    //check if relationship to Drupal user exists
    $relation = $person_object->relationships->get(RELS_INT_NS, 'isDrupalId', $account->uid, RELS_TYPE_INT);
    if (!empty($relation)) {
      $account_person = $person_object;
    }
  }

  if (!empty($account_person)) {
    switch ($method) {
      case 'user_cancel_block':
        //do nothing
        break;
      case 'user_cancel_block_unpublish': //account is blocked and information is still accessible to administrators
        //mark object as deleted in Fedora
        $account_person->delete();  //flag object as deleted
        break;
      case 'user_cancel_reassign':  //account is deleted and information its contents belong to an anonymous user
        //purge object from fedora
        $account_person->delete();  //flag object as deleted
        $tuque->repository->purgeObject($account_person->id); //purge the object from the database
        break;
      /*
      case 'user_cancel_delete':   //Account is removed and all information is deleted
        //purge object from fedora
        $account_person->delete();  //flag object as deleted
        $tuque->repository->purgeObject($account_person->id); //purge the object from the database
        break;
      */
    }
  }
}

function islandora_bioinformatics_user_delete($account) {
  /**
   * @author Quintin Dawson qdawson@upei.ca
   * @param $edit
   * @param $account
   * @param $method
   *
   * Implements hook_user_delete()
   *
   * Called when a drupal user is deleted. This should find and delete the associated Fedora person object from the repository
   */


  //retrieve Person object related to the Drupal user
  $tuque = new IslandoraTuque();
  //this query should retrieve all person_cmodel objects
  $query = "SELECT ?pid
          FROM <#ri>
          WHERE {
          ?pid <fedora-model:hasModel> <info:fedora/islandora:person_cmodel> .
        }";

  $result = $tuque->repository->ri->sparqlQuery($query, 'unlimited');

  $account_person = NULL;

  //iterate over all people objects to find the one related to this drupal account
  foreach ($result as $person) {
    //load the current person object
    $person_object = islandora_object_load($person['pid']['value']);

    //check if relationship to Drupal user exists
    $relation = $person_object->relationships->get(RELS_INT_NS, 'isDrupalId', $account->uid, RELS_TYPE_INT);
    if (!empty($relation)) {
      $account_person = $person_object;
    }
  }

  if (!empty($account_person)) {
    //purge object from fedora
    $account_person->delete();  //flag object as deleted
    $tuque->repository->purgeObject($account_person->id); //purge the object from the database
  }
}