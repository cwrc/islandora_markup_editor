<?php

/**
 * @file
 * Handles the Entity callback requirements for islandora_markup_editor module.
 */


define("ISLANDORA_MARKUP_EDITOR_ENTITIES_DATASTREAM",   "");
define("ISLANDORA_MARKUP_EDITOR_ENTITIES_DATASTREAM_LABEL", "");
define("ISLANDORA_MARKUP_EDITOR_ENTITIES_CMODEL",   "cwrc:openAnnotationCModel");
define("ISLANDORA_MARKUP_EDITOR_ENTITIES_HASTARGET", "cwrc:hasTarget");
define("ISLANDORA_MARKUP_EDITOR_ENTITIES_CWRC_NS", 'http://cwrc.ca/ns/relsext#');



/**
 * Menu callback to handle the REST methods to handle an Entity .
 *
 * model after: islandora_image_annotation - islandora_rest_datastream and 
 *    islandora_rest_get_response functions to model the REST functionality
 *  
 * @param array $uri
 *   The GET parameters that were embedded within the path of the URL. They
 *   typically contain the following fields:
 *   - pid: Optional, the PID of the requested resource
 *
 * @param string cModel
 *   optional - Fedora cModel describing the entity type
 *
 *
 * @param string collection = NULL
 *    optional - Fedora PID of the collection to add the entity as a child
 *
 * @param string workstatus = NULL
 *    optional - CWRC workflow status information 
 *
 *
 * @param string workstamp = NULL
 *    optional - CWRC workflow status information 
 *
 *
 * @param string entityToDCXSL = NULL
 *    optional - path to an XSLT to generate a DC datastream 
 *
 */
function islandora_markup_editor_entities(
  $uri = NULL
  , $cModel = NULL
  , $collectionPID = NULL
  , array $activity = NULL
  , $entityToDCXSLT = NULL
  , $dataStreamID = NULL
  ) 
  {
 
  // include REST utilities for the API callbacks 
  module_load_include('inc', 'islandora_rest', "includes/utilities");

  $endpoint = 'object';

  //$url_parameters = array_filter($url_parameters);
  $request_method = islandora_rest_get_request_method();
  //$pid = !empty($url_parameters['pid']) ? $url_parameters['pid'] : FALSE;
  //if ( $pid===FALSE ) {
  //  $pid = islandora_markup_editor_oa_convert_uri_to_pid($uri);
  //}

  $pid = islandora_markup_editor_entities_convert_uri_to_pid($uri);
  $dsid = FALSE;

  try {
    // Fetch the requested resource. Throws 404, if the resource is not found.
    $resource = islandora_rest_get_requested_resource($pid, $dsid);

    // Check Drupal Permissions. Throws 401, 403, if access is denied.
    islandora_markup_editor_require_access($endpoint, $request_method, $resource);

    // Generate the Response.
    $response_function = "islandora_markup_editor_entities_{$request_method}_response";
    if (function_exists($response_function)) {
      $parameters = array(
        'uri' => $uri
        , 'pid' => $pid
        , 'request_method' => $request_method
        , 'request' => islandora_rest_get_request_parameters()
        , 'resource' => $resource
        , 'cModel' => $cModel
        , 'collectionPID' => $collectionPID
        , 'activity' => $activity
        , 'entityToDCXSLT' => $entityToDCXSLT
        , 'dataStreamID' => $dataStreamID
      );


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

function islandora_markup_editor_entities_get_response(array $parameters) {

  $fedora_object = $parameters['resource'];
  $ds_id = $parameters['dataStreamID'];

  if (!isset($fedora_object) or !isset($fedora_object[$ds_id])) {
    $tmp = $parameters['pid'];
    drupal_add_http_header('Status', 404);
  }
  else {
    $tmp = $fedora_object[$ds_id]->content;
  }

  return drupal_json_output($tmp);
}


/**
 * Responds to the PUT request for this resource.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - resource: The object to generate the response for.
 *   - request: The request parameters
 *     - annotation: The object's OA datastream contents.
 *
 * @return 
 */
function islandora_markup_editor_entities_put_response(array $parameters) {

  $fedora_object = $parameters['resource'];
  $ds_id = $parameters['dataStreamID'];
  $activity_params = $parameters['activity'];
  $entityToDCXSLT = $parameters['entityToDCXSLT'];

  $request = $parameters['request'];
  $content_blob = $request[ISLANDORA_CWRC_ENTITES_REQUEST_CONTENT_PARAM];

  if (!isset($fedora_object) or !isset($fedora_object[$ds_id])) {
    drupal_add_http_header('Status', 404);
  }
  else {

    // update content datastream
    $fedora_object[$ds_id]->setContentFromString($content_blob);

    // add a workflow record 
    if (isset($activity_params)) {
      islandora_markup_entity_add_workflow($fedora_object, $activity_params);
    }

    // update the DC datastream and the associated Fedora Object label
    if (isset($entityToDCXSLT)) {
      $dc = islandora_markup_entity_build_DC($fedora_object, $entityToDCXSLT, $content_blob);
      islandora_markup_entity_update_DC($fedora_object, $dc);
      $title  = islandora_markup_entity_get_title_from_DC($dc);
      $fedora_object->label = $title;
    }
  }

  //return islandora_markup_editor_entities_get_response(array('resource' => $fedora_object));
  return drupal_json_output($content_blob);
      //return drupal_json_output(print_r($fedora_object));
  //return drupal_json_output(print_r($activity_params)); 
  //return drupal_json_output("");
}


/**
 * Responds to the POST request for this resource.
 *
 * Creates a new object, with the given $params.
 *
 * @param array $parameters
 *   An associative array containing relevent data for this request.
 *   - request: The request parameters
 *     - data: content blob to store in Fedora 
 *
 *   - dataStreamID: DS_ID to store the content of the Fedora Object
 *
 * @return array
 *   Returns the same response as a GET request to the existing object.
 */
function islandora_markup_editor_entities_post_response(array $parameters) {
  
  module_load_include('inc', 'islandora', 'includes/utilities');

  $ds_id = $parameters['dataStreamID'];
  $cModel = $parameters['cModel'];
  $collectionPID = $parameters['collectionPID'];
  $activity_params = $parameters['activity'];
  $entityToDCXSLT = $parameters['entityToDCXSLT'];

  $request = $parameters['request'];

  $content_blob = $request[ISLANDORA_CWRC_ENTITES_REQUEST_CONTENT_PARAM];

  $hasTarget = $request['hasTarget'];

  $uri = $request['uri'];
  $pid = islandora_markup_editor_entities_convert_uri_to_pid($uri);

  if (!isset($pid) or $pid == FALSE)
  {
    // generate an id for the object
    $connection = islandora_get_tuque_connection();
    $pid = $connection->repository->getnextIdentifier("cwrc", TRUE);
  }

  // prepare a new object.
  $fedora_object
    = islandora_prepare_new_object(
      $pid
      , NULL 
      , array()
      , array($cModel)
      , array()
      , array(
          'relationship' => 'isMemberOfCollection'
          , 'pid' => "$collectionPID"
        )
    );


  // add the new object as a child of a specific collection
  if (isset($collectionPID))
  {
    $fedora_object->relationships->add(
      FEDORA_RELS_EXT_URI
      , 'isMemberOfCollection'
      , $collectionPID 
    );
  }

  // add a hasTarget relationship
  if (isset($hasTarget) or isset($hasTarget[$ds_id])) {
    # add RELS-EXT relationships
    # ToDo: temporary 'targets', consider removing
    $fedora_object->relationships->add(
      ISLANDORA_RELS_EXT_URI
      , 'targets'
      , substr( $hasTarget, strrpos( $hasTarget, '/' )+1) 
    );
    $fedora_object->relationships->add(
      ISLANDORA_MARKUP_EDITOR_OA_CWRC_NS
      , ISLANDORA_MARKUP_EDITOR_OA_HASTARGET
      , $hasTarget
      , RELS_TYPE_FULL_URI
    );
  }

  // add a workflow record 
  if (isset($activity_params)) {
    islandora_markup_entity_add_workflow($fedora_object, $activity_params);
  }

  // update the DC datastream and the associated Fedora Object label
  if (isset($entityToDCXSLT)) {
    $dc = islandora_markup_entity_build_DC($pid,$entityToDCXSLT,$content_blob);
    $title  = islandora_markup_entity_get_title_from_DC($dc);
    islandora_markup_entity_update_DC($fedora_object, $dc);
    $fedora_object->label = $title;
  }


  // create content datastream
  $content_ds 
    = $fedora_object->constructDatastream($ds_id, 'M');
  $content_ds->label = $ds_id;
  $content_ds->mimeType = 'text/xml';
  $content_ds->setContentFromString($content_blob);
  $fedora_object->ingestDatastream($content_ds);


  // save the object 
  islandora_add_object($fedora_object);

  // set the response status
  drupal_add_http_header('Status', 201);
  //return drupal_json_output($parameters);

  return drupal_json_output($fedora_object->id);

  // Mock the Get Response.
  return islandora_markup_editor_entities_get_response(array('resource' => $fedora_object));
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
function islandora_markup_editor_entities_delete_response(array $parameters) {
  $object = $parameters['resource'];
  $object->repository->purgeObject($object->id);
}



function islandora_markup_editor_entities_convert_uri_to_pid($uri)
{
  $match_array = array();
  //preg_match("/^.*\/(cwrc:[\w-]*)/i",$uri, $match_array);
  //preg_match("/(cwrc:[\w-]*)/i",$uri, $match_array);
  preg_match("/([\w]+:[\w-]*)/i",$uri, $match_array);
  $pid = !empty($match_array[1]) ? $match_array[1] : FALSE;
  return $pid;
}


/**
 * build a DC xml file 
 *
 * @param string $pid
 *   PID of the fedora object 
 *
 */
function islandora_markup_entity_build_DC($pid, $entityToDCXSLT, $content_blob)
{
    // setup XSL
    // Get the path to the content_to_dc.xsl file.
    $path_to_xsl_file
          = drupal_get_path('module', 'islandora_markup_editor') . $entityToDCXSLT;
    $xsl_doc = new DOMDocument();
    $xsl_doc->load($path_to_xsl_file);

    $xslt_proc = new XSLTProcessor();
    $xslt_proc->importStylesheet($xsl_doc);
    
    $input_doc = new DOMDocument();
    $input_doc->loadXML($content_blob);

    // build the DC content
    // Convert MODS to DC.
    // Transform to DC xml.
    // Apply the stylesheet.
    $xslt_proc->setParameter( "", "PID_PARAM", $pid );
    $dc_xml = $xslt_proc->transformToXML($input_doc);

    return $dc_xml;
}


/**
 * Update the DC datastream of the specific object 
 *
 * @param object $fedora_object
 *   The requested Fedora object/datastream .
 * @param string $dc_xml
 *   DC record for the Fedora Object 
 *
 */
function islandora_markup_entity_update_DC($fedora_object, $dc_xml)
{
    if (!isset($fedora_object['DC'])) {
      // create content datastream
      $ds 
        = $fedora_object->constructDatastream('DC', 'M');
      $ds->label = 'DC';
      $ds->mimeType = 'text/xml';
      $ds->setContentFromString($dc_xml);
      $fedora_object->ingestDatastream($ds);
    }
    else
    {
      $fedora_object['DC']->setContentFromString($dc_xml);
    }
}

/**
 * Get the Fedora Object Title from the DC record 
 *
 * @param string $dc_xml
 *   DC record for the Fedora Object 
 *
 */
function islandora_markup_entity_get_title_from_DC($dc_xml)
{
    $dc_doc = new DOMDocument();
    $dc_doc->loadXML($dc_xml); 
    $xpath = new DOMXPath($dc_doc);
    $xpath->registerNamespace('dc',"http://purl.org/dc/elements/1.1/");
    $xpath->registerNamespace('oai_dc',"http://www.openarchives.org/OAI/2.0/oai_dc/");
    $label_node = $xpath->evaluate('/oai_dc:dc/dc:title/text()');

    return $label_node->item(0)->nodeValue;
}


/**
 * add a simple workflow activity to the Entity's WORKFLOW datastream 
 *
 * @param object $fedora_object
 *   The requested Fedora object/datastream .
 * @param object $activity_params
 *   an associative array of activity params such as stamp, status
 *
 */
function islandora_markup_entity_add_workflow($fedora_object, array $activity_params) {
    //$use_realname = module_exists('realname');
    module_load_include('inc', 'islandora_workflow_rest', 'includes/cwrc_workflow');
    module_load_include('inc', 'islandora_workflow_rest', 'includes/basic_enum');
    module_load_include('inc', 'islandora_workflow_rest', 'includes/workflow_item');
    module_load_include('inc', 'islandora_workflow_rest', 'includes/utilities');

    try
    {
      if (!isset($fedora_object['WORKFLOW'])) {
          $workflow = CWRCWorkflowAPI::fromTemplate();
          CWRCWorkflowAPI::updateDatastream($workflow, $fedora_object);
      }
      else {
          $workflow = CWRCWorkflowAPI::fromDatastream($fedora_object['WORKFLOW']);
      }

      //createDefaultParamsForWorkflow
      $entry = new WorkflowItem(WorkflowConst::WORKFLOW, array());
      $activity = new WorkflowItem(WorkflowConst::ACTIVITY, $activity_params);
      //$assigned = new WorkflowItem(WorkflowConst::ASSIGNED, $assigned_params);
      $assigned = NULL;

      $workflow->addWorkflowStep($fedora_object->id, $entry, $activity, $assigned);
      CWRCWorkflowAPI::updateDatastream($workflow, $fedora_object);
    }
    catch (Exception $e)
    {
      throw new Exception("Unable to create workflow. reason: " . $e->getMessage());
    }
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
      'GET' => ISLANDORA_CWRC_ENTITY_VIEW,
      'PUT' => ISLANDORA_CWRC_ENTITY_CREATE,
      'POST' => ISLANDORA_CWRC_ENTITY_CREATE,
      'DELETE' => ISLANDORA_CWRC_ENTITY_CREATE,
    ),
  );
  return $perm_map[$endpoint][$request_method];
}



/**
 * call the Solr search API 
 * modeled off of CWRC-ccm-rest-api/controllers/entity.php
 *
 * @param string $cModel
 *    Fedora cModel describing the type of entity
 *
 * @param array solr_field_array
 *   Solr field array names to search 
 **/
function islandora_markup_editor_entity_search($cModel, $solr_field_array)
{
  try
  {
    module_load_include('inc', 'islandora_rest', "includes/utilities");

    $tmp = islandora_rest_get_request_parameters();
    $limit  = $tmp['limit'];
    $page   = $tmp['page'];
    $queryTerms  = $tmp['query'];

    // not sure if needed - 2014-11-5 - copied directly from ccm-rest-api entity.php
    $queryTerms = strtolower($queryTerms);
    $queryTerms = preg_replace("/\+/", " ", $queryTerms);
    $queryTerms = urldecode($queryTerms);
    $queryTerms = preg_replace("/\s/", "+", $queryTerms);

    // build query string
    $iter = 0;
    $queryStr = "(";
    foreach ($solr_field_array as $item)
    {
      if ($iter !=0)
      {
        $queryStr = $queryStr . "%20OR%20";
      }
      $queryStr .= $item . ":" . "(" . $queryTerms .")";
    }
    $queryStr .= ")";

    $request = array(
      'wt' => 'json'
      , 'limit' => $limit 
      , 'page'  => $page
      , 'f[]' => "RELS_EXT_hasModel_uri_ms:" . str_replace(array("%3A"), "%5C:", urlencode($cModel))
      );

    $solr = new IslandoraSolrQueryProcessor();
    $solr->buildAndExecuteQuery($queryStr, $request);
    $response = $solr->islandoraSolrResult;
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
    return drupal_json_output($response);
  }
  return drupal_json_encode($response);
}


