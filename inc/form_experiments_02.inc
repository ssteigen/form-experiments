<?php

function form_experiments_02($form, &$form_state) {

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
    '#submit' => array('form_experiments_02_ajax_callback'),
    '#ajax' => array(
      'callback' => 'form_experiments_02_ajax_callback',
      'wrapper' => 'ajax-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}

function form_experiments_02_ajax_callback($form, $form_state) {
  $file = file_save_upload('file', array('file_validate_extensions' => array('png gif jpg jpeg')), "public://", $replace = FILE_EXISTS_REPLACE);

  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    $form['ajax-wrapper']['uploaded-files']['#markup'] = '<p>Preview:</p><img src="/sites/default/files/' . $file->filename . '" style="max-width: 250px; max-height: 250px;" />';
  }
  else {
    drupal_set_message('No file uploaded.');
  }

  return $form['ajax-wrapper'];
}
