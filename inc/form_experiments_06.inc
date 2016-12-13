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

  $form['notes'] = array(
    '#markup' => '
      <h2>To-do:</h2>
      <ul>
        <li>Apply Bootstrap theme - check upload button behavior</li>
        <li>Implement datepicker</li>
        <li>Implement provider auto-complete</li>
        <li>Add validation
          <ul>
            <li>Provider must be at least two characters</li>
            <li>Title must be at least two characters</li>
            <li>Date completed must be before today (today is also valid)</li>
            <li>Credit hours must be an integer between .25 and 999 (inclusive - .25 and 999 are allowed)</li>
            <li>Credit hours can be divided into quarters only (i.e. x.00, x.25, x.50, x.75 are allowed)</li>
          </ul>
        </li>
        <li>File upload tweaks
          <ul>
            <li>Files should appear between "Supporting Documentation" heading and "choose file" input</li>
            <li>File names should be links to open the file in a new tab</li>
            <li>Appear blank when upload completes (clear out last filename)</li>
            <li>Limit to three files</li>
            <li>Limit filesize to 6MB</li>
            <li>Supported file types: doc docx gif jpeg jpg odt pdf png tif tiff</li>
            <li>Implement delete confirmation</li>
          </ul>
        </li>
      </ul>
    ',
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Activity Type'),
    '#options' => array(t('Option 1'), t('Option 2'), t('Option 3')),
    '#empty_option' => '- Select -',
    //'#required' => TRUE,
  );

  $form['provider'] = array(
    '#type' => 'textfield',
    '#title' => t('Provider'),
    '#attributes' => array('placeholder' => t('Add the name of the provider')),
    //'#required' => TRUE,
    //'#description' => t('Add the name of the provider'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title / Description'),
    '#attributes' => array('placeholder' => t('Add the name of the activity')),
    //'#description' => t('Add the name of the activity'),
    //'#required' => TRUE,
  );

  $form['date'] = array(
    '#type' => 'date',
    '#title' => t('Date Completed'),
    //'#required' => TRUE,
  );

  $form['credits'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit Hours'),
    '#attributes' => array('placeholder' => t('Add hours')),
    //'#description' => t('Add hours'),
    //'#required' => TRUE,
  );

  /** UPLOADED FILES **/

  $form['field_container'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#prefix' => '<div id="js-ajax-elements-wrapper">',
    '#suffix' => '</div>',
  );
  $uploaded_files = isset($form_state['file_attachments']) ? $form_state['file_attachments'] : array();
  foreach ($uploaded_files as $fid => $file) {
    $form['field_container'][$fid] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );
    $form['field_container'][$fid]['filename'] = array(
      '#markup' => drupal_basename($file->filename),
    );
    $form['field_container'][$fid]['remove_file'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#name' => 'remove_file_' . $file->fid,
      '#submit' => ['form_experiments_06_remove'],
      '#ajax' => array(
        'callback' => 'form_experiments_06_remove_callback',
        'wrapper' => 'js-ajax-elements-wrapper',
      ),
    );
  }

  /** ADD A FILE **/

  $form['file_upload'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#attributes' => array('class' => array('auto-upload')),
    '#title' => t('Supporting Documentation'),
  );
  $form['file_upload']['file'] = array(
    '#type' => 'file',
  );
  $form['file_upload']['upload'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
    '#submit' => ['form_experiments_06_upload'],
    '#ajax' => array(
      'callback' => 'form_experiments_06_upload_callback',
      'wrapper' => 'js-ajax-elements-wrapper',
    ),
  );

  /** FORM ACTIONS **/

  $form['form-actions'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );
  $form['form-actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => ['form_experiments_06_cancel'],
  );
  $form['form-actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => ['form_experiments_06_submit'],
  );

  return $form;
}

function form_experiments_06_remove($form, &$form_state) {
  $fid = $form_state['triggering_element']['#parents'][1];
  $file = $form_state['file_attachments'][$fid];

  unset($form_state['file_attachments'][$fid]);
  file_delete($file);

  $form_state['rebuild'] = TRUE;
}

function form_experiments_06_remove_callback($form, &$form_state) {
  return $form['field_container'];
}

function form_experiments_06_upload($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('png gif jpg jpeg'));
  $file = file_save_upload('file', $validators);

  if ($file) {
    if (!isset($form_state['file_attachments'])) {
      $form_state['file_attachments'] = [];
    }

    $form_state['file_attachments'][$file->fid] = $file;

    // Only rebuild the form when a file passes validation and is uploaded correctly.
    $form_state['rebuild'] = TRUE;
  }
}

function form_experiments_06_upload_callback($form, &$form_state) {
  return $form['field_container'];
}

function form_experiments_06_submit($form, &$form_state) {
  drupal_goto('/');
}

function form_experiments_06_cancel($form, &$form_state) {
  drupal_goto('/');
}
