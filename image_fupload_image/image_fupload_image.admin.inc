<?php
// $Id$

function image_fupload_image_admin() {
  $form['fupload_title_replacements'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Title Processor'),
    '#description' => t('All entered elements which have to be separated by a semicolon (";"), are replaced by a whitespace when the node title is created out of the original image filename.')
      .'<p>'. t('<em>Note:</em> The theme function "fupload_create_filename" can be overwritten to provide a customised title creation.') .'</p>',
    '#default_value' => variable_get('fupload_title_replacements', '_;{;}'),
    '#weight' => -5,
    '#required' => TRUE,
  );
  
  return system_settings_form($form);
}