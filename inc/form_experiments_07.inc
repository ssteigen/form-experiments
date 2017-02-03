<?php
/**
 * @file
 * Functionality to support Form 07.
 */

/**
 * Builds Form 07.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return mixed
 *   The renderable form array.
 */
function form_experiments_07($form, &$form_state) {
  $form['#cache'] = FALSE;

  /** UPLOADED FILES **/

  $form['uploaded_docs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Supporting Documentation'),
    '#description' => t('(Up to 3 files) File size is limited to a maximum of 6MB. File types supported: doc docx gif jpeg jpg odt pdf png tif tiff.'),
    '#tree' => FALSE,
    '#attributes' => array('id' => 'ajax-wrapper')
  );

  $files = isset($form_state['files']) ? $form_state['files'] : array();

  foreach ($files as $file) {
    $form['uploaded_docs'][$file->fid] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );
    $form['uploaded_docs'][$file->fid]['filename'] = array(
      '#markup' => drupal_basename($file->filename),
      '#prefix' => '<a target="_blank" href="' . file_create_url($file->uri) . '">',
      '#suffix' => '</a>',
    );
    $form['uploaded_docs'][$file->fid]['remove_file'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#name' => 'remove_file_' . $file->fid,
      '#limit_validation_errors' => array(),
      '#submit' => array('form_experiments_07_doc_remove'),
      '#attributes' => array('class' => array('pull-right', 'btn-xs')),
      '#ajax' => array(
        'callback' => 'form_experiments_07_doc_remove_callback',
        'wrapper' => 'ajax-wrapper',
      ),
    );
  }

  /** ADD A FILE **/

  if (sizeof($files) < 3) {
    $form['uploaded_docs']['doc_upload'] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('auto-upload')),
    );
    $form['uploaded_docs']['doc_upload']['file'] = array(
      '#type' => 'file',
    );
    $form['uploaded_docs']['doc_upload']['upload'] = array(
      '#type' => 'submit',
      '#value' => t('Upload'),
      '#limit_validation_errors' => array(),
      '#submit' => array('form_experiments_07_doc_upload'),
      '#ajax' => array(
        'callback' => 'form_experiments_07_doc_upload_callback',
        'wrapper' => 'ajax-wrapper',
      ),
    );
  }

  /** FORM ACTIONS **/

  $form['form-actions'] = array(
    '#type' => 'actions',
    '#tree' => TRUE,
  );
  $form['form-actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('form_experiments_07_cancel'),
  );
  $form['form-actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('form_experiments_07_submit'),
  );

  return $form;
}

/**
 * Validate the form.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function form_experiments_07_validate($form, &$form_state) {
}

/**
 * Process doc removal.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function form_experiments_07_doc_remove($form, &$form_state) {
  $fid = $form_state['triggering_element']['#parents'][0];
  $file = $form_state['files'][$fid];

  if (file_delete($file)) {
    unset($form_state['files'][$fid]);
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Ajax callback to update uploaded_docs after a doc is removed.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return mixed
 *   The updated portion of the form.
 */
function form_experiments_07_doc_remove_callback($form, &$form_state) {
  return $form['uploaded_docs'];
}

/**
 * Process doc upload.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function form_experiments_07_doc_upload($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('png gif jpg jpeg'));
  $file = file_save_upload('file', $validators, "public://", FILE_EXISTS_REPLACE);

  if ($file) {
    $form_state['files'] = isset($form_state['files']) ? $form_state['files'] : array();

    $form_state['files'][$file->fid] = $file;

    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Ajax callback to update uploaded_docs after a doc is added.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return mixed
 *   The updated portion of the form.
 */
function form_experiments_07_doc_upload_callback($form, &$form_state) {
  return $form['uploaded_docs'];
}

/**
 * Process form submit.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function form_experiments_07_submit($form, &$form_state) {
  drupal_set_message('Submit form');
}

/**
 * Process form cancel.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function form_experiments_07_cancel($form, &$form_state) {
  drupal_set_message('Cancel form');
}
