<?php

/**
 * @file
 * Handles the uploading of the PDF file as part of the ingest process.
 */

/**
 * Defines a file upload form for uploading the tei file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_markup_editor_tei_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('xml');
  $form = array();
  $file_size = array($upload_size * 1024 * 1024);
  $form['file'] = array(
    '#title' => t('TEI File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      // Assume it's specified in MB.
      'file_validate_size' => $file_size,
    ),
  );
  $form['rng_schema'] = array(
    '#type' => 'textfield',
    '#title' => t('RNG Schema URL'),
    '#size' => 60,
    '#required' => FALSE,
  );
  $form['islandora_markup_editor_text_upload'] = array(
    '#type' => 'checkbox',
    '#title' => t("Add text file to this upload?"),
    '#default_value' => FALSE,
  );
  $form['text'] = array(
    '#title' => t('Plain text transcription'),
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#description' => t('Select text file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b><br />This file is optional.', array('@size' => $upload_size, '@ext' => 'txt')),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('txt'),
      // Assume it's specified in MB.
      'file_validate_size' => $file_size,
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_markup_editor_text_upload"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['islandora_markup_editor_tn_upload'] = array(
    '#type' => 'checkbox',
    '#title' => t("Add optional thumbnail?"),
    '#default_value' => FALSE,
  );
  $form['tn'] = array(
    '#title' => t('Thumbnail for object'),
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#description' => t('Select text file to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong><br />This file is optional.', array('@size' => $upload_size, '@ext' => 'jpg, jpeg, png')),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('jpg jpeg png'),
      // Assume it's specified in MB.
      'file_validate_size' => $file_size,
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_markup_editor_tn_upload"]' => array('checked' => TRUE),
      ),
    ),
  );

  return $form;
}

/**
 * Addes the uploaded file into the ingestable objects 'OBJ' datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_markup_editor_tei_upload_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_markup_editor', 'includes/utilities');

  $object = $form_state['islandora']['objects'][0];
  if (empty($object['OBJ'])) {
    $ds = $object->constructDatastream('OBJ', 'M');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['OBJ'];
  }
  $tei_file = file_load($form_state['values']['file']);
  $tei_path = drupal_realpath($tei_file->uri);
  $ds->label = $tei_file->filename;
  // mimetype assumes only xml file extensions and "readible" xml
  $ds->mimetype = "text/xml";
  $ds->setContentFromFile($tei_path, FALSE);


  $result = islandora_markup_editor_query_for_processing_instruction($object['OBJ']->content);
  if ($result->length == 0) {
    // No processing instruction, add the default.
    // Add the schema as a <?xml-model processing instruction in the
    //tei stream.
    islandora_markup_editor_add_tei_processing_instruction($object, $form_state['values']['rng_schema']);
  }


  if (isset($form_state['values']['text']) && ($form_state['values']['text'] > 0)) {
    if (empty($object['FULL_TEXT'])) {
      $ds = $object->constructDatastream('FULL_TEXT', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['FULL_TEXT'];
    }
    $text_file = file_load($form_state['values']['text']);
    $text_path = drupal_realpath($text_file->uri);
    $ds->setContentFromFile($text_path, FALSE);
    $ds->label = $text_file->filename;
    $ds->mimetype = $text_file->filemime;
  }

  if ($form_state['values']['tn']) {
    $thumbnail_file = file_load($form_state['values']['tn']);
    islandora_scale_thumbnail($thumbnail_file, 200, 200);
    if (empty($object['TN'])) {
      $tn = $object->constructDatastream('TN', 'M');
      $object->ingestDatastream($tn);
    }
    else {
      $tn = $object['TN'];
    }
    $tn->setContentFromFile(drupal_realpath($thumbnail_file->uri), FALSE);
    if ($tn->label != $thumbnail_file->filename) {
      $tn->label = $thumbnail_file->filename;
    }
    if ($tn->mimetype != $thumbnail_file->filemime) {
      $tn->mimetype = $thumbnail_file->filemime;
    }
  }
}
