<?php
/**
 * @file Islandora Bioinformatics Hook API
 *
 *       Lists the hooks used by this file.
 */

/**
 * Alters the related objects page.
 */
function hook_islandora_related_objects_page_alter(&$page, &$page_state){
}

/**
 * Alters the related objects page
 * if the Object is of type OBJECT_CMODEL
 */
function hook_islandora_OBJECT_CMODEL_objects_page_alter(&$page, &$page_state){}

/**
 * Alters the related objects form.
 */
function hook_form_islandora_related_objects_form_alter(&$form, &$form_state){}

/**
 * Alters the related objects form
 * if the Object is of type OBJECT_CMODEL
 */
function hook_islandora_OBJECT_CMODEL_related_objects_form_alter(&$form, &$form_state){}

/**
 * Alters the related objects form
 * if the Subject is of type SUBJECT_CMODEL
 */
function hook_islandora_related_SUBJECT_CMODEL_objects_form_alter(&$form, &$form_state){}

/**
 * Alters the related objects form
 * if the Object is of type OBJECT_CMODEL
 * and the Subject is of type SUBJECT_CMODEL
 */
function hook_islandora_OBJECT_CMODEL_related_SUBJECT_CMODEL_objects_form_alter(&$form, &$form_state){}
