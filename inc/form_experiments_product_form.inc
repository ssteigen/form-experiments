<?php
/**
 * @file
 * Functionality to support the product form.
 */

function form_experiments_product_form($form, &$form_state, $product_id = NULL) {
  $product = NULL;

  // If a product id was specified, load the data for that product.
  if ($product_id) {
    $product = form_experiments_product_get($product_id);

    if (empty($product)) {
      watchdog('form_experiment', '@product_id is not a valid product.', array('@product_id' => $product_id), WATCHDOG_ERROR);

      drupal_set_message($product_id . ' is not a valid product.', 'error');
      drupal_goto('form_experiments');
    }

    // If the product is not editable, redirect to the view product page.
    if (!$product['is_editable']) {
      drupal_set_message('You do not have permission to edit this product.', 'error');
      drupal_goto('form_experiments/product/' . $product_id);
    }

    $type = $product['type'];
    $brand = $product['brand_name'];
    $title = $product['title'];
    $date = $product['start_date'];
    $price = number_format($product['price'], 2);

    $images = isset($product['images']) ? $product['images'] : array();
  }

  // Get configuration settings for the form.
  $max_num_images = 4;
  $valid_img_types = 'png jpg';
  $max_img_size = 6291456;

  $form = array(
    '#prefix' => '<div id="product-form" class="ajax-disabling">',
    '#suffix' => '</div>',
    '#cache' => FALSE,
    '#attached' => array(
      'library' => array(array('system', 'ui.autocomplete')),
      'js' => array(
        drupal_get_path('module', 'form_experiments') . '/js/form_experiments_product_form.js',
        array(
          'data' => array('form_experiments_brands' => array_values(form_experiments_get_brands())),
          'type' => 'setting',
        ),
      ),
    ),
  );

  $form['form_legend'] = array(
    '#markup' => '<p class="form-legend">' . t('Required') . ' <span class="form-required">*</span></p>',
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array_values(form_experiments_get_types()),
    '#empty_option' => t('- Select -'),
    '#field_prefix' => '<span class="icon-downarrow"></span>',
    '#required' => TRUE,
    '#default_value' => (isset($type) ? $type : NULL),
  );

  $form['brand'] = array(
    '#type' => 'textfield',
    '#title' => t('Brand'),
    '#maxlength' => 100,
    '#attributes' => array('placeholder' => t('Add the name of the brand')),
    '#required' => TRUE,
    '#default_value' => (isset($brand) ? $brand : ''),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#maxlength' => 100,
    '#attributes' => array('placeholder' => t('Add the name of the product')),
    '#required' => TRUE,
    '#default_value' => (isset($title) ? $title : ''),
  );

  $start_date = DateTime::createFromFormat('Y-m-d H:i:s', '2017-1-1 00:00:00');
  $today = new DateTime();

  $form['date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Date'),
    '#title_display' => 'invisible',
    '#date_format' => 'm/d/Y',
    '#attributes' => array('placeholder' => t('MM/DD/YYYY')),
    '#required' => TRUE,
    '#timepicker' => 'timepicker',
    '#datepicker_options' => array(
      'minDate' => $start_date->format('m/d/Y'),
      'maxDate' => $today->format('m/d/Y'),
    ),
    '#default_value' => (isset($date) ? $date : ''),
  );

  $form['price'] = array(
    '#type' => 'textfield',
    '#title' => t('Price'),
    '#attributes' => array('placeholder' => t('Add price')),
    '#required' => TRUE,
    '#default_value' => (isset($price) ? $price : ''),
  );

  // Wrapper for uploaded docs (and doc input field).
  $form['supporting_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Supporting Documentation'),
    '#description' => t('(Up to @max_num files) File size is limited to a maximum of @max_size. File types supported: @valid_types.',
      array(
        '@max_num' => $max_num_images,
        '@valid_types' => $valid_img_types,
        '@max_size' => format_size($max_img_size),
      )
    ),
    '#tree' => TRUE,
    '#prefix' => '<div id="js-ajax-elements-wrapper">',
    '#suffix' => '</div>',
    '#attributes' => array('id' => 'supporting-files'),
  );

  // If an product id has been provided, add it to the form_state.
  if (isset($product_id)) {
    $form_state['product_id'] = $product_id;
  }

  // If the file_attachments have not yet been set,
  // populate it with any previously uploaded documents.
  if (!isset($form_state['file_attachments'])) {
    $form_state['file_attachments'] = isset($images) ? $images : array();

    // Initialize the add/remove doc arrays.
    $form_state['add_docs'] = array();
    $form_state['remove_docs'] = array();
  }

  // Render uploaded docs.
  foreach ($form_state['file_attachments'] as $doc_id => $doc) {
    // Create a container for the doc.
    $form['supporting_files'][$doc_id] = array(
      '#type' => 'container',
      '#tree' => FALSE,
      '#attributes' => array('class' => array('supporting-doc')),
    );

    // Build a link to the doc.
    $doc_link = l(
      $doc['doc_file_name'],
      'form_experiments/document/' . $doc_id,
      array('attributes' => array('target' => '_blank'))
    );
    // Render the doc size.
    $doc_size = '(' . format_size($doc['doc_size_unencoded']) . ')';

    // Add the doc link and size to the container.
    $form['supporting_files'][$doc_id]['filename_' . $doc_id] = array(
      '#markup' => $doc_link . ' <span class="description">' . $doc_size . '</span>',
      '#prefix' => '<span class="doc-name">',
      '#suffix' => '</span>',
    );

    // Add the remove button next to the file.
    $form['supporting_files'][$doc_id]['remove_file_' . $doc_id] = array(
      '#markup' => '<span class="glyphicon glyphicon-trash"><span class="sr-only">Remove</span></span>',
      '#prefix' => '<a class="btn btn-link btn-narrow pull-right remove">',
      '#suffix' => '</a>',
    );

    $form['supporting_files'][$doc_id]['confirmation-group'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('btn-group pull-right'),
        'style' => array('min-width:0'),
      ),
    );

    // Add the confirm remove button.
    $form['supporting_files'][$doc_id]['confirmation-group']['confirm_remove_file_' . $doc_id] = array(
      '#type' => 'submit',
      '#value' => 'Delete',
      '#attributes' => array(
        'class' => array(
          'btn', 'btn-default', 'btn-left', 'btn-narrow', 'confirm-remove',
        ),
      ),
      '#name' => 'remove_file_' . $doc_id,
      '#limit_validation_errors' => array(),
      '#submit' => array('form_experiments_product_form_doc_remove'),
      '#ajax' => array(
        'callback' => 'form_experiments_product_form_doc_remove_callback',
        //'path' => 'form-experiments/system/ajax',
        'wrapper' => 'js-ajax-elements-wrapper',
      ),
    );

    // Add the cancel remove button.
    $form['supporting_files'][$doc_id]['confirmation-group']['cancel_remove_file_' . $doc_id] = array(
      '#markup' => '<a class="btn btn-default btn-right btn-narrow cancel-remove">Cancel</a>',
    );
  }

  // If less than $max_num_docs files have been uploaded so far,
  // render the file upload fieldset.
  if (count($form_state['file_attachments']) < $max_num_images) {
    $form['supporting_files']['doc_upload'] = array(
      '#type' => 'container',
      '#tree' => FALSE,
      '#attributes' => array('class' => array('auto-upload')),
    );
    $form['supporting_files']['doc_upload']['file'] = array(
      '#type' => 'file',
    );
    $form['supporting_files']['doc_upload']['upload'] = array(
      '#type' => 'submit',
      '#value' => t('Upload'),
      '#limit_validation_errors' => array(),
      '#submit' => array('form_experiments_product_form_doc_upload'),
      '#ajax' => array(
        'callback' => 'form_experiments_product_form_doc_upload_callback',
        //'path' => 'form-experiments/system/ajax',
        'wrapper' => 'js-ajax-elements-wrapper',
      ),
    );
  }

  $form['form-actions'] = array(
    '#type' => 'actions',
    '#tree' => TRUE,
  );
  $form['form-actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('form_experiments_product_form_submit'),
  );
  $form['form-actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('form_experiments_product_form_cancel'),
  );

  // Empty the file upload errors.
  $form_state['file_upload_errors'] = NULL;

  return $form;
}

function form_experiments_product_form_validate($form, &$form_state) {
  krumo($form);

  $brand = $form_state['values']['brand'];
  $min_brand_chars = 2;
  if (!empty($brand) && strlen($brand) < 2) {
    form_set_error('brand', 'Brand name must contain at least @min_chars characters.', array('@min_chars' => $min_brand_chars));
  }

  $title = $form_state['values']['title'];
  $min_title_chars = 2;
  if (!empty($title) && strlen($title) < $min_title_chars) {
    form_set_error('title', 'Product title must contain at least @min_chars characters.', array('@min_chars' => $min_title_chars));
  }

  //// Date must be within current renewal period and not in the future.
  //if (!empty($form_state['values']['date'])) {
  //  $date = DateTime::createFromFormat('Y-m-d', $form_state['values']['date']);
  //  if (isset($date) && $date > new DateTime()) {
  //    form_set_error('date', 'Date cannot be in the future.');
  //  }
  //
  //  $start_date = DateTime::createFromFormat('Y-m-d', '2017-1-1');
  //  if ($date < $start_date) {
  //    form_set_error('date', 'Date must be after @start_date', array('@start_date' => $start_date));
  //  }
  //}

  // Hours must be within valid range and use valid increments.
  $price = $form_state['values']['price'];
  $increment = .25;
  $min_price = $increment;
  $max_price = 999;
  if (!empty($price) && ($price < $min_price || $price > $max_price)) {
    form_set_error('price', 'Price must be greater than @min_price and less than @max_price', array('@main_price' => $min_price, '@max_price' => $max_price));
  }
  if (!empty($price) && fmod(floatval($price), floatval($increment)) != 0) {
    form_set_error('price', 'Price hours must be rounded to the nearest $0.@increment', array('@increment' => $increment));
  }

  // @todo: Is this the best place for this? Use $form['#after_build'] instead?
  form_experiments_product_form_set_inline_errors($form, $form_state);
}

function form_experiments_product_form_submit($form, &$form_state) {
  // Get the values from the form.
  $values = $form_state['values'];

  // Build a product from the submitted form data.
  $product = array(
    'type' => intval($values['type']),
    'brand' => $values['brand'],
    'title' => $values['title'],
    'date' => date('Y-m-d H:i:s', strtotime($values['date'])),
    'price' => floatval($values['price']),
  );

  // If modifying an existing product...
  if ($form_state['product_id']) {
    // Fill-in the product id.
    $product['id'] = intval($form_state['product_id']);

    // Remove any matching elements from the arrays.
    // (If a doc was added and removed, it would appear in both arrays.)
    $matching = array_intersect($form_state['add_docs'], $form_state['remove_docs']);
    $add_docs = array_diff($form_state['add_docs'], $matching);
    $remove_docs = array_diff($form_state['remove_docs'], $matching);

    // Fill-in the documents to add/remove.
    if (!empty($add_docs)) {
      $product['documents'][0]['add'] = array_values($add_docs);
    }

    if (!empty($remove_docs)) {
      $product['documents'][0]['delete'] = array_values($remove_docs);
    }

    $message = '<span aria-hidden="true" class="glyphicon glyphicon-ok"></span> Product Updated';
  }
  // If creating a new product...
  else {
    // Fill-in any documents included in the form submission.
    $product['documents'][0]['add'] = array_keys($form_state['file_attachments']);

    $message = '<span aria-hidden="true" class="glyphicon glyphicon-ok"></span> Product Submitted';
  }

  // Try to save the product.
  // Upon success, an product is returned.
  $product_id = form_experiments_product_save($product);

  if ($product_id) {
    drupal_set_message($message, 'status');
  }
  else {
    drupal_set_message('An error occurred while saving the product.', 'error');
    drupal_goto('/form-experiments');
  }

  drupal_goto('/form-experiments/product/' . $product);
}

function form_experiments_product_form_cancel($form, &$form_state) {
  drupal_goto('/form-experiments');
}

function form_experiments_popup_process_alter(&$element, &$form_state, $context) {
  unset($element['date']['#description']);
  return $element;
}

function form_experiments_product_form_set_inline_errors($form, &$form_state) {
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
  }

  // @todo: Is this a valid thing to do? Can't find docs on it anywhere.
  // For some reason, the page is rendering form state's complete form,
  // rather than the form array.
  $form_state['complete form'] = $form;
}

function form_experiments_product_form_doc_remove($form, &$form_state) {
  // Get the name of the field that corresponds to the button that was clicked.
  $field_id = $form_state['triggering_element']['#parents'][0];

  // Parse the field name to get the $doc_id.
  $doc_id = str_replace("confirm_remove_file_", '', $field_id);

  // Queue the doc up for removal.
  $form_state['remove_docs'][] = intval($doc_id);

  // Remove the doc from the form.
  // Don't actually delete the file until the form is submitted.
  unset($form_state['file_attachments'][$doc_id]);

  // Rebuild the form.
  $form_state['rebuild'] = TRUE;
}

function form_experiments_product_form_doc_remove_callback($form, &$form_state) {
  return $form['supporting_files'];
}

function form_experiments_product_form_doc_upload($form, &$form_state) {
  $valid_img_types = 'png jpg';
  $max_img_size = 6291456;

  // Define the file validators.
  $validators = array(
    'file_validate_extensions' => array('png gif jpg jpeg'),
    'file_validate_size' => array($max_img_size),
  );

  // Try to upload the file. Perform validation.
  $drupal_file = file_save_upload('file', $validators);

  // If the file was saved to Drupal...
  if ($drupal_file) {
    // Format a Drupal file to a doc array.
    $doc = array(
      'doc_file_name' => $drupal_file->filename,
      'doc_format' => 'BASE64',
      'mime_type' => $drupal_file->filemime,
      'doc_size_unencoded' => intval($drupal_file->filesize),
      'clob' => base64_encode(file_get_contents($drupal_file->uri)),
    );

    // Save the file.
    $doc_id = form_experiments_doc_save($doc);

    // If the file was successfully saved to oracle...
    if ($doc_id) {
      // Queue the doc up for attachment.
      $form_state['add_docs'][] = intval($doc_id);

      // Get the new doc.
      $doc = form_experiments_get_doc($doc_id);

      // Load the doc info into the form.
      if (isset($doc)) {
        // If file attachments have not been set, use an empty array.
        if (!isset($form_state['file_attachments'])) {
          $form_state['file_attachments'] = array();
        }

        // Add the new doc to the file attachments.
        $form_state['file_attachments'][$doc['doc_id']] = $doc;

        // Only rebuild the form when a file passes
        // validation and is uploaded correctly.
        $form_state['rebuild'] = TRUE;
      }
      else {
        drupal_set_message('Doc not found', 'error');
      }
    }

    //// Delete the file from the Drupal database.
    //if (file_delete($drupal_file) == FALSE) {
    //  drupal_set_message('File could not be deleted.', 'error');
    //};
  }
  else {
    drupal_set_message('File upload error', 'error');
  }
}

function form_experiments_product_form_doc_upload_callback($form, &$form_state) {
  // Attach the file-related errors to the file field.
  // @todo: What if there are multiple file errors?
  // @todo: What if the message isn't of type 'error'?
  // @todo: Is this the correct place to do this?
  if (isset($form_state['file_upload_errors'])) {
    $form['supporting_files']['doc_upload']['#suffix'] = '<p class="help-block error">' . $form_state['file_upload_errors']['error'][0] . '</p>';
  }

  return $form['supporting_files'];
}
