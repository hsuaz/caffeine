<?php
class FurAffinityApi_User
  extends FurAffinityApi_AbstractBase
{

  public function getLoginSalt ($username) {
    $params = array(
      'username' => $username,
    );
    $result = $this->_query('user/login/getPasswordSalt', $params);
    if (!$this->_isSuccess ($result)) {
      return NULL;
    }

    return $result['result']['salt'];
  }


  public function login ($username, $password) {
    $params = array(
      'username' => $username,
      'passwordHash' => $password,
    );
    $result = $this->_query('user/login', $params);
    if (!$this->_isSuccess ($result)) {
      return NULL;
    }

    return $result['result']['sid'];
  }

  public function getUserInfo ($username) {
    $params = array(
      'username' => $username,
    );
    $result = $this->_query('user/find/username', $params);
    if (!$this->_isSuccess ($result)) {
      return NULL;
    }
    return $result['result']['user'];
  }

  public function logout () {
    $result = $this->_query('user/logout');
    return TRUE;
  }
}