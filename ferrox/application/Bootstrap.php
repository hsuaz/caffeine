<?php

class Bootstrap
  extends Zend_Application_Bootstrap_Bootstrap
{

  protected function _initFurAffinityApi () {
    $systemSession = new Zend_Session_Namespace('system');
    $remoteSid = isset($systemSession->remoteSession) ? $systemSession->remoteSession : NULL;

    FurAffinityApi_AbstractBase::setSessionId($remoteSid);
    FurAffinityApi_AbstractBase::setDeveloperId('00000000-0000-5000-0000-000000000000');
  }

  protected function _initTimezone() {
    date_default_timezone_set('America/Los_Angeles');
  }

  protected function _initDefaultResourceLoader () {
    $resourceLoader = new Zend_Application_Module_Autoloader(array(
      'namespace' => 'Default',
      'basePath' => dirname(__FILE__) . '/modules/default',
    ));
  }

  protected function _initViewEnvironment () {
    $this->bootstrap('view');
    $view = $this->getPluginResource('view')->getView();
    $view->setEncoding('UTF-8');
    $view->headMeta('', 'UTF-8', 'charset');
    $request = new Zend_Controller_Request_Http();
  }

  protected function _initRouting () {
    $router = $this->getPluginResource('Router')->getRouter();
    $user = new Module_User();
    $router->addRoutes($user->getRoutes());
//    $router->addRoute('userLogin',
//      new Zend_Controller_Router_Route_Static('user/login', array('module' => 'user', 'controller' => 'user', 'action' => 'login')));
//    $router->removeDefaultRoutes();
    return $router;
  }

  protected function _initRequest() {
    $this->bootstrap('frontController');
    $front = $this->getResource('frontController');
    $front->addModuleDirectory(APPLICATION_PATH . '/modules');
    $request = new Zend_Controller_Request_Http();
    $request->setBaseUrl('');
    $front->setRequest($request);
    return $request;
  }

  /**
   * @todo: Make language dynamic on request
   * @return unknown_type
   */
  protected function _initResponse() {
    $this->bootstrap('frontController');
    $response = new Zend_Controller_Response_Http();
    $response->setHeader('language', 'en')
      ->setHeader('content-language', 'en')
      ->setHeader('Content-Type', 'text/html; charset=UTF-8');
    $this->getPluginResource('FrontController')->getFrontController()->setResponse($response);
  }

  protected function _initView () {
    $view = new Zend_View();
    $view->doctype('XHTML1_STRICT');
    $view->headTitle('Ferrox: FurAffinity 2.0');
    $view->addBasePath(APPLICATION_PATH . '/modules/user/views', 'Ferrox_User_View');
    $view->addBasePath(APPLICATION_PATH . '/modules/news/views', 'Ferrox_News_View');
    $viewRenderer = Zend_Controller_Action_HelperBroker::getStaticHelper('ViewRenderer');
    $viewRenderer->setView($view);
    return $view;
  }
}