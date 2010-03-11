<?php
declare(encoding='UTF-8');
namespace Ferrox\Module;

class User {
  const SYSTEM_ID = 'system_user';

  protected function staticRoute($path, $controller, $action) {
    return new \Zend_Controller_Router_Route_Static($path, array(
        'module' => 'User',
        'controller' => $controller,
        'action' => $action,
      ));
  }


  protected function dynamicRoute($path, $controller, $action, $defaults) {
    $defaults += array(
        'module' => 'User',
        'controller' => $controller,
        'action' => $action,
     );
     return new \Zend_Controller_Router_Route($path, $defaults);
  }


  public function getRoutes () {
    $routes = array();
    $routes['userProfile_2']       = $this->staticRoute('user', 'profile', 'index');
    $routes['userProfile']         = $this->staticRoute('profile', 'profile', 'index');
    $routes['userPage']            = $this->dynamicRoute('user/:username', 'profile', 'view', array('username' => ''));
    $routes['userLogin']           = $this->staticRoute('user/login', 'user', 'login');
    $routes['userRecoverPassword'] = $this->staticRoute('user/lostPassword', 'user', 'lostPassword');
    $routes['userLogout']          = $this->staticRoute('user/logout', 'user', 'logout');
    $routes['userRegister']        = $this->staticRoute('user/register', 'register', 'register');
    return $routes;
  }
}