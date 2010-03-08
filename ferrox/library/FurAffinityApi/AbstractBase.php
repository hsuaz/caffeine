<?php

class FurAffinityApi_AbstractBase {
  static protected $_server = 'caffeine.localhost/externalApi.php';
  static protected $_protocol = 'http://';
  static protected $_sid = '';
  static protected $_devId = '';

  static public function setServer ($value) {
    static::$server = $value;
  }

  static public function setProtocol ($value) {
    static::$_protocol = $value;
  }

  static public function setSessionId ($sid) {
    static::$_sid = $sid;
  }

  static public function setDeveloperId ($devId) {
    static::$_devId = $devId;
  }


  protected function _getDefaultParams() {
    return array(
      'sid' => static::$_sid,
      'devId' => static::$_devId,
    );
  }
  /**
   * Sends the query off and returns the results
   * @param $cmd string
   *  Command
   * @param $params array
   *  Command Parameters
   * @return array
   *  Result of the call
   */
  protected function _query($cmd, $params=array()) {
    $result = array(
      'status' => TRUE,
    );
    $uri = static::$_protocol . static::$_server . '/' . $cmd;
    $http = new Zend_Http_Client();
    $http->setUri($uri);
    $http->setMethod(Zend_Http_Client::GET);
    $http->setConfig(array(
      'timeout' => 30,
      'useragent' => 'Ferrox External API Dispatcher',
    ));

    $params = $params + $this->_getDefaultParams();
    $http->setParameterGet($params);

    $response = $http->request();
    $status = $response->getStatus();
    if ((int) (floor($status/100)) !== 2) {
      $result['status'] = FALSE;
      $result['__httpStatus'] = $status;
    } else {
      $responseData = $this->_parseResponse($response);
      $result = $responseData + $result;
    }
    return $result;
  }


  protected function _parseResponse($response) {
    $body = $response->getBody();
    if ($body) {
      // This will throw a NOTICE if something goes horribly wrong
      $data = unserialize($body);
      if (($data === FALSE) && ($body !== 'b:0;') ) {
        return array('status' => FALSE, 'error' => array('Unknown Error' => 'An unknown error has occured'));
      }
      return $data;
    }
    return array();
  }


  protected function _isSuccess ($response) {
    if (is_array($response)
      && array_key_exists('status', $response)
      && $response['status'])
    {
      return TRUE;
    }
    return FALSE;
  }
}