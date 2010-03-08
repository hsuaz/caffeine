<?php
declare(encoding=UTF8);
namespace Ferrox\Caffeine\News;
use
  Ferrox\Caffeine\System\DataAccess\IDataTransferObject;

/**
 * MySql Adaptor
 * @author IceWolf
 */
class DataAccess
  extends
    \Ferrox\Caffeine\System\Mapper\Mapper
  implements
    \Ferrox\Caffeine\News\IDataAccess
{
  protected $_tableName = 'news';
  protected $_tablePrimaryKey = 'news_id';

  protected function _populateDataTransferObject ($databaseRow) {
    // Normalize the database results
    return new \Ferrox\Caffeine\System\DataAccess\DataTransferObject(
      array(
        'id' => $databaseRow['news_id'],
        'title' => $databaseRow['title'],
        'locale' => $databaseRow['locale'],
        'datePublished' => $databaseRow['published'],
        'author' => $databaseRow['author'],
        'content' => $databaseRow['content'],
        'status' => $databaseRow['status'],
      )
    );
  }

  private function _getDtoArray ($dto, $defaults=array()) {
    $data = array();
    if (isset($dto->id)) {
      $data['news_id'] = $dto->id;
    }

    if (isset($dto->title)) {
      $data['title'] = $dto->title;
    }

    if (isset($dto->status)) {
      $data['status'] = $dto->status;
    }

    if (isset($dto->locale)) {
      $data['locale'] = $dto->locale;
    }

    if (isset($dto->datePublished)) {
      $data['published'] = $dto->datePublished;
    }

    if (isset($dto->author)) {
      $data['author'] = $dto->author;
    }

    if (isset($dto->content)) {
      $data['content'] = $dto->content;
    }

    // Assign Defaults
    foreach ($defaults as $key=>$value) {
      if (!\array_key_exists($key, $data)) {
        $data[$key] = $value;
      }
    }
    return $data;
  }

  /**
   * Datamapper for the create function
   * @param IDataTransferObject $dto
   * @return array
   */
  protected function _insert (IDataTransferObject $dto) {
    if (!isset ($dto->title) || !isset ($dto->locale) || !isset($dto->author)) {
      return FALSE;
    }
    $defaults = array(
      'content' => '',
      'status' => News::STATUS_ACTIVE,
    );
    $data = $this->_getDtoArray($dto, $defaults);
    $data['published'] = date('Y-m-d H:i:s', time());
    return $data;
  }

  protected function _delete (IDataTransferObject $dto) {
    if (!isset ($dto->id)) {
      return FALSE;
    }
    $data = $this->_getDtoArray($dto);
    $data['status'] = News::STATUS_DELETED;
    return $data;
  }

  /**
   *
   * @param IDataTransferObject $dto
   * @return unknown_type
   */
  protected function _update (IDataTransferObject $dto) {
    if (!isset ($dto->id)) {
      return FALSE;
    }
    $data = $this->_getDtoArray($dto);
    return $data;
  }

  public function getLatestNews ($count = 10, $locales = array()) {
    $select = $this->_dbR->select()
      ->from($this->_tableName)
      ->limit($count);
    if (!empty ($locales)) {
      $select->where('locale in (?)', $locales);
    }
    $tableResult = $this->_dbR->fetchAll($select);
    $result = \array_map(array($this, '_populateDataTransferObject'), $tableResult);
    return $result;
  }
}