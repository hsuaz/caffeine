<?php

class Ferrox_User_View_Helper_LoginBlock
  extends Zend_View_Helper_Abstract
{

  public function loginBlock ()
  {
    $userSession = new \Zend_Session_Namespace('user');
    $user = NULL;
    if (isset($userSession->user)) {
      $user = $userSession->user;
    }
    if (empty($user) || $user['id'] == 0) {
      $loggedIn = FALSE;
    } else {
      $loggedIn = TRUE;
    }
    $this->view->loggedIn = $loggedIn;
    $this->view->user = $user;
    return $this->view->render('userLoginBlock.phtml');
  }
}