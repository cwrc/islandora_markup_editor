<?php

/**
 * @file
 * Handles the OA callback requirements for islandora_markup_editor module.
 */


define("ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM",   "OA");
define("ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM_LABEL", "Open Annotation Serialization");
define("ISLANDORA_MARKUP_EDITOR_OA_CMODEL",   "cwrc:openAnnotationCModel");
define("ISLANDORA_MARKUP_EDITOR_OA_HASTARGET", "cwrc:hasTarget");

/**
 * Menu callback for the editor to persist OA Open Annotation data.
 *
 * @param $hasTarget_uri
 *    the URI of the target object in OA 
 */
function islandora_markup_editor_oa_hasTarget($hasTarget_uri) {

  try {
    //$result_array = array("success"=> urldecode($hasTarget_uri).urlencode("a/a") );
    
    $result_array = array( 
      '<rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:cw="http://cwrc.ca/ns/cw#" xmlns:oa="http://www.w3.org/ns/oa#" xmlns:foaf="http://xmlns.com/foaf/0.1/"><rdf:Description rdf:about="http://apps.testing.cwrc.ca/editor/documents/null"><cw:mode>0</cw:mode></rdf:Description><rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/annotation/6a82d26e-b059-4e25-88c0-35fd00085709"><oa:hasTarget xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/target/1e7c9ad5-358f-4c7f-90e3-0515a05b5558"/><oa:hasBody xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/person/56c9f7fe-d0ef-4c18-9563-e26272f9ee29"/><oa:annotatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource=""/><oa:annotatedAt xmlns:oa="http://www.w3.org/ns/oa#">2014-10-03T22:56:06.440Z</oa:annotatedAt><oa:serializedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource=""/><oa:serializedAt xmlns:oa="http://www.w3.org/ns/oa#">2014-10-03T22:56:06.440Z</oa:serializedAt><rdf:type rdf:resource="http://www.w3.org/ns/oa#Annotation"/><oa:motivatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://www.w3.org/ns/oa#tagging"/><oa:motivatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://www.w3.org/ns/oa#identifying"/><cw:hasCertainty xmlns:cw="http://cwrc.ca/ns/cw#" rdf:resource="http://cwrc.ca/ns/cw#reasonable"/><cw:cwrcInfo xmlns:cw="http://cwrc.ca/ns/cw#">{"id":"42172560","name":"Houde, Camillien","repository":"viaf"}</cw:cwrcInfo><cw:cwrcAttributes xmlns:cw="http://cwrc.ca/ns/cw#">{"cert":"reasonably certain","ref":"42172560"}</cw:cwrcAttributes></rdf:Description><rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/person/56c9f7fe-d0ef-4c18-9563-e26272f9ee29"><rdf:type rdf:resource="http://www.w3.org/ns/oa#SemanticTag"/><rdf:type rdf:resource="http://xmlns.com/foaf/0.1/Person"/></rdf:Description><rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/target/1e7c9ad5-358f-4c7f-90e3-0515a05b5558"><oa:hasSource xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/doc/690e168d-a5fc-4339-9772-66fd5b2cc315"/><rdf:type rdf:resource="http://www.w3.org/ns/oa#SpecificResource"/><oa:hasSelector xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/selector/e2c48327-c2cf-4731-ae6d-6ba0f352c546"/></rdf:Description><rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/selector/e2c48327-c2cf-4731-ae6d-6ba0f352c546"><rdf:value>xpointer(//persName[@annotationId="ent_88"])</rdf:value><rdf:type rdf:resource="http://www.w3.org/ns/oa#FragmentSelector"/></rdf:Description></rdf:RDF>'
      ); 

    $cwrc = array(
      "hasTarget" => urldecode($hasTarget_uri) 
      , "numFound" => "1"
      , "page" => array("start" => "0", "rows" => "5" )
      , "oa_items" => $result_array
      );
    drupal_json_output(
        $cwrc 
      );
  }

  catch (Exception $e) {
    watchdog('islandora_markup_editor',
      'Failed to lookup hasTarget @hasTarget_uri</br>code: @code<br/>message: @msg',
      array(
        '@hasTarget_uri' => $hasTarget_uri,
        '@code' => $e->getCode(),
        '@msg' => $e->getMessage(),
      ),
      WATCHDOG_ERROR
    );
    drupal_json_output(array("Error" => "An error during the lookup"));
  }
}


/**
 * Menu callback to handle the REST methods to handle an OA annotation.
 *
 * model after: islandora_image_annotation - islandora_rest_datastream and 
 *    islandora_rest_get_response functions to model the REST functionality
 *   
 */
function islandora_markup_editor_oa_annotation() {
  
  $results = 
      '<rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:cw="http://cwrc.ca/ns/cw#" xmlns:oa="http://www.w3.org/ns/oa#" xmlns:foaf="http://xmlns.com/foaf/0.1/"><rdf:Description rdf:about="http://apps.testing.cwrc.ca/editor/documents/null"><cw:mode>0</cw:mode></rdf:Description><rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/annotation/6a82d26e-b059-4e25-88c0-35fd00085709"><oa:hasTarget xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/target/1e7c9ad5-358f-4c7f-90e3-0515a05b5558"/><oa:hasBody xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/person/56c9f7fe-d0ef-4c18-9563-e26272f9ee29"/><oa:annotatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource=""/><oa:annotatedAt xmlns:oa="http://www.w3.org/ns/oa#">2014-10-03T22:56:06.440Z</oa:annotatedAt><oa:serializedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource=""/><oa:serializedAt xmlns:oa="http://www.w3.org/ns/oa#">2014-10-03T22:56:06.440Z</oa:serializedAt><rdf:type rdf:resource="http://www.w3.org/ns/oa#Annotation"/><oa:motivatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://www.w3.org/ns/oa#tagging"/><oa:motivatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://www.w3.org/ns/oa#identifying"/><cw:hasCertainty xmlns:cw="http://cwrc.ca/ns/cw#" rdf:resource="http://cwrc.ca/ns/cw#reasonable"/><cw:cwrcInfo xmlns:cw="http://cwrc.ca/ns/cw#">{"id":"42172560","name":"Houde, Camillien","repository":"viaf"}</cw:cwrcInfo><cw:cwrcAttributes xmlns:cw="http://cwrc.ca/ns/cw#">{"cert":"reasonably certain","ref":"42172560"}</cw:cwrcAttributes></rdf:Description><rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/person/56c9f7fe-d0ef-4c18-9563-e26272f9ee29"><rdf:type rdf:resource="http://www.w3.org/ns/oa#SemanticTag"/><rdf:type rdf:resource="http://xmlns.com/foaf/0.1/Person"/></rdf:Description><rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/target/1e7c9ad5-358f-4c7f-90e3-0515a05b5558"><oa:hasSource xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/doc/690e168d-a5fc-4339-9772-66fd5b2cc315"/><rdf:type rdf:resource="http://www.w3.org/ns/oa#SpecificResource"/><oa:hasSelector xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/selector/e2c48327-c2cf-4731-ae6d-6ba0f352c546"/></rdf:Description><rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/selector/e2c48327-c2cf-4731-ae6d-6ba0f352c546"><rdf:value>xpointer(//persName[@annotationId="ent_88"])</rdf:value><rdf:type rdf:resource="http://www.w3.org/ns/oa#FragmentSelector"/></rdf:Description></rdf:RDF>'
    ;
  
  drupal_add_http_header('Status', 201);
 
  drupal_json_output($results);
}











// ================================================================

/**
 * Menu callback to mint a new URI.
 *
 * 
 * @param $optional_UUID
 *    allows the specification of a UUID that becomes part of the URI 
 *   
 */
function islandora_markup_editor_oa_uri_mint($optional_UUID = null) {
  // URI cwrc.ca/oa/namespace:UUID
  
  // ToDo: should change to be the same namespace as the target
  $namespace = "cwrc"; 
  $prefix = "http://id.cwrc.ca/";

  if ($optional_UUID) 
  {
    $results = array("uri" => $prefix.$namespace.":".$optional_UUID );
  }
  else 
  {
    $connection = islandora_get_tuque_connection();
    $uuid = $connection->repository->getNextIdentifier($namespace, TRUE);

    $results = array("uri" => $prefix.$uuid );
  }

  drupal_json_output($results);
}




/**
 * Menu callback for the editor to persist OA Open Annotation data.
 *
 * @param $hasTarget_uri
 *    the URI of the target object in OA 
 */
function islandora_markup_editor_oa_hasTarget_v2($hasTarget_uri) {

  // given a hasTarget URI use RI to query list of object PIDS
  try {
    $query = <<<QUERY
      PREFIX io: <http://islandora.ca/ontology/relsext#>
      PREFIX fm: <info:fedora/fedora-system:def/model#>

      SELECT ?id from <#ri>
      WHERE {
        ?id io:targets <info:fedora/$hasTarget_uri> ;
        fm:state fm:Active ;
      }
QUERY;

    $connection = islandora_get_tuque_connection();
    $query_results = $connection->repository->ri->sparqlQuery($query);
    $oa_array = array();
    $count = 0;
    foreach ($query_results as $item) {
      $count++;
      $fedora_object = islandora_object_load($item['id']['value']);
      if (isset($fedora_object[ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM]))
      {
        $tmp = $fedora_object[ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM]->content;
      }
      if (!isset($tmp)) { 
        array_push ($oa_array, "");
      }
      else {
        array_push ($oa_array, $tmp);
      }
      unset($fedora_object);
      unset($tmp);
    }


    drupal_json_output( 
      array( 
        //"hasTarget" => "urldecode($hasTarget_uri)"
        "hasTarget" => "$hasTarget_uri"
        , "numFound" => "$count"
        , "page" => array("start" => "0", "rows" => "5" )
        , "oa_items" => $oa_array
      )
      );
  }

  catch (Exception $e) {
    watchdog('islandora_markup_editor',
      'Failed to lookup hasTarget @hasTarget_uri</br>code: @code<br/>message: @msg',
      array(
        '@hasTarget_uri' => $hasTarget_uri,
        '@code' => $e->getCode(),
        '@msg' => $e->getMessage(),
      ),
      WATCHDOG_ERROR
    );

    drupal_add_http_header('Status', 500);

    drupal_json_output(array("Error" => "error occured during the hasTarget lookup - see the Drupal WatchDog log for more details. $e->getMessage()"));
  }
}



/**
 * Menu callback to handle the REST methods to handle an OA annotation.
 *
 * model after: islandora_image_annotation - islandora_rest_datastream and 
 *    islandora_rest_get_response functions to model the REST functionality
 *  
 * @param array $url_parameters
 *   The GET parameters that were embedded within the path of the URL. They
 *   typically contain the following fields:
 *   - pid: Optional, the PID of the requested resource
 */
function islandora_markup_editor_oa_annotation_v2($uri = NULL) {
 
  // include REST utilities for the API callbacks 
  module_load_include('inc', 'islandora_rest', "includes/utilities");

  $endpoint = 'object';

  //$url_parameters = array_filter($url_parameters);
  $request_method = islandora_rest_get_request_method();
  //$pid = !empty($url_parameters['pid']) ? $url_parameters['pid'] : FALSE;
  //if ( $pid===FALSE ) {
  //  $pid = islandora_markup_editor_oa_convert_uri_to_pid($uri);
  //}

  $pid = islandora_markup_editor_oa_convert_uri_to_pid($uri);
  $dsid = FALSE;

  try {
    // Fetch the requested resource. Throws 404, if the resource is not found.
    $resource = islandora_rest_get_requested_resource($pid, $dsid);

    // Check Drupal Permissions. Throws 401, 403, if access is denied.
    islandora_markup_editor_require_access($endpoint, $request_method, $resource);

    $response = NULL;
    // Generate the Response.
    $response_function = "islandora_markup_editor_oa_annotation_{$request_method}_response";
    if (function_exists($response_function)) {
      $parameters = array(
        'uri' => $uri,
        'pid' => $pid,
        'request_method' => $request_method,
        'request' => islandora_rest_get_request_parameters(),
        'resource' => $resource,
      );


//    watchdog('islandora_markup_editor',
//      var_export($parameters),
//      array(
//      ),
//      WATCHDOG_ERROR
//    );



      // Can throw 400, 401, 500, etc.
      $response = $response_function($parameters);
    }
  }
  catch (Exception $e) {
    drupal_add_http_header('Status', $e->getCode());
    $response = array('message' => $e->getMessage());
  }
  $encode = is_array($response) || is_object($response);
  if ($encode) {
    // Reponses that don't generate objects or arrays, must set their own
    // content types. Not calling drupal_json_output because we are setting
    // encoding.
    drupal_add_http_header('Content-Type', 'application/json; utf-8');
    return drupal_json_encode($response);
  }
  return $response;
}



/**
 * Responds to the GET request for this resource.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - resource: The object to generate the response for.
 *
 * @return array
 *
 *
*/

function islandora_markup_editor_oa_annotation_get_response(array $parameters) {
  
  $fedora_object = $parameters['resource'];

  if (!isset($fedora_object)) {
    $tmp = $parameters['pid'];
  }
  else {
    $tmp = $fedora_object[ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM]->content;
  }

  //drupal_add_http_header('Status', 405);
  drupal_json_output($tmp);
}


/**
 * Responds to the PUT request for this resource.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - resource: The object to generate the response for.
 *   - request: The request parameters
 *     - label: The object's new label.
 *     - owner: The object's new owner.
 *     - state: The object's new state.
 *
 * @return array
 *   The un-encoded response containing the following fields.
 *   - modified: The objects last modified date.
 */
function islandora_markup_editor_oa_annotation_put_response(array $parameters) {
  /*
  $object = $parameters['resource'];
  $request = $parameters['request'];
  if (isset($request['label'])) {
    $object->label = $request['label'];
  }
  if (isset($request['owner'])) {
    $object->owner = $request['owner'];
  }
  if (isset($request['state'])) {
    $object->state = $request['state'];
  }
  return array(
    'pid' => $object->id,
    'label' => $object->label,
    'owner' => $object->owner,
    'state' => $object->state,
    'modified' => (string) $object->lastModifiedDate,
  );
   */

  drupal_add_http_header('Status', 405);
  drupal_json_output($parameters);
}


/**
 * Responds to the POST request for this resource.
 *
 * Creates a new object, with the given $params.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - request: The request parameters
 *     - uri: The identifier for the annotation 
 *     - hasTarget: target URI fro the annotation
 *     - annotation: RDF XML serialization of the annotation
 *
 * @return array
 *   Returns the same response as a GET request to the existing object.
 */
function islandora_markup_editor_oa_annotation_post_response(array $parameters) {
  
  module_load_include('inc', 'islandora', 'includes/utilities');

  $request = $parameters['request'];

  $annotation_xml = $request['annotation'];

  $hasTarget = $request['hasTarget'];
  //$content_model = ISLANDORA_MARKUP_EDITOR_OA_CMODEL;

  $uri = $request['uri'];
  $pid = islandora_markup_editor_oa_convert_uri_to_pid($uri);


    watchdog('islandora_markup_editor post',
      'pid @pid</br>',
      array(
        '@pid' => $pid,
      ),
      WATCHDOG_ERROR
    );




    // We need to prepare a new object.
  $fedora_object
    = islandora_prepare_new_object(
      $pid
      , NULL 
      , array()
      , array(ISLANDORA_MARKUP_EDITOR_OA_CMODEL)
      , array()
    );

  # ToDo: temporary 'targets', consider removing
  $fedora_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'targets', substr( $hasTarget, strrpos( $hasTarget, '/' )+1 ) );
  $fedora_object->relationships->add('http://cwrc.ca/ns/relsext#', ISLANDORA_MARKUP_EDITOR_OA_HASTARGET, $hasTarget);

  // create content datastream
  $oa_ds 
    = $fedora_object->constructDatastream(ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM, 'M');
  $oa_ds->label = ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM_LABEL;
  $oa_ds->mimeType = 'text/xml';
  $oa_ds->setContentFromString($annotation_xml);
  $fedora_object->ingestDatastream($oa_ds);



  islandora_add_object($fedora_object);



/*
  $tuque = islandora_get_tuque_connection();

  // create Fedora Object
  $fedora_object = $tuque->repository->constructObject($pid);

  #$fedora_object->label = isset($request['label']) ? $request['label'] : $fedora_object->label;
  #$fedora_object->owner = isset($request['owner']) ? $request['owner'] : $fedora_object->owner;

  $tuque->repository->ingestObject($fedora_object);

  // add datastream
  if (!isset($fedora_object[ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM])) {
    $obj_ds = $fedora_object->constructDatastream(ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM, 'M');
    $new_ds->mimetype = 'text/xml';
    $new_ds->content = $annotation_xml;
    $fedora_object->ingestDatastream($obj_ds);
  }
  else {
    $obj_ds = $fedora_object[ISLANDORA_MARKUP_EDITOR_OA_DATASTREAM];
    $obj_ds->setContentFromString($annotation_xml);
  }

  // add RELS-EXT hasTarget
  $fedora_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'targets', $hasTarget);
  $fedora_object->relationships->add('http://cwrc.ca/ns/relsext#', 'hasTarget', $hasTarget);
  //is member of
  $fedora_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $content_model);

  islandora_add_object($fedora_object);
*/



  drupal_add_http_header('Status', 201);



  return drupal_json_output($parameters);

  // Mock the Get Response.
  //return islandora_markup_editor_oa_annotation_get_response(array('resource' => $fedora_object));
}


/**
 * Responds to the DELETE request for this resource.
 *
 * Delete's the given object.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - resource: The object to generate the response for.
 */
function islandora_markup_editor_oa_annotation_delete_response(array $parameters) {
  $object = $parameters['resource'];
  $object->repository->purgeObject($object->id);
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




function islandora_markup_editor_oa_convert_uri_to_pid($uri)
{
  $match_array = array();
  //preg_match("/^.*\/(cwrc:[\w-]*)/i",$uri, $match_array);
  //preg_match("/(cwrc:[\w-]*)/i",$uri, $match_array);
  preg_match("/([\w]+:[\w-]*)/i",$uri, $match_array);
  $pid = !empty($match_array[1]) ? $match_array[1] : FALSE;
  return $pid;
}


/**
 * Checks to see if the current user has access for the requested resource.
 *
 * @throw Exception If not access is not granted to the user. If the user is not
 * logged in throw a 'Unauthorized' exception, if they are logged in throw a
 * 'Forbidden' exception.
 *
 * @param string $endpoint
 *   The rest end-point to generate the response for.
 * @param array $request_method
 *   The request method (GET/POST/PUT/DELETE).
 * @param object $resource
 *   The requested Fedora object/datastream (optional).
 *
 * @return bool
 *   TRUE if all parameters are present FALSE otherwise.
 */
function islandora_markup_editor_require_access($endpoint, $request_method, $resource = NULL) {
  if (!islandora_markup_editor_accessible($endpoint, $request_method, $resource)) {
    if (user_is_anonymous()) {
      throw new Exception('Unauthorized', 401);
    }
    else {
      throw new Exception('Forbidden', 403);
    }
  }
}


/**
 * Check module permissions etc, to see if the current action is allowed.
 *
 * @param array $request_method
 *   The request method (GET/POST/PUT/DELETE)
 * @param object $resource
 *   The requested Fedora object/datastream (optional).
 *
 * @return bool
 *   TRUE if the user is allowed to access the rest end-point, FALSE otherwise.
 */
function islandora_markup_editor_accessible($endpoint, $request_method, $resource = FALSE) {
  $perm = islandora_markup_editor_resource_access_permission($endpoint, $request_method);
  if (is_subclass_of($resource, 'AbstractObject')) {
    return islandora_object_access($perm, $resource);
  }
  elseif (is_subclass_of($resource, 'AbstractDatastream')) {
    return islandora_datastream_access($perm, $resource);
  }
  return user_access($perm);
}


/**
 * Gets the defined access permission for the given resource and request method.
 *
 * @param string $endpoint
 *   The rest end-point to generate the response for.
 * @param array $request_method
 *   The request method (GET/POST/PUT/DELETE).
 */
function islandora_markup_editor_resource_access_permission($endpoint, $request_method) {
  $perm_map = array(
    'object' => array(
      'GET' => ISLANDORA_OPEN_ANNOTATION_VIEW,
      'PUT' => ISLANDORA_OPEN_ANNOTATION_CREATE,
      'POST' => ISLANDORA_OPEN_ANNOTATION_CREATE,
      'DELETE' => ISLANDORA_OPEN_ANNOTATION_CREATE,
    ),
  );
  return $perm_map[$endpoint][$request_method];
}




