<?php
/**
 * @file
 * Functionality to support form experiment 06.
 */

/**
 * Form definition.
 */
function form_experiments_06($form, &$form_state) {
  $form['#cache'] = false;

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'form_experiments') . '/js/form_experiments.js',
  );

  $form['summary'] = array(
    '#markup' => 'Multi-File Upload List',
    '#prefix' => '<p class="lead">',
    '#suffix' => '</p>',
  );

  $form['description']['#markup'] = '<p>Single file field supports uploading several files</p>';
  //$form['description']['#markup'] .= '<p>Requires rebuild = TRUE</p>';
  //$form['description']['#markup'] .= '<p>Renders uploaded files when building the form</p>';
  //$form['description']['#markup'] .= '<p>Image caption contains link to full-size image and button to delete image</p>';


  $form['notes'] = array(
    '#markup' => '
      <p>To-do:
        <ul>
          <li>Limit to three files</li>
          <li>Limit filesize to 6MB</li>
          <li>Supported file types: doc docx gif jpeg jpg odt pdf png tif tiff</li>
          <li>Implement delete confirmation</li>
        </ul>
      </p>
    ',
  );

  $form['ajax_wrapper'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array('id' => 'ajax-wrapper'),
    '#title' => t('File Upload'),
  );

  /** UPLOADED FILES **/

  $form_state['files'] = isset($form_state['files']) ? $form_state['files'] : array();
  foreach ($form_state['files'] as $file) {
    $form['ajax_wrapper'][$file->fid] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );

    $form['ajax_wrapper'][$file->fid]['filename'] = array(
      '#markup' => '<a target="_blank" href="/sites/default/files/' . $file->filename . '">' . $file->filename  . '</a>',
    );

    $form['ajax_wrapper'][$file->fid]['remove_file'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#name' => 'remove_file_' . $file->fid,
      '#submit' => array('form_experiments_06_remove'),
      '#ajax' => array(
        'callback' => 'form_experiments_06_remove_callback',
        'wrapper' => 'ajax-wrapper',
      ),
      '#attributes' => array('class' => array('pull-right', 'btn-xs')),
    );
  }

  /** ADD A FILE **/

  $form['ajax_wrapper']['file_upload'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#attributes' => array('class' => array('auto-upload')),
  );
  $form['ajax_wrapper']['file_upload']['file'] = array(
    '#type' => 'file',
  );
  $form['ajax_wrapper']['file_upload']['upload'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
    '#submit' => array('form_experiments_06_upload'),
    '#ajax' => array(
      'callback' => 'form_experiments_06_upload_callback',
      'wrapper' => 'ajax-wrapper',
    ),
  );

  /** FORM ACTIONS **/

  $form['form-actions'] = array(
    '#type' => 'actions',
  );
  $form['form-actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('form_experiments_06_cancel'),
  );
  $form['form-actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('form_experiments_06_submit'),
  );

  return $form;
}

function form_experiments_06_remove($form, &$form_state) {
  $fid = $form_state['triggering_element']['#parents'][1];
  $file = $form_state['files'][$fid];

  if (file_delete($file)) {
    unset($form_state['files'][$fid]);
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message('File could not be deleted.');
  }
}

function form_experiments_06_remove_callback($form, &$form_state) {
  return $form['ajax_wrapper'];
}

function form_experiments_06_upload($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('png gif jpg jpeg'));
  $file = file_save_upload('file', $validators, "public://", FILE_EXISTS_REPLACE);

  if ($file) {
    $form_state['files'][$file->fid] = $file;

    // Only rebuild the form when a file passes validation and is uploaded correctly.
    $form_state['rebuild'] = TRUE;
  }
}

function form_experiments_06_upload_callback($form, &$form_state) {
  return $form['ajax_wrapper'];
}

function form_experiments_06_submit($form, &$form_state) {
  drupal_goto('/');
}

function form_experiments_06_cancel($form, &$form_state) {
  drupal_goto('/');
}
