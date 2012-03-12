<?php
/**
 * @file
 */


/**
 * Implements hook_drush_command().
 */
function mongodb_node_drush_command() {
  $items['mongodb-node-sync-all'] = array(
    'description' => '',
    'aliases' => array('mdb-nsa'),
    'examples' => array(
      'drush mongodb-node-sync-all article' => 'Sync all articles.',
    ),
  );
  
  return $items;
}

/**
 * Drush callback.
 */
function drush_mongodb_node_sync_all() {
  $args = drush_get_arguments();
  array_shift($args);
  $type = $args[0];

  $res = db_query('SELECT nid FROM {node} WHERE type LIKE "%s"', $type);
  $counter = 0;

  while ($nid = db_result($res)) {
    $node = node_load($nid);
    node_save($node);
    $counter++;
  }

  drush_print("$counter nodes with type $type successfully synced");
}