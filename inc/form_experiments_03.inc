<?php

function form_experiments_03($form, &$form_state) {

  $form['summary'] = array(
    '#markup' => 'Multi-File Upload with Thumbnails',
    '#prefix' => '<p class="lead">',
    '#suffix' => '</p>',
  );

  $form['description']['#markup'] = '<p>Single file field supports uploading several files</p>';
  $form['description']['#markup'] .= '<p>Renders uploaded files in the ajax callback (not best practice?)</p>';

  $form['ajax_wrapper'] = array(
    '#prefix' => '<div id="ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $form['ajax_wrapper']['uploaded-files'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('row')),
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
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $form_state['files'][] = $file;
  }
  else {
    drupal_set_message('No file uploaded.');
  }
}

function form_experiments_03_ajax_callback($form, &$form_state) {
  if (isset($form_state['files'])) {
    foreach ($form_state['files'] as $file) {
      $form['ajax_wrapper']['uploaded-files'][$file->fid] = array(
        '#markup' => '
        <div class="thumbnail">
          <img class="img-responsive" src="/sites/default/files/' . $file->filename . '" />
          <div class="caption"><p>' . $file->filename  . '</p></div>
        </div>
      ',
        '#prefix' => '<div class="col-xs-6 col-sm-4">',
        '#suffix' => '</div>',
      );
    }
  }

  return $form['ajax_wrapper'];
}


