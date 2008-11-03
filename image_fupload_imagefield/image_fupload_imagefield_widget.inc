<?php

/**
 * @file
 * Image Fupload ImageField widget hooks and callbacks.
 *
 * This file is loaded by filefield.module/imagefield.module and the callbacks it contains are
 * invoked by filefield.module. This happens because imagefield delegates it's
 * CCK hooks and callbacks to filefield in imagefield.module.
 *
 * This file is mainly based on imagefield_widget.inc (D6 3.0-alpha2)
 */
 
module_load_include('inc', 'imagefield', 'imagefield_widget');

/**
 * @defgroup "FileField widget settings callbacks"
 * @{
 *
 * These callbacks are invoked by filefield_widget_settings. Their
 * return values are merged with and will override filefields' widget
 * settings callback's return values.
 */
function image_fupload_imagefield_widget_widget_settings_form($widget) {
  $form = array();
  $form = imagefield_widget_widget_settings_form($widget);
  
  $form['fupload_previewlist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images preview list'),
    '#description' => t('This feature adds the ability to show a list of all images after having uploaded them. At the same time, all title and body fields can be edited all at once.<br /> In order to use it, it has to be activated separately per !admin-permissions.', array('!admin-permissions' => l(t('user role'), 'admin/user/permissions'))),
    '#collapsible' => TRUE,
    '#weight' => 3,
  );
  $form['fupload_previewlist']['fupload_previewlist_img'] = array(
    '#type' => 'select',
    '#title' => t('Preview Image Preset'),
    '#description' => t('This setting is responsible for the way of displaying and handling the preview image which is generated out of the original image. Between different image sizes can be chosen.<br /> If <em>imagecache module</em> is installed and enabled, also its presets (if available) can be used to display the preview image.'),
    '#options' => _fupload_imagepreview_settings('list', str_replace("-", "_", check_plain(arg(3)))), // @str_replace: returns correct node type
    '#default_value' => _fupload_imagepreview_settings('read', 'image'),
    '#required' => TRUE,
  );
  $form['fupload_previewlist']['fupload_previewlist_img_attributes'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Attributes'),
    '#description' => t('Provide some additional attributes to be integrated in image (preview) tag, for example: class="my_custom_css"'),
    '#default_value' => !empty($widget['img_attributes']) ? $widget['img_attributes'] : '',
  );
   $form['fupload_previewlist']['field_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['fupload_previewlist']['field_settings']['fupload_previewlist_field_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Editable fields'),
    '#description' => t('Choose the fields which should be editable by the uploader.') .'<p>'.t('<strong>Important information:</strong><br /> If the body is not editable by the user, <em>"minimum number of words"</em> !setting for the body field has to be <strong>0</strong>.', array('!setting' => l(t('setting'), 'admin/content/node-type/image'))).'</p>',
    '#options' => array(
      'description' => t('Description'),   
    ),
    '#default_value' => variable_get('fupload_previewlist_field_settings', array('title', 'body')),
  );
  
  drupal_set_message(t('Blblas'), 'warning');
  print_r($widget);
  
  return $form;
}

function image_fupload_imagefield_widget_widget_settings_save($widget) {
  // elements which should be saved for this field (settings)
  //$settings = array('');
  return array('max_resolution', 'min_resolution', 'alt',  'custom_alt', 'title', 'custom_title');
}

/**
 * @} End defgroup "Filefield widget settings callbacks."
 */

/**
 * @defgroup "FileField widget element callbacks.
 * @{
 *
 * The call backs are called by form elements that leverage the
 * filefield_widget_value and filefield_widget_process callbacks.
 * They will be called after the filefield callbacks and their
 * return values will be merged with the filefield callback's.
 */
function image_fupload_imagefield_widget_widget_value($element, $edit = FALSE) {
  // handle additional
  if ($edit) {
      return array(
        'alt' => isset($edit['alt']) ? $edit['alt'] : '',
        'title' => isset($edit['title']) ? $edit['title'] : '',
      );
  }
  return array('alt' => '', 'title' => '');
}

function image_fupload_imagefield_widget_widget_process($element, $edit, &$form_state, $form) {
  $file = $element['#value'];
  $field = content_fields($element['#field_name'], $element['#type_name']);

  $element['data']['alt'] = array(
    '#title' => t('Alternate Text'),
    '#type' => 'value',
    '#default_value' => $field['widget']['alt'],
  );
  if ($field['widget']['custom_alt']) {
    $element['data']['alt']['#type'] = $file['fid'] ? 'textfield' : 'hidden';
    if (!empty($file['data']['alt'])) $element['data']['alt']['#default_value'] =  $file['data']['alt'];
  }

  $element['data']['title'] = array(
    '#type' => 'value',
    '#title' => t('Title'),
    '#default_value' => $field['widget']['title'],
  );
  if ($field['widget']['custom_title']) {
    $element['data']['title']['#type'] = $file['fid'] ? 'textfield' : 'hidden';
    if (!empty($file['data']['title'])) $element['data']['title']['#value'] =  $file['data']['title'];
  }
  return $element;
}

function image_fupload_imagefield_widget_widget_validate($element) {
}

/**
 * @} End defgroup "FileField widget element callbacks."
 */
 
 /**
 * FormAPI theme function. Theme the output of an image field.
 */
function theme_image_fupload_imagefield_widget(&$element) {
  return theme('form_element', $element, $element['#children']);
}