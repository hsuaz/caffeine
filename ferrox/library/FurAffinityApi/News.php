<?php
class FurAffinityApi_News
  extends FurAffinityApi_AbstractBase
{

  public function getLatest($count = 5, $locales = array()) {
    $params = array(
      'count' => (int) $count,
      'locales' => implode(',', $locales),
    );
    $result = $this->_query('news/getLatest', $params);
    if (!$this->_isSuccess ($result)) {
      return NULL;
    }

    return $result['result']['news'];
  }
}