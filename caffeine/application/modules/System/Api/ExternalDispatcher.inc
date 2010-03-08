<?php
namespace Ferrox\Caffeine\System\Api;


class ExternalDispatcher
  implements \Ferrox\System\Api\IDispatcher
{
  protected $_config = array();
  protected $_server = 'caffeine.localhost/internalApi.php';
  protected $_protocol = 'http://';

  /**
   * Default Constructior
   * @param $config Zend_Config | array
   */
  public function __construct ($config = array()) {
    if ($config instanceof \Zend_Config) {
      $this->_config = $config->toArray();
    } elseif (\is_array($config)) {
      $this->_config = $config;
    } else {
      throw new Exception('Invalid configuration.');
    }
  }


  /**
   * Initialization
   */
  protected function _init () {
    foreach ($this->_config as $key => $value) {
      $method = 'set' . \ucfirst($key);
      if (\method_exists($this, $method)) {
        $this->$method($value);
      }
    }
  }


  /**
   * Sets the target server for the request
   * @param $value url
   */
  protected function setServer ($value) {
    $this->_server = $value;
  }

  protected function setProtocol ($value) {
    $this->_protocol = $value;
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
  protected function _query($cmd, $params) {
    $result = array(
      'status' => TRUE,
    );
    $uri = $this->_protocol . $this->_server . '/' . $cmd;
    $http = new \Zend_Http_Client();
    $http->setUri($uri);
    $http->setMethod(\Zend_Http_Client::GET);
    $http->setConfig(array(
      'timeout' => 30,
      'useragent' => 'Ferrox External API Dispatcher',
    ));
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

  /**
   * Returns the basic
   * @return unknown_type
   */
  protected function _addRequestData () {
    $request = array(
      '__ipaddress' => $_SERVER['REMOTE_ADDR'],
    );
    return $request;
  }


  /**
   * Dispatch the request to the proper router
   * @param $route string
   *  Matched Route
   * @param $options array
   *  Routing OPtions
   * @param $request array
   *  Request Variables
   * @return array
   *  Result of the call
   */
  public function dispatch ($route, $options, $request) {
    $request['output'] = 'php';
    $request += $this->_addRequestData();

    // TODO: PROCESS OPTIONS HERE
    $result = $this->_query($route, $request);
    if (\array_key_exists('filterResult', $options)) {
      $className = $options['filterResult'];
      $className = ltrim($className, '\\');
      $filter = new $className;
      $result['result'] = $filter->filter($result['result']);
    }
    return $result;
  }
}