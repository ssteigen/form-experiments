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

  $form['other_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Other field'),
  );

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

  $form['file_upload'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#attributes' => array('class' => array('auto-upload')),
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
