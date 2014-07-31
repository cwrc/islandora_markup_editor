<?php
/**
 * @file
 * Handles the callback requirements for islandora_markup_editor module.
 */

/**
 * Menu callback for the editor to persist TEI data.
 *
 * @param AbstractObject $fedora_object
 *   The fedora object
 */
function islandora_markup_editor_savedata(AbstractObject $fedora_object) {
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
 * Menu callback to provide setup info for the cwrc-writer/viewer.
 *
 * @param AbstractObject $fedora_object
 *   Fedora object to provide setup for.
 */
function islandora_markup_editor_setup(AbstractObject $fedora_object) {
  global $user;
  module_load_include('inc', 'islandora_basic_collection', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $stroke_width = variable_get('image_annotation_annotation_stroke_width', '1%');
  $pages = array();
  $results = array();
  $results['uid'] = $user->uid;
  $results['position'] = "1";
  $results['pages'] = $pages[0] = $fedora_object->id;
  $results['title'] = $fedora_object->label;
  $results['no_edit'] = FALSE;
  $results['page_count'] = 1;
  $results['islandora_anno_stroke_width'] = '1%';

  $place_entity_collection = variable_get('islandora_entities_places_collection', 'islandora:entity_collection');
  $results['create_entity_callbacks']['places'] = url('islandora/object/' . $place_entity_collection . '/manage/overview/ingest');
  $event_entity_collection = variable_get('islandora_entities_events_collection', 'islandora:entity_collection');
  $results['create_entity_callbacks']['events'] = url('islandora/object/' . $event_entity_collection . '/manage/overview/ingest');
  $organization_entity_collection = variable_get('islandora_entities_organizations_collection', 'islandora:entity_collection');
  $results['create_entity_callbacks']['organizations'] = url('islandora/object/' . $organization_entity_collection . '/manage/overview/ingest');
  $person_entity_collection = variable_get('islandora_entities_people_collection', 'islandora:entity_collection');
  $results['create_entity_callbacks']['people'] = url('islandora/object/' . $person_entity_collection . '/manage/overview/ingest');

  drupal_json_output($results);
}
