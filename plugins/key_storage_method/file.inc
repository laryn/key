<?php

/**
 * @file
 * Plugin definition for the File key storage method.
 */

$plugin = array(
  'title' => t('File'),
  'description' => t('Use a file, preferably one that is outside the web root directory.'),
  'key get callback' => 'key_storage_method_file_get_key',
  'settings form' => 'key_storage_method_file_settings_form',
  'instructions' => 'key_storage_method_file_instructions',
);

/**
 * Callback function to return a key from a file.
 */
function key_storage_method_file_get_key($settings) {
  if (empty($settings['location']) || empty($settings['method'])) {
    return NULL;
  }
  $file = $settings['location'];
  switch ($settings['method']) {
    case 'file_contents':
      $key = file_get_contents($file);
      break;
    case 'md5':
      $key = md5_file($file);
      break;
    default:
      $key = NULL;
  }

  return $key;
}

/**
 * Settings form for the File key storage method.
 */
function key_storage_method_file_settings_form($defaults) {
  $form = array();

  $form['location'] = array(
    '#type' => 'textfield',
    '#title' => t('File location'),
    '#description' => t('The location of the file in which the key will be stored. The path may be absolute (e.g., %abs), relative to the Drupal directory (e.g., %rel), or defined using a stream wrapper (e.g., %str).', array(
      '%abs' => '/etc/keys/foobar.key',
      '%rel' => '../keys/foobar.key',
      '%str' => 'private://keys/foobar.key',
      )
    ),
    '#default_value' => isset($defaults['location']) ? $defaults['location'] : '',
    '#required' => TRUE,
  );
  $form['method'] = array(
    '#type' => 'select',
    '#title' => t('Method'),
    '#description' => t('If the selected method is “File contents”, the contents of the file will be used as entered. If “MD5 hash” is selected, an MD5 hash of the file contents will be used as the key.'),
    '#options' => array(
      'file_contents' => t('File contents'),
      'md5' => t('MD5 hash'),
    ),
    '#default_value' => isset($defaults['method']) ? $defaults['method'] : 'file_contents',
  );

  return $form;
}

function key_storage_method_file_instructions() {
  return array(
    'instructions' => array(
      '#markup' => t('Enter the key in a file at the specified location. Be sure the file is readable by the user the web server runs under.'),
    )
  );
}