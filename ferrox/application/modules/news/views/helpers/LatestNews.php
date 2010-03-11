<?php
declare(encoding='UTF-8');
namespace Ferrox\News\View\Helper;

class News_LatestNews
  extends\Zend_View_Helper_Abstract
{
  public function latestNews ($count)
  {
    $newsApi = new \FurAffinity\Api\News();
    $news = $newsApi->getLatest(5);
    $this->view->news = $news;
    return $this->view->render('newsLatestNews.phtml');
  }
}