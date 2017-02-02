<?php
/**
 * @file
 * Functionality to support Form 08.
 */

/**
 * Builds Form 08.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return mixed
 *   The renderable form array.
 */
function form_experiments_08($form, &$form_state) {
  $providers = array('Abby', 'Becky', 'Carissa', 'Daisy', 'Emma', 'Frida', 'Gretchen', 'Hannah', 'Ingrid');

  $form['#cache'] = FALSE;

  $form['#attached'] = array(
    'library' => array(array('system', 'ui.autocomplete')),
    'js' => array(
      drupal_get_path('module', 'form_experiments') . '/js/form_experiments_auto_upload.js',
      drupal_get_path('module', 'form_experiments') . '/js/form_experiments_auto_complete.js',
      array(
        'data' => array('form_experiment_providers' => $providers),
        'type' => 'setting',
      ),
    ),
  );

  $form['notes'] = array(
    '#markup' => '
      <ul>
        <li>Move upload validation errors between uploaded files and file input.</li>
        <li>Make sure form works for anonymous users.</li>
        <li>Some file validation occurs client-side?</li>
        <li>Sometimes .error class isn\'t applied to file field?</li>
        <li>Need to add remove confirmation popup.</li>
        <li>File names should be links to open the file in a new tab</li>
        <li>File field should appear blank when upload completes (clear out last filename)</li>
      </ul>
    ',
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array('Option 1', 'Option 2', 'Option 3'),
    '#empty_option' => '- Select -',
    //'#required' => TRUE,
  );

  $form['provider'] = array(
    '#type' => 'textfield',
    '#title' => t('Provider'),
    '#max_length' => 100,
    '#attributes' => array('placeholder' => t('Add the name of the provider')),
    //'#required' => TRUE,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title / Description'),
    '#max_length' => 100,
    '#attributes' => array('placeholder' => t('Add the name of the activity')),
    //'#required' => TRUE,
  );

  $form['date'] = array(
    '#type' => 'date_popup',
    '#timepicker' => 'timepicker',
    '#title' => t('Date'),
    '#title_display' => 'invisible',
    '#date_format' => 'm/d/Y',
    '#attributes' => array('placeholder' => t('MM/DD/YYYY')),
    //'#required' => TRUE,

  );

  $form['credits'] = array(
    '#type' => 'textfield',
    '#title' => t('Hours'),
    '#attributes' => array('placeholder' => t('Add hours')),
    //'#required' => TRUE,
  );

  /* UPLOADED FILES */
  $form['uploaded_docs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Supporting Documentation'),
    '#description' => t('(Up to 3 files) File size is limited to a maximum of 6MB. File types supported: doc docx gif jpeg jpg odt pdf png tif tiff.'),
    '#tree' => TRUE,
    '#prefix' => '<div id="js-ajax-elements-wrapper">',
    '#suffix' => '</div>',
  );

  $uploaded_files = isset($form_state['file_attachments']) ? $form_state['file_attachments'] : array();
  foreach ($uploaded_files as $fid => $file) {
    $form['uploaded_docs'][$fid] = array(
      '#type' => 'fieldset',
      '#tree' => FALSE,
    );
    $form['uploaded_docs'][$fid]['filename_' . $fid] = array(
      '#markup' => drupal_basename($file->filename),
    );
    $form['uploaded_docs'][$fid]['remove_file_' . $fid] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#name' => 'remove_file_' . $file->fid,
      '#limit_validation_errors' => array(),
      '#submit' => array('form_experiments_08_doc_remove'),
      '#ajax' => array(
        'callback' => 'form_experiments_08_doc_remove_callback',
        'wrapper' => 'js-ajax-elements-wrapper',
      ),
    );
  }

  if (sizeof($uploaded_files) < 3) {
    /* ADD A FILE */
    $form['uploaded_docs']['doc_upload'] = array(
      '#type' => 'fieldset',
      '#tree' => FALSE,
      '#attributes' => array('class' => array('auto-upload')),
    );
    $form['uploaded_docs']['doc_upload']['file'] = array(
      '#type' => 'file',
    );
    $form['uploaded_docs']['doc_upload']['upload'] = array(
      '#type' => 'submit',
      '#value' => t('Upload'),
      '#limit_validation_errors' => array(),
      '#submit' => array('form_experiments_08_doc_upload'),
      '#ajax' => array(
        'callback' => 'form_experiments_08_doc_upload_callback',
        'wrapper' => 'js-ajax-elements-wrapper',
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
    '#submit' => array('form_experiments_08_cancel'),
  );
  $form['form-actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('form_experiments_08_submit'),
  );

  return $form;
}


/**
 * Implements hook_element_info_alter().
 *
 * @todo Why do we need this function? It appears to work without it.
 * @see date_popup docs: @link https://www.drupal.org/node/2495255 @endlink
 * @link http://www.zyxware.com/articles/3546/drupal-7-how-to-remove-the-default-date-format-option-added-by-date-module @endlink
 */
function form_experiments_element_info_alter(&$type) {
  if (isset($type['date_popup'])) {
    $type['date_popup']['#process'][] = 'form_experiments_date_popup_process_alter';
  }
}

/**
 * Remove the description from date_popup.
 */
function form_experiments_date_popup_process_alter(&$element, &$form_state, $context) {
  unset($element['date']['#description']);
  return $element;
}

/**
 * Validate the form.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 */
function form_experiments_08_validate($form, &$form_state) {
  $provider = $form_state['values']['provider'];
  if (!empty($provider) && strlen($provider) < 2) {
    form_set_error('provider', 'Provider must contain at least two characters');
  }

  $title = $form_state['values']['title'];
  if (!empty($title) && strlen($title) < 2) {
    form_set_error('title', 'Title must contain at least two characters.');
  }

  if (!empty($form_state['values']['date'])) {
    $date = DateTime::createFromFormat('Y-m-d', $form_state['values']['date']);
    if (isset($date) && $date > new DateTime()) {
      form_set_error('date', 'Date cannot be in the future');
    }
  }

  $hours = $form_state['values']['credits'];
  if (!empty($hours) && ($hours < 0.25 || $hours > 999)) {
    form_set_error('credits', 'Hours must be between 0.25 and 999.');
  }
  if (!empty($hours) && fmod(floatval($hours), floatval(0.25)) != 0) {
    form_set_error('credits', 'Hours must be rounded to the nearest quarter-hour.');
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
function form_experiments_08_doc_remove($form, &$form_state) {
  $field_id = $form_state['triggering_element']['#parents'][0];

  $fid = str_replace("remove_file_", '', $field_id);

  $file = $form_state['file_attachments'][$fid];

  unset($form_state['file_attachments'][$fid]);

  file_delete($file);

  $form_state['rebuild'] = TRUE;
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
function form_experiments_08_doc_remove_callback($form, &$form_state) {
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
function form_experiments_08_doc_upload($form, &$form_state) {
  $validators = array(
    'file_validate_extensions' => array('doc docx gif jpeg jpg odt pdf png tif tiff'),
    'file_validate_size' => array(6291456),
  );

  // Try to upload the file.
  $file = file_save_upload('file', $validators, "public://", FILE_EXISTS_REPLACE);

  if ($file) {
    if (!isset($form_state['file_attachments'])) {
      $form_state['file_attachments'] = array();
    }

    $form_state['file_attachments'][$file->fid] = $file;

    // Only rebuild the form when a file passes
    // validation and is uploaded correctly.
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
function form_experiments_08_doc_upload_callback($form, &$form_state) {
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
function form_experiments_08_submit($form, &$form_state) {
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
function form_experiments_08_cancel($form, &$form_state) {
  drupal_set_message('Cancel form');
}
