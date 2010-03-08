<?php
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\News;

use
 \Ferrox\Caffeine\System\DataAccess\DataTransferObject as DataTransferObject;

class News
  extends
    \Ferrox\Caffeine\System\DataObject
{
  const STATUS_DELETED = 0x02;
  const STATUS_ACTIVE  = 0x01;
  const TITLE_MAX = 15;


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
      'title' => 'Untitled',
      'locale' => 'en_US',
      'datePublished' => date('Y-m-d H:i:s', time()),
      'author' => 0,
      'content' => '',
      'status' => self::STATUS_ACTIVE,
    );
  }


  public function load ($newsId) {
    return $this->find($newsId);
  }


  public function save () {
    $status = $this->_dataAccess->update(new DataTransferObject (
      array(
        'id' => $this->id,
        'title' => $this->title,
        'locale' => $this->locale,
        'datePublished' => $this->datePublished,
        'author' => $this->author,
        'content' => $this->content,
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
        'title' => $this->title,
        'locale' => $this->locale,
        'datePublished' => $this->datePublished,
        'author' => $this->author,
        'content' => $this->content,
        'status' => $this->status,
      )));
    if ($status) {
      $this->id = $status;
    }
    return !empty($status);
  }


  public function find ($id) {
    $this->reset();
    $dto = $this->_dataAccess->find($id);
    if (count($dto) > 0) {
      $dto = $dto[0];
      $this->_populateFromDto($dto);
    }
    return ($this->id == $id);
  }


  public function getLatest ($count = 10, $locale = array()) {
    $this->reset();
    $dto = $this->_dataAccess->getLatestNews($count, $locale);
    $results = array();
    if (count($dto) > 0) {
      foreach ($dto as $item) {
        $results[] = $item->toArray();
      }
    }

    return $results;
  }


  public function validateTitle ($value) {
    $regex = '/^[a-z0-9]{1,' . self::TITLE_MAX . '}$/u';
    return preg_match($regex, $value);
  }
  public function validateLocale ($value) {
    $regex = '/[a-z]{2}_[A-Z]{2,3}$/u';
    return preg_match($regex, $value);
  }

  protected function preprocessTitle($value) {
    $value = mb_strtolower($value, 'UTF-8');
    $value = \Normalizer::normalize($value, \Normalizer::FORM_D);
    return $value;
  }
  protected function preprocessContent($value) {
    $value = mb_strtolower($value, 'UTF-8');
    $value = \Normalizer::normalize($value, \Normalizer::FORM_D);
    return $value;
  }

}