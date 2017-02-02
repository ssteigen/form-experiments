<?php
/**
 * @file
 * Functionality to support Form 10.
 */

/**
 * Builds Form 10.
 *
 * @param array $form
 *   An associative array containing the structure of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return mixed
 *   The renderable form array.
 */
function form_experiments_10($form, &$form_state) {

  $form['field_with_input_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Input Group Example'),
    '#field_prefix' => '$',
    '#field_suffix' => '.00',
    '#input_group' => TRUE,
  );

  $form['field_with_button_input_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Input Group Button Example'),
    '#field_prefix' => '<button class="btn btn-default" type="button">$</button>',
    '#field_suffix' => '<button class="btn btn-default" type="button">.00</button>',
    '#input_group_button' => TRUE,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#maxlength' => 4,
    '#required' => TRUE,
    '#field_prefix' => '<a href="#" class="btn btn-link">test</a>',
    '#field_suffix' => '<button class="btn btn-default" type="button">Go!</button>',
    '#input_group_button' => TRUE,
  );

  // Add date/time field with Pop-up calendar to the form.
  $form['datetime'] = array(
    '#type' => 'date_popup',
    '#title' => 'Date Title',
    '#description' => 'Date Description',
    '#date_format' => 'm/d/Y',
    '#attributes' => array('placeholder' => t('MM/DD/YYYY')),
    '#timepicker' => 'timepicker',
    '#datepicker_options' => array(
      'minDate' => '01/01/2016',
      'maxDate' => '+0D +0M +0Y',
      //'buttonText' => '<span class="glyphicon glyphicon-calendar"></span>',
      //'buttonImage' => base_path() . drupal_get_path('module', 'date_api') . "/images/calendar.png",
      //'buttonImageOnly' => TRUE,
      //'showOn' => 'button',
    ),
  );

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


function form_experiments_10_validate($form, &$form_state) {
  $min_chars = 2;
  $max_chars = 6;
  $title = isset($form_state['values']['title']) ? $form_state['values']['title'] : '';
  if (strlen($title) < $min_chars || $title > $max_chars) {
    form_set_error('title', t('<em>@field_name</em> must be between @min_chars and @max_chars characters.', array(
      '@field_name' => $form_state['complete form']['title']['#title'],
      '@min_chars' => $min_chars,
      '@max_chars' => $max_chars
    )));
  }
}

function form_experiments_date_popup_process_alter(&$element, &$form_state, $context) {
  krumo($element);

  $element['date']['#title'] = $element['#title'];
  unset($element['#title']);
  $element['date']['#description'] = $element['#description'];
  unset($element['#description']);
  $element['date']['#field_suffix'] = '<a class="btn btn-primary"><span class="glyphicon glyphicon-calendar"></span></a>';
  $element['date']['#input_group_button'] = TRUE;
}
