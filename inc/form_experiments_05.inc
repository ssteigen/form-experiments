<?php
function form_experiments_05($form, &$form_state) {
  $form['#cache'] = false;
  $form['field_container'] = [
    '#type' => 'container',
    '#weight' => 80,
    '#tree' => TRUE,
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="js-ajax-elements-wrapper">',
    '#suffix' => '</div>',
  ];

  $uploaded_files = isset($form_state['file_attachments']) ? $form_state['file_attachments'] : [];

  foreach ($uploaded_files as $fid => $file) {
    $form['field_container'][$fid] = [
      '#type' => 'fieldset',
      '#tree' => TRUE,
    ];

   $form['field_container'][$fid]['field1'] = [
      '#markup' => drupal_basename($file->filename),
    ];

    $form['field_container'][$fid]['remove_file'] = [
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => ['form_experiments_05_remove'],
      '#attributes' => array(
        'style' => 'float:right',
      ),
      // See the examples in ajax_example.module for more details on the
      // properties of #ajax.
      '#ajax' => [
        'callback' => 'form_experiments_05_remove_callback',
        'wrapper' => 'js-ajax-elements-wrapper',
      ],
      '#name' => 'remove_file_' . $file->fid,
    ];
  }

  $form['file_upload'] = [
    '#type' => 'file',
  ];

  $form['field_container']['add_name'] = [
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => ['form_experiments_05_add_one'],
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    '#ajax' => [
      'callback' => 'form_experiments_05_add_one_callback',
      'wrapper' => 'js-ajax-elements-wrapper',
    ],
    '#weight' => 100,
  ];

  $form['other_field'] = [
    '#type' => 'textfield',
    '#title' => t('Other field'),
  ];

  return $form;
}

function form_experiments_05_remove($form, &$form_state) {
  $fid = $form_state['triggering_element']['#parents'][1];
  $file = $form_state['file_attachments'][$fid];

  unset($form_state['file_attachments'][$fid]);
  file_delete($file);

  $form_state['rebuild'] = TRUE;
}

function form_experiments_05_remove_callback($form, &$form_state) {
  return $form['field_container'];
}

function form_experiments_05_add_one($form, &$form_state) {
  $validators = array('file_validate_extensions' => array('png gif jpg jpeg'));
  $file = file_save_upload('file_upload', $validators);

  if ($file) {
    if (!isset($form_state['file_attachments'])) {
      $form_state['file_attachments'] = [];
    }

    $form_state['file_attachments'][$file->fid] = $file;

    // Only rebuild the form when a file passes validation and is uploaded correctly.
    $form_state['rebuild'] = TRUE;
  }
}

function form_experiments_05_add_one_callback($form, $form_state) {
  return $form['field_container'];
}
