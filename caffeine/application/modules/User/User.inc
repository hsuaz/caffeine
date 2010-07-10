<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User;

use
 \Ferrox\Caffeine\System\IEvent as IEvent,
 \Ferrox\System\IValidator as IValidator,
 \Ferrox\Caffeine\System\DataAccess\DataTransferObject as DataTransferObject;

class User
  extends
    \Ferrox\Caffeine\System\DataObject
{
  const STATUS_DELETED = 0x02;
  const STATUS_ACTIVE  = 0x01;
  const USERNAME_MIN = 5;
  const USERNAME_MAX = 15;


  protected $_usernameValidator = NULL;


  /**
   *
   * @var IDataAccess
   */
  protected $_dataAccess = NULL;


  /**
   * @var array Configuration Options;
   */
  protected $_config = array();

  /**
   * @var Event
   */
  protected $_event = NULL;


  /**
   * Default Constructor
   * @param $options array
   * @return unknown_type
   */
  public function __construct ($options = array()) {
    $this->_config = $options;
    $this->_parseConfiguration();
    $this->_initDefaults();
    $this->_assertInitialState();
    $this->reset();
  }


  /**
   * Check initial State assumptions
   */
  protected function _assertInitialState () {
    if ($this->_dataAccess === NULL || !($this->_dataAccess instanceof IDataAccess))
    {
      throw new Exception ('Database Access instance not inserted.');
    }
    if ($this->_event === NULL || !($this->_event instanceof IEvent))
    {
      throw new Exception ('Database Access instance not inserted.');
    }
  }


  /**
   * Parse configurations
   */
  protected function _parseConfiguration () {
    foreach ($this->_config as $key => $value) {
      $method = 'set' . \ucfirst($key);
      if (\method_exists($this, $method)) {
        $this->$method($value);
      }
    }
  }


  protected function _initDefaults () {
    if ($this->_usernameValidator === NULL) {

    }
    if ($this->_event === NULL) {
      $this->_event = new \Ferrox\Caffeine\System\Event();
    }
  }


  public function setEventHandler (IEvent $event) {
    $this->_event = $event;
  }


  public function setUsernameValidator (IValidator $usernameValidator) {
    $this->_usernameValidator = $usernameValidator;
  }


  public function setDataAccess (IDataAccess $dataAccess) {
    $this->_dataAccess = $dataAccess;
  }


  public function reset () {
    $this->_fields = array
    (
      'id' => 0,
      'username' => 'Guest',
      'passwordHash' => md5(''),
      'passwordSalt' => md5(time()),
      'status' => self::STATUS_ACTIVE,
    );
    $this->triggerEvent('reset');
  }


  public function load ($userId) {
    return $this->find($userId);
  }


  public function save () {
    $dto = new DataTransferObject (
      array(
        'id' => $this->id,
        'username' => $this->username,
        'passwordHash' => $this->passwordHash,
        'passwordSalt' => $this->passwordSalt,
        'status' => $this->status,
      )
    );
    $this->triggerEvent('update');
    $status = $this->_dataAccess->update();
    return $status;
  }


  public function delete () {
    $status = $this->_dataAccess->delete(new DataTransferObject(
      array(
        'id' => $this->id,
      )));

    if ($status) {
      $this->find($this->id);
    }
    return $status;
  }


  public function create () {
    $dto = new DataTransferObject(
      array(
        'username' => $this->username,
        'passwordHash' => $this->passwordHash,
        'passwordSalt' => $this->passwordSalt,
        'status' => $this->status,
      ));
    $status = $this->_dataAccess->insert();
    if ($status) {
      $this->id = $status;
    }
    $this->triggerEvent('create');
    return !empty($status);
  }


  public function find ($userId) {
    $this->reset();
    $dto = $this->_dataAccess->find($userId);
    if (count($dto) > 0) {
      $dto = $dto[0];
      $this->_populateFromDto($dto);
    }
    $success = ($this->id == $userId);
    if ($success) {
      $this->triggerEvent('load');
    }
    return $success;
  }


  protected function triggerEvent ($eventName, $params = array()) {
    $this->_event->trigger(_module::ID, $eventName, $this, $params);
  }


  public function findByUsername ($username) {
    $this->reset();
    $dto = $this->_dataAccess->findByUsername($username);
    if (count($dto) > 0) {
      $dto = $dto[0];
      $this->_populateFromDto($dto);
    }
    $success = ($this->username == $username);
    if ($success) {
      $this->triggerEvent('load');
    }
    return $success;
  }


  public function cyclePasswordSalt () {
    $oldSalt = $this->passwordSalt;
    $this->passwordSalt = md5($this->passwordSalt . time());
    $dto = new DataTransferObject (
      array(
        'id' => $this->id,
        'passwordSalt' => $this->passwordSalt,
      )
    );
    $this->triggerEvent('cyclePasswordSalt', array('oldPasswordSalt' => $oldSalt));
    $status = $this->_dataAccess->update($dto);
    return $status;
  }


  protected function _populateFromDto (\Ferrox\Caffeine\System\DataAccess\IDataTransferObject $dto) {
    $this->reset();
    $this->id = $dto->id;
    $this->username = $dto->username;
    $this->status = $dto->status;
    $this->passwordHash = $dto->passwordHash;
    $this->passwordSalt = $dto->passwordSalt;
  }


  public function validate() {

  }

  /**
   * Naming Conventions:
   * @param unknown_type $value
   */
  protected function validateHandle ($value) {
  }

  protected function suggestUsernameForHandle ($value) {
    $value = mb_strtolower($value, 'UTF-8');
    $value = \Normalizer::normalize($value, \Normalizer::FORM_D);
    $value = preg_replace('/^[a-z0-9]/', $value);
    return $value;
  }

  protected function _getUsernameValidator() {
    $usernameValidator = new \Ferrox\System\Text\UsernameValidator();
    return $usernameValidator;
  }

  protected function validateUsername ($value) {
    $regex = $this->_getUsernameValidator()->getRegex();
    return preg_match($regex, $value);
  }


  protected function preprocessUsername ($value) {
    $value = mb_strtolower($value, 'UTF-8');
    $value = \Normalizer::normalize($value, \Normalizer::FORM_D);
    return $value;
  }

  public function validatePasswordHash ($passwordHash) {
    $hash = md5($this->passwordHash . $this->salt);
    return ($passwordHash == $hash);
  }

  public function getRegistrationRequirements ($object, $params) {
    $validator = $this->_getUsernameValidator();
    $result = $params['result'];
    if (!isset($result->group)) {$result->group = array(); }
    $result->group[] = 'primary information';

    $result->username = array(
      '#type' => 'text',
      '#validationType' => 'regex',
      '#regex' => $validator->getRegex(),
      '#maxLength' => $validator->getMinLength(),
      '#minLength' => $validator->getMaxLength(),
      '#defaultValue' => '',
      '#required' => TRUE,
      '#name' => 'username',
      '#_apiReference' => 'Desired Username for Login',
      '#group' => 'primary information',
      '#disabled' => FALSE,
    );
    $result->emailAddress = array(
      '#type' => 'text',
      '#validationType' => 'email',
      '#maxLength' => 128,
      '#minLength' => 6,
      '#defaultValue' => '',
      '#required' => TRUE,
      '#name' => 'emailAddress',
      '#_apiReference' => 'Admin Contact Email',
      '#group' => 'primary information',
      '#disabled' => FALSE,
    );
  }

}