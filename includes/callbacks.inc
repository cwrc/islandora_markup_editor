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

  if (module_exists('cwrc_entities'))
  {
    $results['annotations'] = islandora_markup_editor_setup_annotations();
    $results['entities'] = islandora_markup_editor_setup_entities($fedora_object);
    $results['cwrcDialogs'] = islandora_markup_editor_setup_entity_dialogs_helper();
  }
  $results['resource'] = islandora_markup_editor_setup_resource($fedora_object);
  $results['schemas'] = islandora_markup_editor_setup_cwrc_writer_helper();

  drupal_json_output($results);
}


/**
 * support for the callback to provide setup info for the cwrc-writer/viewer.
 * annotation urls
 *
 */
function islandora_markup_editor_setup_annotations() 
{

  // global Drupal variables 
  global $base_url;

  $p = $base_url.'/';

  return $tmp = array (
    "cwrc:handleAnnotations" => $p . CWRC_OPEN_ANNOTATION_API
    );
}



/**
 * support for the callback to provide setup info for the cwrc-writer/viewer.
 * entity URLs 
 *
 */
function islandora_markup_editor_setup_entities_helper(
  $hasLookupBase = NULL
  , $hasUpdatePoint = NULL
  , $hasContent = NULL
  ) 

{
  $tmp = array();

  if ($hasLookupBase != null)
  {
    $tmp["cwrc:hasLookupBase"] = $hasLookupBase;
  }
  if ($hasUpdatePoint != null)
  {
    $tmp["cwrc:hasUpdatePoint"] = $hasUpdatePoint;
  }
  if ($hasContent != null)
  {
    $tmp["cwrc:hasContent"] = $hasContent;
  }

  if (empty($tmp))
  {
    $tmp = null;
  }

  return $tmp;
}

/**
 * support for the callback to provide setup info for the cwrc-writer/viewer.
 * entities
 *
 * @param AbstractObject $fedora_object
 *   Fedora object to provide setup for.
 */
function islandora_markup_editor_setup_entities(AbstractObject $fedora_object) 
{

  module_load_include('inc', 'cwrc_entities', 'cwrc_entities.module');

  // global Drupal variables 
  global $base_url;

  $pid = $fedora_object->id;
  $p = $base_url.'/';


  // build JSON structure for Orlando Entities
  $person 
    = islandora_markup_editor_setup_entities_helper(
      $p . CWRC_ENTITES_API_PERSON_SEARCH 
      , $p . CWRC_ENTITES_API_PERSON
      , $p . CWRC_ENTITES_API_PERSON
      );

  $organization 
    = islandora_markup_editor_setup_entities_helper(
      $p . CWRC_ENTITES_API_ORGANIZATION_SEARCH 
      , $p . CWRC_ENTITES_API_ORGANIZATION
      , $p . CWRC_ENTITES_API_ORGANIZATION
      );

  $place 
    = islandora_markup_editor_setup_entities_helper(
      $p . CWRC_ENTITES_API_PLACE_SEARCH 
      , $p . CWRC_ENTITES_API_PLACE
      , $p . CWRC_ENTITES_API_PLACE
      );

  $title 
    = islandora_markup_editor_setup_entities_helper(
      $p . CWRC_ENTITES_API_TITLE_SEARCH 
      , $p . CWRC_ENTITES_API_TITLE
      , $p . CWRC_ENTITES_API_TITLE
      );

  // build JSON structure for default / CWRC Entities

  $default = array(
    "person" => $person
    , "organization" => $organization
    , "place" => $place
    , "title" => $title
    );


  // build JSON structure for default / CWRC Entities
  // same as CWRC for now - 2014-11-07
  $orlando = array(
    "person" => $person
    , "organization" => $organization
    , "place" => $place
    , "title" => $title
  );


  // build JSON structure for international authority for Entities
  $person = $organization = $place = $title = null;
  $person 
    = islandora_markup_editor_setup_entities_helper(
      $p . "services/viaf/search?query=local.personalNames"
      , null
      , null
      );

  $organization 
    = islandora_markup_editor_setup_entities_helper(
      $p . "services/viaf/search?query=local.corporateNames"
      , null
      , null
      );

  $place 
    = islandora_markup_editor_setup_entities_helper(
      $p . 'cwrc-mtp/geonames/?query='
      , null
      , null
      );

  $title 
    = islandora_markup_editor_setup_entities_helper(
      $p . "services/viaf/search?query=local.uniformTitleWorks"
      , null
      , null
      );

  $international_authority = array(
    "person" => $person
    , "organization" => $organization
    , "place" => $place
    , "title" => $title
    );



  // build JSON structure for special entities lookups 
  $person = $organization = $place = $title = null;
  $place 
    = islandora_markup_editor_setup_entities_helper(
      "http://maps.googleapis.com/maps/api/geocode/xml?address="
      , null
      , null
      );

  $special = array(
    "place" => $place
    );



  // final build
  return $tmp = array (
    "default" => $default
    , "orlando" => $orlando
    , "international_authority" => $international_authority
    , "special" => $special
    );
}

/**
 * support for the callback to provide setup info for the cwrc-writer/viewer.
 * 
 *
 * @param AbstractObject $fedora_object
 *   Fedora object to provide setup for.
 */
function islandora_markup_editor_setup_resource(AbstractObject $fedora_object) 
{
  // global Drupal variables 
  global $base_url;

  $pid = $fedora_object->id;
  $p = $base_url.'/';

  return array (
    "cwrc:hasAnnotation" => $p . CWRC_OPEN_ANNOTATION_API_HASTARGET . $pid
    , "cwrc:hasDocUpdatePoint" => $p . ISLANDORA_MARKUPEDITOR_API_UPDATE . $pid
    , "cwrc:hasCopiesOfText" => $p . sprintf(ISLANDORA_MARKUPEDITOR_API_VIEW, $pid)
    , "cwrc:hasID" => $pid 
    );
}


/**
 * support for the callback to provide setup info for the cwrc-writer/viewer.
 * CWRC Entity Dialogs - modeled after writerConfig.js
 *
 * TODO: setup URLs so that they are not hardcoded
 */
function islandora_markup_editor_setup_entity_dialogs_helper()
{
  global $base_url;

  $tmp = null;

  $tmp = array(
    "cwrcApiUrl" => $base_url . "/" . CWRC_ENTITES_API . '/',
    "geonameUrl" => $base_url . "/cwrc-mtp/geonames/",
    "viafUrl" => $base_url . "/services/viaf/",
    "schemas" => array(
            "person" => "http://cwrc.ca/schemas/entities.rng",
            "organization" => "http://cwrc.ca/schemas/entities.rng",
            "place" => "http://cwrc.ca/schemas/entities.rng"
        )
    );
    return $tmp;
}

/**
 * support for the callback to provide setup info for the cwrc-writer/viewer.
 * CWRC Writer - modeled after writerConfig.js
 *
 * TODO: setup URLs so that they are not hardcoded
 */

function islandora_markup_editor_setup_cwrc_writer_helper()
{

  global $base_url;

  $tmp = null;

  $CWRC_Writer_path = $base_url . '/' . libraries_get_path('CWRC-Writer');

  $tmp = array(
        "tei" => array(
            "name" => "CWRC Basic TEI Schema",
            "url" => "http://cwrc.ca/schemas/cwrc_tei_lite.rng",
            "cssUrl" => $CWRC_Writer_path . "/src/css/tei_converted.css",
            "schemaMappingsId" => "tei"
        ),
        "events" => array(
            "name" => "Events Schema",
            "url" => "http://cwrc.ca/schemas/orlando_events.rng",
            "cssUrl" => $CWRC_Writer_path . "/src/css/orlando_converted.css",
            "schemaMappingsId" => "orlando"
        ),
        "biography" => array(
            "name" => "Biography Schema",
            "url" => "http://cwrc.ca/schemas/orlando_biography.rng",
            "cssUrl" => $CWRC_Writer_path . "/src/css/orlando_converted.css",
            "schemaMappingsId" => "orlando"
        ),
        "writing" => array(
            "name" => "Writing Schema",
            "url" => "http://cwrc.ca/schemas/orlando_writing.rng",
            "cssUrl" => $CWRC_Writer_path . "/src/css/orlando_converted.css",
            "schemaMappingsId" => "orlando"
        ),
        "cwrcEntry" => array(
            "name" => "CWRC Entry Schema",
            "url" => "http://cwrc.ca/schemas/cwrc_entry.rng",
            "cssUrl" => $CWRC_Writer_path . "/src/css/cwrc_converted.css",
            "schemaMappingsId" => "cwrcEntry"
        )
  );

  return $tmp;
}
