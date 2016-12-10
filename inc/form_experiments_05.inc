<?php
function form_experiments_05($form, &$form_state) {
  $form['field_container'] = [
    '#type' => 'container',
    '#weight' => 80,
    '#tree' => TRUE,
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="js-ajax-elements-wrapper">',
    '#suffix' => '</div>',
  ];

  $uploaded_files = isset($form_state['file_attachments']) ? $form_state['file_attachments'] : [];

  foreach ($uploaded_files as $uri => $filename) {
    $form['field_container'][$uri] = [
      '#type' => 'fieldset',
      '#tree' => TRUE,
    ];

   $form['field_container'][$uri]['field1'] = [
      '#markup' => $filename,
    ];

    $form['field_container'][$uri]['remove_file'] = [
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
      '#name' => 'remove_file_' . $uri,
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
  print_r($form_state['triggering_element']['#parents']);

  $uri_to_remove = $form_state['triggering_element']['#parents'][1];

  unset($form_state['file_attachments'][$uri_to_remove]);

  $form_state['rebuild'] = TRUE;
  drupal_get_messages();
}

function form_experiments_05_remove_callback($form, &$form_state) {
  return $form['field_container'];
}

function form_experiments_05_add_one($form, &$form_state) {
  $file = file_save_upload('file_upload');

  if (!$form_state['file_attachments']) {
    $form_state['file_attachments'] = [];
  }

  $form_state['file_attachments'][$file->uri] = $file->filename;

  $form_state['rebuild'] = TRUE;
  drupal_get_messages();
}

function form_experiments_05_add_one_callback($form, $form_state) {
  return $form['field_container'];
}
