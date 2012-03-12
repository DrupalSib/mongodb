<?php
/**
 * @file
 * Mongodb module menu callback's handlers
 */

/**
 * Mongodb settings form
 *
 * @param array $form_state
 * @return array
 */
function mongodb_settings_form(&$form_state) {
  $form = array();

  $form['mongodb_enabled'] = array(
    '#title' => 'Use mongodb',
    '#type'  => 'checkbox',
    '#default_value' => variable_get('mongodb_enabled', FALSE),
  );

//  $form['export'] = array(
//      '#type' => 'submit',
//      '#name' => 'export',
//      '#value' => t('Save and export'),
//      '#weight' => 1,
//  );

  $form['index'] = array(
      '#type' => 'submit',
      '#name' => 'index',
      '#value' => t('Save and ensure indexes'),
      '#weight' => 1,
  );

  $form['#submit'][] = 'mongodb_settings_form_submit';
  
  return system_settings_form($form);
}

/**
 * Mongodb settings form submit
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function mongodb_settings_form_submit($form, &$form_state) {
  if (!_MONGODB_ENABLED) {
    return;
  }

//  if ($form_state['clicked_button']['#name'] == 'export') {
//    mongodb_export();
//  }

  elseif ($form_state['clicked_button']['#name'] == 'index') {
    mongodb_ensure_index();
  }
}