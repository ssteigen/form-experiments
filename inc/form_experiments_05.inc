<?php
function form_experiments_05($form, &$form_state) {
  //$form['#cache'] = FALSE; // Is this required?

  $form['summary'] = array(
    '#markup' => 'Multi-File Upload with Thumbnails',
    '#prefix' => '<p class="lead">',
    '#suffix' => '</p>',
  );

  $form['description']['#markup'] = '<p>Single file field supports uploading several files</p>';
  $form['description']['#markup'] .= '<p>Requires rebuild = TRUE</p>';
  $form['description']['#markup'] .= '<p>Renders uploaded files when building the form</p>';
  $form['description']['#markup'] .= '<p>Image caption contains link to full-size image and button to delete image</p>';

  $form['ajax_wrapper'] = array(
    '#type' => 'container',
    '#tree' => FALSE,
    '#attributes' => array('id' => 'ajax-wrapper'),
  );

  $files = isset($form_state['files']) ? $form_state['files'] : array();

  $form['ajax_wrapper']['files'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('row')),
  );

  foreach ($files as $file) {
    $form['ajax_wrapper']['files'][$file->fid] = array(
      '#type' => 'container',
      '#tree' => TRUE,
      '#attributes' => array('class' => array('col-xs-6', 'col-sm-4')),
    );

    $form['ajax_wrapper']['files'][$file->fid]['thumbnail'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('thumbnail')),
    );

    $form['ajax_wrapper']['files'][$file->fid]['thumbnail']['image'] = array(
      '#markup' => '<img class="img-responsive" src="/sites/default/files/' . $file->filename . '" />',
    );

    $form['ajax_wrapper']['files'][$file->fid]['thumbnail']['caption'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('caption')),
    );

    $form['ajax_wrapper']['files'][$file->fid]['thumbnail']['caption']['filename'] = array(
      '#markup' => '<a href="/sites/default/files/' . $file->filename . '">' . $file->filename  . '</a>',
    );

    $form['ajax_wrapper']['files'][$file->fid]['thumbnail']['caption']['button'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('form_experiments_05_remove_file'),
      '#attributes' => array('class' => array('pull-right', 'btn-xs')),
      '#ajax' => array(
        'callback' => 'form_experiments_05_remove_file_callback',
        'wrapper' => 'ajax-wrapper',
      ),
      '#name' => 'remove_file_' . $file->fid,
    );
  }

  $form['ajax_wrapper']['file_upload'] = array(
    '#type' => 'fieldset',
  );

  $form['ajax_wrapper']['file_upload']['file'] = array(
    '#type' => 'file',
  );

  $form['ajax_wrapper']['file_upload']['button'] = array(
    '#type' => 'submit',
    '#value' => t('Add File'),
    '#submit' => array('form_experiments_05_add_file'),
    '#ajax' => array(
      'callback' => 'form_experiments_05_add_file_callback',
      'wrapper' => 'ajax-wrapper',
    ),
  );

  return $form;
}

function form_experiments_05_remove_file($form, &$form_state) {
  $fid = $form_state['triggering_element']['#parents'][0];
  $file = $form_state['files'][$fid];

  if (file_delete($file)) {
    unset($form_state['files'][$fid]);
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message('The file could not be deleted.', 'error');
  }
}

function form_experiments_05_remove_file_callback($form, &$form_state) {
  return $form['ajax_wrapper'];
}

function form_experiments_05_add_file($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('png gif jpg jpeg'));
  $file = file_save_upload('file', $validators);

  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    $form_state['files'][$file->fid] = $file;

    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message('No file uploaded.');
  }
}

function form_experiments_05_add_file_callback($form, &$form_state) {
  return $form['ajax_wrapper'];
}
