<?php

function form_experiments_02($form, &$form_state) {

  $form['summary'] = array(
    '#markup' => 'Multi-File Upload with Thumbnails',
    '#prefix' => '<p class="lead">',
    '#suffix' => '</p>',
  );

  $form['description']['#markup'] = '<p>Single file field supports uploading several files</p>';
  $form['description']['#markup'] .= '<p>Requires rebuild = TRUE</p>';
  $form['description']['#markup'] .= '<p>Renders uploaded files when building the form</p>';

  $form['ajax_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'ajax-wrapper'),
  );

  $form['ajax_wrapper']['uploaded_files'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('row')),
  );

  $form_state['files'] = (isset($form_state['files']) ? $form_state['files'] : array());

  foreach ($form_state['files'] as $file) {
    $form['ajax_wrapper']['uploaded_files'][$file->fid] = array(
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

  $form['file_upload'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
  );

  $form['file_upload']['file'] = array(
    '#type' => 'file',
  );

  $form['file_upload']['button'] = array(
    '#type' => 'submit',
    '#value' => 'Upload',
    '#submit' => array('form_experiments_02_ajax_callback'),
    '#ajax' => array(
      'callback' => 'form_experiments_02_ajax_callback',
      'wrapper' => 'ajax-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  /** FORM ACTIONS **/

  $form['form-actions'] = array(
    '#type' => 'actions',
  );
  $form['form-actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );
  $form['form-actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function form_experiments_02_ajax_callback($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('png gif jpg jpeg'));
  $file = file_save_upload('file', $validators, "public://", FILE_EXISTS_REPLACE);

  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    $form_state['files'][] = $file;

    $form_state['rebuild'] = TRUE;
  }

  return $form['ajax_wrapper'];
}
