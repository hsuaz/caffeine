<?php

class Ferrox_News_View_Helper_LatestNews
  extends Zend_View_Helper_Abstract
{

  public function latestNews ($count)
  {
    $newsApi = new FurAffinityApi_News();
    $news = $newsApi->getLatest(5);
    $this->view->news = $news;
    return $this->view->render('newsLatestNews.phtml');
  }
}