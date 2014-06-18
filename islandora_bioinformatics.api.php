<?php
/**
 * @file Islandora Bioinformatics Hook API
 *
 *       Lists the hooks used by this file.
 */

/**
 * hook_islandora_related_objects_page_alter
 *
 * @param $page
 * @param $page_state
 */
function hook_islandora_related_objects_page_alter(&$page, &$page_state){
}

/**
 * hook_islandora_OBJECT_CMODEL_objects_page_alter
 *
 * @param $page
 * @param $page_state
 *
 * Alters the related objects page
 * if the Object is of type OBJECT_CMODEL
 */
function hook_islandora_OBJECT_CMODEL_objects_page_alter(&$page, &$page_state){}

/**
 * @param $form
 * @param $form_state
 *
 * Alters the related objects form.
 */
function hook_form_islandora_related_objects_form_alter(&$form, &$form_state){}

/**
 * @param $form
 * @param $form_state
 *
 * Alters the related objects form
 * if the Object is of type OBJECT_CMODEL
 */
function hook_form_islandora_OBJECT_CMODEL_related_objects_form_alter(&$form, &$form_state){}


/**
 * @param $form
 * @param $form_state
 *
 * Alters the related objects form
 * if the Subject is of type RELATED_CMODEL
 */
function hook_form_islandora_related_RELATED_CMODEL_objects_form_alter(&$form, &$form_state){}


/**
 * @param $form
 * @param $form_state
 *
 * Alters the related objects form
 * if the Object is of type OBJECT_CMODEL
 * and the Subject is of type RELATED_CMODEL
 */
function hook_form_islandora_OBJECT_CMODEL_related_RELATED_CMODEL_objects_form_alter(&$form, &$form_state){}