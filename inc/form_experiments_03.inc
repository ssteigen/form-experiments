<?php

function form_experiments_03($form, &$form_state) {

  $form['ajax-wrapper'] = array(
    '#prefix' => '<div id="ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $form['ajax-wrapper']['uploaded-files'] = array(
    '#markup' => '',
  );

  $form['file'] = array(
    '#type' => 'file',
  );

  $form['upload'] = array(
    '#type' => 'submit',
    '#value' => 'upload',
    '#submit' => array('form_experiments_03_submit'),
    '#ajax' => array(
      'callback' => 'form_experiments_03_ajax_callback',
      'wrapper' => 'ajax-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}

function form_experiments_03_submit($form, &$form_state) {
  $file = file_save_upload('file', array('file_validate_extensions' => array('png gif jpg jpeg')), "public://", $replace = FILE_EXISTS_REPLACE);
  if ($file) {
    
    drupal_set_message('<pre>' . print_r($file, TRUE) . '</pre>');
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $form_state['files'][] = $file->filename;
  }
  else {
//    drupal_set_message('No file uploaded.');
  }
}

function form_experiments_03_ajax_callback($form, &$form_state) {
  if (isset($form_state['files'])) {
    $form['ajax-wrapper']['uploaded-files']['#markup'] = implode('<br/>', $form_state['files']);
  }

  return $form['ajax-wrapper'];
}


