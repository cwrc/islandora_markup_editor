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
function islandora_markup_editor_oa_hasTarget($hasTarget_uri) {

  try {
    //$result_array = array("success"=> urldecode($hasTarget_uri).urlencode("a/a") );
    
    $result_array = array( 
'
<rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:cw="http://cwrc.ca/ns/cw#" xmlns:oa="http://www.w3.org/ns/oa#" xmlns:foaf="http://xmlns.com/foaf/0.1/">
    <rdf:Description rdf:about="http://apps.testing.cwrc.ca/editor/documents/null">
        <cw:mode>0</cw:mode>
    </rdf:Description>
    <rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/annotation/6a82d26e-b059-4e25-88c0-35fd00085709">
        <oa:hasTarget xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/target/1e7c9ad5-358f-4c7f-90e3-0515a05b5558"/>
        <oa:hasBody xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/person/56c9f7fe-d0ef-4c18-9563-e26272f9ee29"/>
        <oa:annotatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource=""/>
        <oa:annotatedAt xmlns:oa="http://www.w3.org/ns/oa#">2014-10-03T22:56:06.440Z</oa:annotatedAt>
        <oa:serializedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource=""/>
        <oa:serializedAt xmlns:oa="http://www.w3.org/ns/oa#">2014-10-03T22:56:06.440Z</oa:serializedAt>
        <rdf:type rdf:resource="http://www.w3.org/ns/oa#Annotation"/>
        <oa:motivatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://www.w3.org/ns/oa#tagging"/>
        <oa:motivatedBy xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://www.w3.org/ns/oa#identifying"/>
        <cw:hasCertainty xmlns:cw="http://cwrc.ca/ns/cw#" rdf:resource="http://cwrc.ca/ns/cw#reasonable"/>
        <cw:cwrcInfo xmlns:cw="http://cwrc.ca/ns/cw#">{"id":"42172560","name":"Houde, Camillien","repository":"viaf"}</cw:cwrcInfo>
        <cw:cwrcAttributes xmlns:cw="http://cwrc.ca/ns/cw#">{"cert":"reasonably certain","ref":"42172560"}</cw:cwrcAttributes>
    </rdf:Description>
    <rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/person/56c9f7fe-d0ef-4c18-9563-e26272f9ee29">
        <rdf:type rdf:resource="http://www.w3.org/ns/oa#SemanticTag"/>
        <rdf:type rdf:resource="http://xmlns.com/foaf/0.1/Person"/>
    </rdf:Description>
    <rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/target/1e7c9ad5-358f-4c7f-90e3-0515a05b5558">
        <oa:hasSource xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/doc/690e168d-a5fc-4339-9772-66fd5b2cc315"/>
        <rdf:type rdf:resource="http://www.w3.org/ns/oa#SpecificResource"/>
        <oa:hasSelector xmlns:oa="http://www.w3.org/ns/oa#" rdf:resource="http://id.cwrc.ca/selector/e2c48327-c2cf-4731-ae6d-6ba0f352c546"/>
    </rdf:Description>
    <rdf:Description xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" rdf:about="http://id.cwrc.ca/selector/e2c48327-c2cf-4731-ae6d-6ba0f352c546">
        <rdf:value>xpointer(//persName[@annotationId="ent_88"])</rdf:value>
        <rdf:type rdf:resource="http://www.w3.org/ns/oa#FragmentSelector"/>
    </rdf:Description>
</rdf:RDF>
'
      ); 

    $cwrc = array(
      "hasTarget" => urldecode($hasTarget_uri) 
      , "numFound" => "0"
      , "page" => array("start" => "0", "rows" => "5" )
      , "oa_items" => $result_array
      );
    drupal_json_output(
      array(
        "response" => $cwrc 
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
