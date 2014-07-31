<?php
/**
 * @file
 * Handles the creation/display of islandora:markupEditorCModel objects.
 */

/**
 * Return an objects datastreams xml processing instructions.
 *
 * @param AbstractObject $object
 *   The Fedora object to act on.
 * @param string $dsid
 *   The datastream ID to use as source content. Defaults to OBJ.
 *
 * @return boolean|array
 *   An indexed array of processing instuctions, or FALSE if not found.
 */
function islandora_markup_editor_fetch_tei_processing_instruction(AbstractObject $object, $dsid = 'OBJ') {
  if (!isset($object[$dsid])) {
    return FALSE;
  }
  $result = islandora_markup_editor_query_for_processing_instruction($object[$dsid]->content);
  if ($result->length == 0) {
    // No processing instruction, add the default.
    islandora_markup_editor_add_tei_processing_instruction($object, "", $dsid);
    $result = islandora_markup_editor_query_for_processing_instruction($object[$dsid]->content);
  }
  // Format the output for convience.
  $arr = explode(" ", $result->item(0)->data);
  $return = array();
  foreach ($arr as $key => $value) {
    $split_arr = explode("=", $value);
    $return[$split_arr[0]] = str_replace('"', '', $split_arr[1]);
  }
  return $return;
}

/**
 * Helper function to default drupal settings to a preprocess funciton.
 *
 * @param array $variables
 *   Call this function from a preprocess function, and pass the
 *   variables here.
 */
function islandora_markup_editor_add_markup_editor_drupal_settings($variables) {
  // Include required modules.
  module_load_include('inc', 'islandora_markup_editor', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'php_lib', 'DOMHelpers');

  // Construct our custom settings.
  $islandora_markup_editor_module_path = drupal_get_path('module', 'islandora_markup_editor');
  $view = islandora_object_access_callback(ISLANDORA_VIEW_OBJECTS, $variables['islandora_object']);
  $view_edit = islandora_object_access_callback('ISLANDORA_CRITICAL_EDITION_EDIT_WITH_CWRC_WRITER', $variables['islandora_object']);
  $validate_path = variable_get('islandora_markup_editor_validate_url', '/validator/validate.html');
  $doc_process_instructions = islandora_markup_editor_fetch_tei_processing_instruction($variables['islandora_object']);

  // This is silly. We need to override the default tei schema with our own.
  $schema_object = array();
  $schema_object['schemas']['tei'] = array(
    'cssUrl' => file_create_url("$islandora_markup_editor_module_path/CWRC-Writer/src/css/tei_converted.css"),
    'name' => 'tei',
    'url' => $doc_process_instructions['href'],
  );
  // The Critical Edition entry here is to temporarily support the old
  // dialogue boxes as they are still under development.
  drupal_add_js(
    array(
      'islandora_markup_editor' => array(
        'can_edit' => $view_edit,
        'can_view' => $view,
        'module_edit_base' => $islandora_markup_editor_module_path,
        'entities_search_callback' => url('islandora/entities/search'),
        'schema_object' => $schema_object,
        'page_setup' => "islandora/markupeditor/setup/",
        'page_pid' => $variables['islandora_object']->id,
        'validate_path' => $validate_path,
        'schema_pref' => array('valid' => 0),
      ),
    ),
    'setting'
  );
}

/**
 * Query a FedoraObjects datastream for an xml processor instruction.
 *
 * @param string $xml
 *   The xml string to query for processing instruction.
 *
 * @return string
 *   The processing instruction as a string.
 */
function islandora_markup_editor_query_for_processing_instruction($xml) {
  $xml_data = $xml;
  $src_doc = new DOMDocument();
  $src_doc->preserveWhiteSpace = FALSE;
  $src_doc->formatOutput = TRUE;
  @$src_doc->loadXML($xml_data);

  $xpath = new DOMXPath($src_doc);
  $e = $xpath->query('//processing-instruction(\'xml-model\')');
  return $e;
}

/**
 * Load js/css files in a given directory for the cwrc editor.
 *
 * @param string $direc
 *   The directory the files reside in.
 * @param string $file_type
 *   For now, either .js or .css.
 */
function islandora_markup_editor_add_files_from_dir($direc, $file_type = 'js') {
  $function = "drupal_add_" . $file_type;
  if (!function_exists($function)) {
    return FALSE;
  }
  $dir = new DirectoryIterator($direc);
  foreach ($dir as $fileinfo) {
    if (!$fileinfo->isDot() && $fileinfo->isFile() && ($fileinfo->getExtension() == $file_type)) {
      $file_name = $fileinfo->getFilename();
      $function($direc . "/$file_name");
    }
  }
}

/**
 * Adds schema processing instructions to a given objects requested datastream.
 *
 * @param AbstractObject $object
 *   The fedora object to act upon.
 * @param string $schema
 *   The accessable url to a relaxng .rng schema
 * @param string $dsid
 *   The dsid to add instructions to. Defaults to OBJ.
 *
 * @return string
 *   The processing instruction as it appears in the given datastream.
 */
function islandora_markup_editor_add_tei_processing_instruction(AbstractObject $object, $schema = "", $dsid = 'OBJ') {
  if ($schema == "") {
    // Use the default schema if none is supplied.
    $schema = islandora_markup_editor_create_schema_rels_url($object);
  }
  $xml_data = $object[$dsid]->content;
  $instructions = array(
    'href' => $schema,
    'type' => 'application/xml',
    'schematypens' => 'http://relaxng.org/ns/structure/1.0',
  );

  $tei_doc = new DOMDocument();
  @$tei_doc->loadXML($xml_data);

  $processing_string = "";
  foreach ($instructions as $key => $value) {
    $processing_string .= $key . '="' . $value . '" ';
  }
  $trimmed = trim($processing_string);

  $src_doc = new DOMDocument("1.0", "UTF-8");
  $src_doc->preserveWhiteSpace = FALSE;
  $src_doc->formatOutput = TRUE;
  $rng_schema = $src_doc->createProcessingInstruction('xml-model', $trimmed);
  $src_doc->appendChild($rng_schema);
  $node = $tei_doc->getElementsByTagName('TEI')->item(0);
  $tei_data = $src_doc->importNode($node, TRUE);
  $src_doc->appendChild($tei_data);
  $data = $src_doc->saveXML();
  $object[$dsid]->setContentFromString($data);

  return $trimmed;
}

/**
 * Add a relationship to an object of the supplied TEI schema.
 *
 * @param AbstractObject $object
 *   The islandora object to add the schema relation to.
 * @param string $schema_pid
 *   The schema pid to add as a relation. Defaults to
 *   'islandora:tei_sample_schema'.
 *
 * @return mixed
 *   RELS.
 */
function islandora_markup_editor_process_schema_relationship(AbstractObject $object, $schema_pid = 'islandora:tei_sample_schema') {
  // Check if the object has a 'hasTEISchema' relationship.
  $rels = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'hasTEISchema');
  if (!$rels) {
    // Rels does not exist, add the default schema.
    $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'hasTEISchema', $schema_pid);
    $rels = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'hasTEISchema');
  }
  return $rels;
}

/**
 * Creates a URL for the given objects RELS-EXT 'hasTEISchema' entry.
 *
 * @param AbstractObject $object
 *   A fedora object to process sechema rels as url.
 */
function islandora_markup_editor_create_schema_rels_url(AbstractObject $object) {
  global $base_url;
  $module_path = drupal_get_path('module', 'islandora_markup_editor');
  $rel = islandora_markup_editor_process_schema_relationship($object);
  $rel_pid = $rel[0]['object']['value'];
  $schema = islandora_object_load($rel_pid);
  $rdf_datastream = $schema['RDF-SCHEMA'];
  // url() function wont suffice for the CWRC-Writer.
  $url = $base_url . "/islandora/object/$rel_pid/datastream/RDF-SCHEMA/view/$rdf_datastream->label";
  return $url;
}
