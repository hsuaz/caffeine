<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User;

use
 \Ferrox\Caffeine\System\DataAccess\DataTransferObject as DataTransferObject;

class User
  extends
    \Ferrox\Caffeine\System\DataObject
{
  const STATUS_DELETED = 0x02;
  const STATUS_ACTIVE  = 0x01;
  const USERNAME_MIN = 5;
  const USERNAME_MAX = 15;


  /**
   *
   * @var IDataAccess
   */
  protected $_dataAccess = NULL;


  /**
   * Default Constructor
   * @param $options array
   * @return unknown_type
   */
  public function __construct ($options = array()) {
    if (
        !(    (array_key_exists('dataAccess', $options))
           && ($options['dataAccess'] instanceof IDataAccess)
         )
       )
    {
      throw new Exception ('Database Access instance not inserted.');
    }

    $this->_dataAccess = $options['dataAccess'];
    $this->reset();
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
  }


  public function load ($userId) {
    return $this->find($userId);
  }


  public function save () {
    $status = $this->_dataAccess->update(new DataTransferObject (
      array(
        'id' => $this->id,
        'username' => $this->username,
        'passwordHash' => $this->passwordHash,
        'passwordSalt' => $this->passwordSalt,
        'status' => $this->status,
      )
    ));
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
    $status = $this->_dataAccess->insert(new DataTransferObject(
      array(
        'username' => $this->username,
        'passwordHash' => $this->passwordHash,
        'passwordSalt' => $this->passwordSalt,
        'status' => $this->status,
      )));
    if ($status) {
      $this->id = $status;
    }
    return !empty($status);
  }


  public function find ($userId) {
    $this->reset();
    $dto = $this->_dataAccess->find($userId);
    if (count($dto) > 0) {
      $dto = $dto[0];
      $this->_populateFromDto($dto);
    }
    return ($this->id == $userId);
  }


  public function findByUsername ($username) {
    $this->reset();
    $dto = $this->_dataAccess->findByUsername($username);
    if (count($dto) > 0) {
      $dto = $dto[0];
      $this->_populateFromDto($dto);
    }

    return ($this->username == $username);
  }


  public function cyclePasswordSalt () {
    $this->passwordSalt = md5($this->passwordSalt . time());
    $status = $this->_dataAccess->update(new DataTransferObject (
      array(
        'id' => $this->id,
        'passwordSalt' => $this->passwordSalt,
      )
    ));
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


  public function validateHandle ($value) {
    // Language Coverage
    // FRENCH, GERMAN, ENGLISH, SPANISH, ICELANDIC, NORWEGIAN, SWEDISH, FINNISH
    // LATVIAN, LITUANIAN, POLISH, CZECH, DUTCH, ROMANIAN, PORTUGUESE,
    // TURKISH, SLOVAK
    $regex = '/^(?:';
    $regex .= '(?:[a-z0-9\xDF\xE6\xF0\xF8\xFE\x{0153}\x{0142}\x{0131}])'; // ss, ae, eth, o/, thorn, oe, l/, dotless-i
    $regex .= '|(?:[aeiouy][\x{0308}\x{0300}\x{0302}])'; // Umlaut, grave, acute, circumplex
    $regex .= '|(?:[aeiouynlrsz][\x{0301}])'; // Acute
    $regex .= '|(?:[aeioun][\x{0303}])'; // Tilde
    $regex .= '|(?:[cgklns][\x{0327}])'; // Cedilla
    $regex .= '|(?:[aou][\x{030A}])'; // ring
    $regex .= '|(?:[cdensrtz][\x{030C}])'; // Hacek
    $regex .= '|(?:[aeiu][\x{0328}])'; // Ogonek
    $regex .= '|(?:[aeiu][\x{0304}])'; // Overbar
    $regex .= '|(?:[ez][\x{0307}])'; // Overdot
    $regex .= '|(?:[st][\x{0326}])'; // Comma
    $regex .= '|(?:[ag][\x{0306}])'; // Breve
    $regex .= '){' . USERNAME_MIN . ',' . USERNAME_MAX . '}$/u';
  }

  public function suggestUsernameForHandle ($value) {
    $value = mb_strtolower($value, 'UTF-8');
    $value = \Normalizer::normalize($value, \Normalizer::FORM_D);
    $value = preg_replace('/^[a-z0-9]/', $value);
    return $value;
  }

  public function validateUsername ($value) {
    $regex = '/^[a-z0-9]{' . self::USERNAME_MIN . ',' . self::USERNAME_MAX . '}$/u';
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

}