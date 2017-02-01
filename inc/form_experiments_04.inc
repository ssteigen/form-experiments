<?php
function form_experiments_04($form, &$form_state) {
  $form['summary'] = array(
    '#markup' => 'Ajax Add/Remove Form Item',
    '#prefix' => '<p class="lead">',
    '#suffix' => '</p>',
  );

  $form['description']['#markup'] = '<p>Dynamically add and remove form fields</p>';
  $form['description']['#markup'] .= '<p>Requires rebuild = TRUE</p>';

  $form['ajax_wrapper'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="ajax-wrapper">',
    '#suffix' => '</div>',
  );

  $form_state['field_deltas'] = isset($form_state['field_deltas']) ? $form_state['field_deltas'] : range(0, 2);

  $field_count = $form_state['field_deltas'];

  foreach ($field_count as $delta) {
    $form['ajax_wrapper'][$delta] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );

    $form['ajax_wrapper'][$delta]['item'] = array(
      '#markup' => t('Item ' . ($delta + 1)),
    );

    $form['ajax_wrapper'][$delta]['remove_one'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('form_experiments_04_remove'),
      '#attributes' => array('class' => array('btn-xs', 'pull-right')),
      '#ajax' => array(
        'callback' => 'form_experiments_04_remove_callback',
        'wrapper' => 'ajax-wrapper',
      ),
      '#name' => 'remove_name_' . $delta,
    );
  }

  $form['ajax_wrapper']['add_one'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('form_experiments_04_add_one'),
    '#ajax' => array(
      'callback' => 'form_experiments_04_add_one_callback',
      'wrapper' => 'ajax-wrapper',
    ),
  );

  return $form;
}

function form_experiments_04_remove($form, &$form_state) {
  $delta_remove = $form_state['triggering_element']['#parents'][1];
  $k = array_search($delta_remove, $form_state['field_deltas']);
  unset($form_state['field_deltas'][$k]);

  $form_state['rebuild'] = TRUE;
}

function form_experiments_04_remove_callback($form, &$form_state) {
  return $form['ajax_wrapper'];
}

function form_experiments_04_add_one($form, &$form_state) {
  $form_state['field_deltas'][] = count($form_state['field_deltas']) > 0 ? max($form_state['field_deltas']) + 1 : 0;

  $form_state['rebuild'] = TRUE;
}

function form_experiments_04_add_one_callback($form, $form_state) {
  return $form['ajax_wrapper'];
}
