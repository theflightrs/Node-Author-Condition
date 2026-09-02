<?php
/**
 * @file
 * Class to provide access control based on whether the user is the node author.
 */
class NodeAuthorLayoutAccess extends LayoutAccessNegatable {

  /**
   * {@inheritdoc}
   */
  function summary() {
    return t('The current user is the node author (or administrator)');
  }

  /**
   * {@inheritdoc}
   */
  function checkAccess() {
    global $user;

    // 1. Always allow administrators or user 1
    if ($user->uid == 1 || user_access('bypass node access') || user_access('administer nodes')) {
      return TRUE;
    }

    // 2. Check if a node context exists in this layout
    if (isset($this->contexts['node'])) {
      $context = $this->contexts['node'];
      if (!empty($context->data) && is_object($context->data)) {
        $node = $context->data;
        
        // 3. Compare node author UID with current user UID
        if (isset($node->uid) && $node->uid == $user->uid) {
          return TRUE;
        }
      }
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  function form(&$form, &$form_state) {
    parent::form($form, $form_state);
    
    $form['info'] = array(
      '#markup' => t('This condition evaluates to TRUE if the logged-in user is the author of the node, or has administrative privileges.'),
    );
  }
}