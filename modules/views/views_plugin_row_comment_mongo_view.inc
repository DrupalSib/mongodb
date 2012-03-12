<?php
// $Id: views_plugin_row_comment_mongo_view.inc,v 1.1.2.1 2009/12/28 21:11:50 merlinofchaos Exp $
/**
 * @file
 * Contains the node RSS row style plugin.
 */

/**
 * Plugin which performs a comment_view on the resulting object.
 */
class views_plugin_row_comment_mongo_view extends views_plugin_row {
  var $base_table = 'comments';
  var $base_field = 'cid';

  function option_definition() {
    $options = parent::option_definition();
    $options['links'] = array('default' => TRUE);
    $options['mode'] = array('default' => 'full');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display links'),
      '#default_value' => $this->options['links'],
    );
    
    $options = array(
      'full'    => 'full',
      'teaser'  => 'teaser',
      'expert'  => 'expert'
    );
    $form['mode'] = array(
      '#type' => 'select',
      '#title' => t('Build mode comments'),
      '#options' => $options,
      '#default_value' => $this->options['mode'],
    );
  }

  function pre_render($result) {
    $cids = array();
    $this->comments = array();

    foreach ($result as $row) {
      $cids[] = (int) $row->cid;
    }
    if (!_MONGODB_ENABLED) {
      if (count($cids) > 1) {
        $placeholder = " IN (" . implode(', ', array_fill(0, sizeof($cids), '%d')) . ")";
      }
      else {
        $placeholder = " = %d";
      }
      $cresult = db_query("SELECT c.cid as cid, c.pid, c.nid, c.subject, c.comment, c.format, c.timestamp, c.name, c.mail, c.homepage, u.uid, u.name AS registered_name, u.signature, u.picture, u.data, c.thread, c.status FROM {comments} c INNER JOIN {users} u ON c.uid = u.uid WHERE c.cid $placeholder", $cids);
      while ($comment = db_fetch_object($cresult)) {
        $comment = drupal_unpack($comment);
        $comment->name = $comment->uid ? $comment->registered_name : $comment->name;
        $comment->depth = count(explode('.', $comment->thread)) - 1;
        $this->comments[$comment->cid] = $comment;
      }
    }
    elseif ($cresult = mongo_comments_comment_get_by_cids($cids)) {
      foreach ($cresult as $comment) {
        $comment = drupal_unpack($comment);
        $comment->name = $comment->name;
        $comment->depth = count(explode('.', $comment->thread)) - 1;
        $this->comments[$comment->cid] = $comment;
      }
    }
  }
}


