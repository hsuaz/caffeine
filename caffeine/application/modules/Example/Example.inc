<?php
/**
 * This class is used internally by teh module, and only the module.
 * Other modules should only interact with your module through the
 * InternalAPI exposed.
 *
 */
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\Example;

use
 \Ferrox\Caffeine\System\DataAccess\DataTransferObject as DataTransferObject;

class Example
  extends
    \Ferrox\Caffeine\System\DataObject
{
  /**
   * Best Practise:
   * Inject a concrete Data Access class for testing purposes.
   * This should be instantiated through the Application Configuration.
   * @var IDataAccess
   */
  protected $_dataAccess = NULL;


  /**
   * Default Constructor
   * @param $options array
   * @return unknown_type
   */
  public function __construct ($options = array()) {
    // Verify the dataaccess has been provided.
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


  /**
   * Resets the fields to default state.
   * These are the fields that are accessed by $this->__get(); aka $this->field
   * @return unknown_type
   */
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


  /**
   * TODO: Allow the base DataObject class call the functions on the
   *       validation here to handle validation of the object data.
   *       planned implementation is a isValid() call that will
   *       call all validation functions defined.
   * @param $value
   * @return unknown_type
   */
  public function validateTitle ($value) {
    $regex = '/^[a-z0-9]{1,' . self::TITLE_MAX . '}$/u';
    return preg_match($regex, $value);
  }
  public function validateLocale ($value) {
    $regex = '/[a-z]{2}_[A-Z]{2,3}$/u';
    return preg_match($regex, $value);
  }


  /**
   * Preprocess data entering the module.
   * These are called when a field is set via $this->field = value.
   * If the fieldname matches a preprocess function, it will be applied
   * before setting the value.
   *
   * This is useful for normalizing data.
   *
   * @param $value
   * @return unknown_type
   */
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