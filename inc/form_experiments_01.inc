<?php

function form_experiments_01() {
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'form_experiments') . '/js/form_experiments.js',
  );

  $form['managed_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Managed Files'),
  );

  $num_docs = 3;
  for ($i = 1; $i <= $num_docs; $i++) {
    $form['managed_files'][$i] = array(
      '#type' => 'managed_file',
      '#title' => 'Managed File ' . $i,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}
