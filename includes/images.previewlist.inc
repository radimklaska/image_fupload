<?php
// $Id$

/*
* Provide a list (queue) of uploaded images; all captions can be edited at the same time
*/
function fupload_list_images(&$form_state, $node_type) {
  global $user;
  $count = 0;
  
  $result = db_query("SELECT nid, title FROM {node} WHERE type = '%s' AND status = '0' AND created > %d", $node_type, time() - 86400); // only entries which are one day or newer
  while ($node = db_fetch_object($result)) {
    $count += 1;
    // Get all node infos, also image paths
    $node_image = node_load($node->nid);
    
    // only use this image node if it was produced by image_fupload
    if ($node_image->body == IMAGE_NOT_COMPLETED) {
      $image_nodes[] = $node_image->nid;
    
      $form[$node_image->nid] = array(
        '#type' => 'fieldset',
        '#title' => t('Image @count', array('@count' => $count)),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
      );
      $form[$node_image->nid]['preview_image'] = array(
        '#prefix' => '<div class="image_fupload_preview">',
        '#value' => _fupload_imagepreview($node_image, $node_type),
        '#post' => '</div>',
        '#weight' => 1,        
      );      
      $form[$node_image->nid]['title_' .$node_image->nid] = array(
        '#type' => 'textfield',
        '#title' => t('Image title'),
        '#default_value' => isset($form_state['values']['title_' .$node_image->nid]) ? $form_state['values']['title_' .$node_image->nid] : $node_image->title,
        '#size' => 60,
        '#required' => TRUE,
        '#weight' => 3,
      );
      $form[$node_image->nid]['body_' .$node_image->nid] = array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => isset($form_state['values']['body_' .$node_image->nid]) ? $form_state['values']['body_' .$node_image->nid] : '',
        '#rows' => 5, 
        '#weight' => 5,
      );
      $form[$node_image->nid]['format_' .$node_image->nid] = filter_form($node_image->format, NULL, array('format_' .$node_image->nid));
      $form[$node_image->nid]['format_' .$node_image->nid]['#weight'] = 6;
    }
  }
  
  if (!empty($image_nodes)) {
    // are there any images to edit?
    $form['text'] = array('#value' => t('In this step, you can edit the captions of all uploaded images. To complete this task, click the button "Done Editing" at the bottom of this page.'), '#weight' => -19);
    $form['#redirect'] = 'node/add/' .$node_type; // redirect to first upload page again
    $form['image_nodes'] = array('#type' => 'value', '#value' => implode(';', $image_nodes));
    $form['submit'] = array('#type' => 'submit', '#value' => t('Done Editing'));
  } else {
    // no images in queue
    $form['text'] = array('#value' => t('No images yet in queue.'), '#weight' => -19);
    drupal_set_message(t('No images have been found in queue, probably no images have been uploaded yet. Please return to !upload_page if you want to upload some images.', array('!upload_page' => l(t('image upload page'), 'node/add/' .$node_type))), 'warning');
  }
  
  // variable_set('image_node_types', array('image' => array('title' => 'Image', 'fieldname' => 'images', 'image_selection' => 'fupload_preview', 'imagecache_preset' => 'fupload_preview', 'image_dimension' => '127x200')));  
  return $form;
}

function fupload_list_images_submit($form, &$form_state) {
  // get nids of images and start batch process
  $image_nids = explode(';', $form_state['values']['image_nodes']);  
  for ($i = 0; $i < count($image_nids); $i++) { 
    // load full node object  
    $node = node_load($image_nids[$i]);    
    // new changes to node object
    $node->status = 1; // publish node
    $node->title = check_plain($form_state['values']['title_' .$image_nids[$i]]);
    $node->teaser = node_teaser($form_state['values']['body_' .$image_nids[$i]], $form_state['values']['format_' .$image_nids[$i]]);
    $node->body = $form_state['values']['body_' .$image_nids[$i]];
    $node->format = $form_state['values']['format_' .$image_nids[$i]];    
    // save new node object
    node_save($node);
  }
  drupal_set_message(t('All images have been saved and published.'));
  drupal_set_message(t('Additonal images can be selected and uploaded now.'));
  drupal_redirect_form($form);  
}