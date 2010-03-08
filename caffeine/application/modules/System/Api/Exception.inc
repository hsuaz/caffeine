<?php
namespace Ferrox\Caffeine\System\Api;

class Exception
  extends \Ferrox\Caffeine\System\Exception
{
  protected $_apiErrorCode = '';
  protected $_apiErrorMsg = '';

  public function __construct ($apiCode, $apiMessage) {
    parent::__construct($apiMessage);
    $this->_apiErrorCode = $apiCode;
    $this->_apiErrorMsg = $apiMessage;
  }
  public function getError () {
    return array($_apiErrorCode => $_apiErrorMsg);
  }
}