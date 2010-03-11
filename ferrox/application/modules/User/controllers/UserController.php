<?php

class User_UserController extends
  \Ferrox\Controller\Action
{
  const M_UNKNOWN_USERNAME = 'Invalid username.';
  const M_UNKNOWN_PASSWORD = 'Invalid username/password.';

  public function userAction()
  {

  }

  public function loginPasswordAction () {

  }

  public function logoutAction () {
    // Ensure sessions are started
    $systemSession = new \Zend_Session_Namespace('system');

    $userApi = new \FurAffinity\Api\User();
    $userApi->logout();

    // Destroy Locally, regardless if remote session failed!
    \Zend_Session::destroy(FALSE, FALSE);
    $this->_helper->redirector('index', 'index', 'default', array());
  }


  public function loginAction ()
  {
    $userSession = new \Zend_Session_Namespace('user');
    $this->view->loginError = FALSE;

    if (isset($userSession->user) && ($userSession->user['id'] !== 0)) {
      $this->view->alreadyLoggedIn = TRUE;
      return;
    }
  }


  public function loginActionPost() {
    $userSession = new \Zend_Session_Namespace('user');
    $this->view->loginError = FALSE;

    if (isset($userSession->user) && ($userSession->user['id'] !== 0)) {
      $this->view->alreadyLoggedIn = TRUE;
      return;
    }

    $request = $this->getRequest();
    $username = $request->getParam('username', '');
    $password = $request->getParam('password', '');

    // Attempt to login
    $userApi = new \FurAffinity\Api\User();
    $salt = $userApi->getLoginSalt($username);
    if ($salt == NULL) {
      // Invalid Username
      $this->setError(self::M_UNKNOWN_USERNAME);
      return;
    }

    // Attempt to login
    $password = \sha1(\md5($password) . $salt);
    $faSessionId = $userApi->login($username, $password);

    if (empty($faSessionId)) {
      $this->setError(self::M_UNKNOWN_PASSWORD);
    }

    // Success
    if (!empty($faSessionId)) {
      $systemSession = new \Zend_Session_Namespace('system');
      $systemSession->remoteSession = $faSessionId;

      // Fetch the user object
      $userInfo = $userApi->getUserInfo($username);
      if ($userInfo !== NULL) {
        $userSession->user = $userInfo;
      }

      // Forward to the index page
      $this->_helper->redirector('index', 'index', 'default', array());
    }
  }


  /**
   * Set the error message for the page
   * @param $message string
   * @param $params array
   */
  protected function setError ($message, $params = array()) {
    $this->view->loginError = TRUE;
    $this->view->loginMessage = $this->view->translate($message, $params);
    $this->view->placeholder('messages')->loginError = $this->view->translate($message, $params);
  }
}