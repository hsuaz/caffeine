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

    // @TODO Add to the API Layer
    if (!\is_array($news)) {
      $news = array(
        'locale' => 'en_US',
        'id' => 0,
        'title' => 'News is Unavailable',
        'datePublished' => '1999-12-31 23:59:59',
        'content' => 'News Server is currently unavailable.',
        'author' => array(
          'username' => 'Unknown',
        )
      );
    }

    $this->view->news = $news;
    return $this->view->render('newsLatestNews.phtml');
  }
}