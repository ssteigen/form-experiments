<?php

function form_experiments_01() {
  $form['summary'] = array(
    '#markup' => 'Multiple Managed File Fields',
    '#prefix' => '<p class="lead">',
    '#suffix' => '</p>',
  );

  $form['description'] = array(
    '#markup' => 'Each managed file field allows one upload',
    '#prefix' => '<p>',
    '#suffix' => '</p>',
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

