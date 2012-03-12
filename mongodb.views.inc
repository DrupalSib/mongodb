<?php 
/**
 * @file
 * Mongodb module inc file for views_api
 */

/**
 * Implementation of hook_views_plugins
 */
function mongodb_views_plugins() {
  return array(
    'row' => array(
      'node_mongodb' => array(
        'title' => t('Node MongoDb'),
        'help'  => t('Display the node with standard node view.'),
        'handler' => 'views_plugin_row_node_view_mongodb',
        'path'  => drupal_get_path('module', 'mongodb') . '/modules/views', // not necessary for most modules
        'theme' => 'views_view_row_node_mongodb',
        'base'  => array('node'), // only works with 'node' as base.
        'uses options' => TRUE,
        'type'  => 'normal',
        'help topic' => 'style-node',
      ),
      'comment_mongodb' => array(
        'title' => t('Comment MongoDb'),
        'help'  => t('Display the comment with standard comment view.'),
        'handler' => 'views_plugin_row_comment_mongo_view',
        'path'  => drupal_get_path('module', 'mongodb') . '/modules/views', // not necessary for most modules
        'theme' => 'views_view_row_comment_mongodb',
        'base'  => array('comments'), // only works with 'node' as base.
        'uses options' => TRUE,
        'type'  => 'normal',
        'help topic' => 'style-node',
      )
    ),    
  );
}



