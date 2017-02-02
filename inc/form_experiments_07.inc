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

  //$form['#after_build'] = array('form_experiments_07_set_inline_errors');

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
    );
    $form['uploaded_docs'][$file->fid]['remove_file'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#name' => 'remove_file_' . $file->fid,
      '#limit_validation_errors' => array(),
      '#submit' => array('form_experiments_07_doc_remove'),
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
 * Set inline form errors.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function form_experiments_07_set_inline_errors($form, &$form_state) {
  // Get the form errors.
  $errors = form_get_errors();

  // If there are any errors, iterate over them
  // and add them to the appropriate form element.
  if (isset($errors)) {
    // Clear error messages.
    drupal_get_messages('error');

    // Add field-associated error messages to their appropriate fields.
    foreach ($errors as $field => $error) {
      // Only process the field-associated errors, omit the page-level ones.
      if (array_key_exists($field, $form)) {
        $form[$field]['#field_suffix'] = '<p class="help-block error">' . $error . '</p>';

        // Remove field-associated error messages from the page-level listing.
        unset($errors[$field]);
      }
    }

    // If there are page-level errors left, add them back as page-level errors.
    if (count($errors) > 0) {
      foreach ($errors as $field => $error) {
        drupal_set_message($error, 'error');
      }
    }
    // @todo: Is this a valid thing to do? Can't find docs on it anywhere.
    $form_state['complete form'] = $form;
  }
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
  $fid = $form_state['triggering_element']['#parents'][1];
  $file = $form_state['file_attachments'][$fid];

  if (file_delete($file)) {
    unset($form_state['file_attachments'][$fid]);
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message('File could not be deleted.', 'error');
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
  else {
    drupal_set_message('File could not be uploaded.', 'error');
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
