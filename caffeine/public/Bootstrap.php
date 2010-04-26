<?php
/**
 * General Bootstrap Logic
 * Common to all bases.
 * Specific logic will be handled with Configuration Options.
 */

use Ferrox\System\Net as Net;


/**
 * General Bootstrap Class
 * @author IceWolf
 *
 */
class Bootstrap {
  /**
   * Autoloader
   * @var \Zend_Loader_Autoloader
   */
  protected $_autoloader = NULL;


  /**
   * Configuration
   * @var \Zend_Config
   */
  protected $_config = NULL;


  /**
   * Has the initialization been run
   * @var boolean
   */
  protected $_initRun = FALSE;


  /**
   * Name of the PHP script
   * @var string
   */
  protected $_scriptName = '';


  /**
   * Application level configuration
   * @var \Zend_Config
   */
  protected $_applicationConfig = NULL;
  /**
   * Default Constructor
   * @param $configFile string
   *  Configuration file
   * @param $configSection string
   *  Section to load
   */
  public function __construct($configFile, $configSection) {
    $this->_initAutoloaders();
    $this->_config = new \Zend_Config_Ini($configFile, $configSection);
    if ($this->_config->siteOffline) {
      $this->_dieWithCode(\Ferrox\System\Net\HttpStatus::HTTP_SITE_OFFLINE);
    }
  }


  /**
   * Configuration Accessor
   * @return Zend_Config
   */
  public function getConfig () {
    return $this->_config;
  }


  /**
   * Set the script name (prevent autodetect)
   * @param $value
   */
  public function setScriptName ($value) {
    $this->_scriptName = $value;
  }


  /**
   * General initialization routine
   */
  protected function _init() {
    if ($this->_initRun) {
      return;
    }

    $this->_initRun = TRUE;
    $this->_cleanEnvironment();
    $this->_initApplication();
  }



  public function setApplicationConfig(\Zend_Config $config) {
    $this->_applicationConfig = $config;
  }


  protected function _initApplication() {
    if ($this->_applicationConfig instanceof \Zend_Config) {
      $application = \Ferrox\Caffeine\Application::getInstance();
      $application->setConfig($this->_applicationConfig);
      $application->init();
    }
  }

  /**
   * Initialize the autoloaders
   */
  protected function _initAutoloaders () {
    // Minimize the search path
    set_include_path(
     realpath('../') . PATH_SEPARATOR .
     realpath('../library')
    );

    // Include the Autoloader
    require_once 'library/Zend/Loader/Autoloader.php';

    // Set up the autoloading functionality
    $autoloader = Zend_Loader_Autoloader::getInstance();

    // set up the Ferrox Caffeine Autoloader
    require_once 'application/Autoloader.inc';
    $caffeineAutoload = new \Ferrox\Caffeine\Autoloader();
    $autoloader->pushAutoloader($caffeineAutoload, 'Ferrox\Caffeine');

    require_once 'library/Ferrox/Autoloader.php';
    $ferroxAutoload = new \Ferrox\Autoloader();
    $autoloader->pushAutoloader($ferroxAutoload, 'Ferrox\\');
    $autoloader->pushAutoloader($ferroxAutoload, 'Zend_');

    $this->_autoloader = $autoloader;
    set_include_path(
       realpath('../')
    );
  }


  /**
   * Parse the request URI to populate the 'PATH_INFO'
   */
  protected function parseRequestUri() {
    $_SERVER['PATH_INFO'] = '';
    $uri = \parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
    if (!empty($this->_scriptName)) {
      $filename = str_replace('.', '\.', $this->_scriptName);
    } else {
      $trace = \debug_backtrace();
      $item = \array_pop($trace);
      $filename = str_replace('.', '\.', basename($item['file']));
    }

    $inPath = preg_match("/$filename(.*)/", $uri, $matches);
    if ($inPath) {
      $_SERVER['PATH_INFO'] = $matches[1];
    }
  }


  /**
   * Clean the _SERVER variables
   */
  protected function _cleanServer() {
    $this->_cleanHost();
    if (!isset($_SERVER['PATH_INFO'])) {
      $this->parseRequestUri();
    }

    if (\strlen($_SERVER['PATH_INFO']) === 0) {
      return;
    }

    if ($_SERVER['PATH_INFO'][0] === '/') {
      $_SERVER['PATH_INFO'] = substr($_SERVER['PATH_INFO'], 1);
    }
  }


  /**
   * Prepares the operating environment variables
   */
  protected function _cleanEnvironment () {
    $this->_cleanServer();
    $_REQUEST = $this->_cleanArray($_REQUEST);
  }


  /**
   * Cleans the HTTP_HOST to a normalized state
   */
  protected function _cleanHost() {
    $hostName = $_SERVER['HTTP_HOST'];
    $regex = '/^[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9](\.[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])+$/D';
    if (!\preg_match($regex, $hostName)) {
      $this->_dieWithCode(Net\HttpStatus::HTTP_BAD_REQUEST);
    }
    $hostName = \Normalizer::normalize(\mb_strtolower(\idn_to_utf8($hostName), 'UTF-8'), Normalizer::FORM_D);
    $_SERVER['HTTP_HOST'] = $hostName;
  }


  /**
   * Validate the request.
   * THis will die if the host doesn't match the configuration
   */
  protected function _validateRequest () {
    if ($this->_config->bootstrap->host->validate) {
      if ($_SERVER['HTTP_HOST'] !== $config->bootstrap->host->name) {
        $this->_dieWithCode(Net\HttpStatus::HTTP_BAD_REQUEST);
      }
    }
  }


  /**
   * Exec
   * @return unknown_type
   */
  public function run() {
    $this->_init();
  }


  /* ------------
   * STUFF TO REFACTOR OUT :
   * ------------
   */

  /**
   * Kill the sequence with an Http status code
   * @param $httpCode int
   *  HTTP Status Code
   */
  protected function _dieWithCode($httpCode) {
    Net\HttpStatus::send($httpCode);
    echo "HTTP/1.1 $httpCode " . Net\HttpStatus::getDescription($httpCode);
    die;
  }


  /**
   * Check the encoding of the input
   * @param $value string
   * @return boolean
   *   Valid UTF-8 data
   */
  protected function _validEncoding($value) {
    return mb_check_encoding($value, 'UTF-8');
  }


  /**
   * Normalize the string to UTF-8 NFD
   * @param $value string
   * @return string
   */
  protected function _normalizeString($value) {
    $value = \Normalizer::normalize($value, Normalizer::FORM_D);
    return $value;
  }


  /**
   * Cleans an array to normalize the strings
   * @param $input array
   * @return array
   */
  protected function _cleanArray($input) {
    $result = array();
    foreach ($input as $key => $value) {
      $key = trim($key);
      if (!$this->_validEncoding($key) || !$this->_validEncoding($value) || empty($key)) {
        $this->_dieWithCode(Net\HttpStatus::HTTP_BAD_REQUEST);
      }

      $key = $this->_normalizeString($key);
      if (\is_array($value)) {
        $result[$key] = $this->_cleanArray($value);
      } else {
        $value = $this->_normalizeString($value);
        $value = trim($value);
        $result[$key] = $value;
      }
    }
    return $result;
  }

}



