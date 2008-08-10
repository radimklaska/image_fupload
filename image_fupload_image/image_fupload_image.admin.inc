<?php
// $Id$

function image_fupload_image_admin() {
  $form['#submit'][] = 'image_fupload_image_admin_submit';
  $form['previewlist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images preview list'),
    '#description' => t('This feature adds the ability to show a list of all images after having uploaded them. At the same time, all title and body fields can be edited all at once.<br /> In order to use it, it has to be activated separately per !admin-permissions.', array('!admin-permissions' => l(t('user role'), 'admin/user/permissions'))),
    '#collapsible' => TRUE,
    '#weight' => -10,
  );
  $form['previewlist']['fupload_previewlist_img'] = array(
    '#type' => 'select',
    '#title' => t('Preview Image Preset'),
    '#description' => t('This setting is responsible for the way of displaying and handling the preview image which is generated out of the original image. Between different image sizes can be chosen.<br /> If <em>imagecache module</em> is installed and enabled, also its presets (if available) can be used to display the preview image.'),
    '#options' => _fupload_imagepreview_settings('list', 'image'),
    '#default_value' => _fupload_imagepreview_settings('read', 'image'),
    '#required' => TRUE,
  );
  $form['previewlist']['fupload_previewlist_img_attributes'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Attributes'),
    '#description' => t('Provide some additional attributes to be integrated in image (preview) tag.'),
    '#default_value' => variable_get('fupload_previewlist_img_attributes', ''),
  );
  $form['previewlist']['field_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['previewlist']['field_settings']['fupload_previewlist_field_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Editable fields'),
    '#description' => t('Choose the fields which should be editable by the uploader.'),
    '#options' => array(
      'title' => t('Title'),
      'body' => t('Body'),    
    ),
    '#default_value' => variable_get('fupload_previewlist_field_settings', array('title', 'body')),
  );
  
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

function image_fupload_image_admin_submit($form, &$form_state) {
  // prepare some form data to be written into database  
  $preview_preset = explode('_', $form_state['values']['fupload_previewlist_image'], 2);
  
  
  switch($preview_preset[0]) {
    case '0':    
      // image module preset
      _fupload_imagepreview_settings('write', 'image', array('image_selection' => $preview_preset[1]));      
      break;
    
    case '1':
      // imagecache module preset
      _fupload_imagepreview_settings('write', 'image', array('imagecache_preset' => $preview_preset[1]));
      break;    
  }
}

function _fupload_imagepreview_settings($op, $node_type, $data_write = NULL) {
  // switch to the right operation: what's up?
  switch($op) {
    case 'list':
      // generate a list of available presets for image_preview: image module or/and imagecache
      $preview_presets_list = array();
      
      // image module presets
      if (module_exists('image') && $node_type == "image") {
        $image_sizes = image_get_sizes();
        
        foreach ($image_sizes as $key => $size) {
          $preview_presets_list['image']['0_' .$key] = t('Image size') .': ' .$size['label'];
        }
      }
      
      // imagecache module presets
      if (module_exists('imagecache')) {
        $image_sizes = imagecache_presets();
        
        foreach ($image_sizes as $key => $size) {
          $preview_presets_list['imagecache']['1_' .$size['presetname']] = t('Image size') .': ' .$size['presetname'];
        }
      }
      
      //output list
      return $preview_presets_list;
      
      break;   
      
    case 'read':
      // read actual setting for this node type
      $image_node_types = variable_get('image_node_types', array());
      
      switch($node_type) {
        case 'image':
          // image node type
          if (!empty($image_node_types['image']['image_selection'])) {
            // image module preset
            return '0_' .$image_node_types['image']['image_selection'];
          
          } elseif (!empty($image_node_types['image']['imagecache_preset'])) {
            // imagecache module preset
            return '1_' .$image_node_types['image']['imagecache_preset'];
          
          } else {
            // no setting saved yet?
            return '';
          }
          break;
        default:
          // node type with CCK ImageField
          break;
      }
      break;
      
    case 'write':
      if (isset($data_write)) { // is some data available?
      // write input data to database
      $image_node_types  = variable_get('image_node_types', array());
        switch($node_type) {
          case 'image':
            // image node type
            $image_node_types['image'] = array('title' => 'Image', 'fieldname' => 'images', 'image_selection' => $data_write['image_selection'], 'imagecache_preset' => $data_write['imagecache_preset']);          
            break;
          
          default:
            // node type with CCK ImageField
            break;
        }
      // write data to database
      variable_set('image_node_types', $image_node_types);
      }
  }
}