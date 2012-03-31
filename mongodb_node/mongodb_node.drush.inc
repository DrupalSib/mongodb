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

  // Skip operation name.
  array_shift($args);

  if (isset($args[0])) {
    is_numeric($args[0]) ? $nid = $args[0] : $type = $args[0];
  }

  $thread_num = isset($args[1]) ? $args[1] : 0;
  $options = array();
  $name = "mongodb_drush_thread_$thread_num";
  $sub_query = 'SELECT @row := @row + 1 AS rownum, nid FROM (SELECT @row := 0) r, {node} WHERE nid > %d ';
  $offset = variable_get($name, 0);
  $options[] = $offset;

  if (!isset($type) && !isset($nid)) {
    drush_print(dt('Start to sync all nodes'));
  }
  if (isset($nid)) {
    $sub_query .= 'AND nid = %d ';
    $options[] = $nid;
  }
  else {
    $sub_query .= 'AND type LIKE "%s" ';
    $options[] = $type;
    drush_print(dt('Start to sync nodes with type @type', array('@type' => $type)));
  }

  $query = "SELECT nid FROM ($sub_query ORDER BY nid ASC) tmp ";

  if ($thread_num) {
    $query .= 'WHERE rownum % %d = 0';
    $options[] = $thread_num;
  }

  $res = db_query($query, $options);
  $counter = 0;

  drush_print(dt('Thread number @num', array('@num' => $thread_num)));

  while ($nid = db_result($res)) {
    drush_print("nid: $nid");
    $node = node_load($nid, NULL, TRUE);

    if (is_object($node)) {
      mongodb_node_mongo_update($node);
      $counter++;

      // Save current offset.
      variable_set($name, $nid);
    }

    if ($counter % 100 == 0) {
      drush_print(dt('Processed @count', array('@count' => $counter)));
    }
  }

  variable_del($name);
  drush_print(dt("@count nodes successfully synced", array('@count' => $counter)));
}
