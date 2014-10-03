<?php
/**
 * @file
 * Handles the OA callback requirements for islandora_markup_editor module.
 */

/**
 * Menu callback for the editor to persist OA Open Annotation data.
 *
 * @param $hasTarget_uri
 *    the URI of the target object in OA 
 */
function islandora_markup_editor_oa($hasTarget_uri) {

  try {
    drupal_json_output($cwrc);
  }

  catch (Exception $e) {
    watchdog('islandora_markup_editor',
      'Failed to set OBJ datastream content for pid @pid</br>code: @code<br/>message: @msg',
      array(
        '@pid' => $fedora_object->id,
        '@code' => $e->getCode(),
        '@msg' => $e->getMessage(),
      ),
      WATCHDOG_ERROR
    );
    drupal_json_output(array("Error" => "An error during the lookup"));
  }
}


function islandora_markup_editor_oa_update() {
  module_load_include('inc', 'islandora_markup_editor', 'includes/utilities');
  try {
    parse_str(file_get_contents('php://input'), $put);
    // @codingStandardsIgnoreStart
    $data = $put['text'];
    $cwrc = str_replace('<br>', '<br />', $data);
    // @codingStandardsIgnoreEnd
    $cwrc = str_replace('&', '&amp;', $cwrc);
    // Update ds.
    if (!isset($fedora_object["OBJ"])) {
      $obj_ds = $fedora_object->constructDatastream('OBJ', 'M');
      $fedora_object->ingestDatastream($obj_ds);
    }
    else {
      $obj_ds = $fedora_object["OBJ"];
    }
    $obj_ds->setContentFromString($cwrc);

    // Sanitize the schema URL, incase it gets managled during transport.
    $schema = html_entity_decode(stripslashes(check_plain($put['schema'])), ENT_QUOTES, 'UTF-8');
    islandora_markup_editor_add_tei_processing_instruction($fedora_object, $schema);

    drupal_json_output($cwrc);
  }
  catch (Exception $e) {
    watchdog('islandora_markup_editor',
      'Failed to set OBJ datastream content for pid @pid</br>code: @code<br/>message: @msg',
      array(
        '@pid' => $fedora_object->id,
        '@code' => $e->getCode(),
        '@msg' => $e->getMessage(),
      ),
      WATCHDOG_ERROR
    );
    drupal_json_output(array("Error" => "An error occured updating "));
  }
 
}


/**
 * Menu callback to mint a new URI.
 *
 * 
 *   
 */
function islandora_markup_editor_oa_uri_mint() {

  $results = array("uri","cwrc.ca/oa/namespace:UUID");

  drupal_json_output($results);
}
