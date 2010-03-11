<?php
declare(encoding='UTF-8');

class User_RegisterController
  extends \Ferrox\Controller\Action
{
  // String Constants centralized
  const MSG_REGISTER_USERNAME = 'system_user:register:Username';
  const MSG_REGISTER_EMAIL = 'system_user:register:Email';
  const MSG_REGISTER_REGISTER = 'system_user:register:Register';

  public function registerActionForm () {
    $action = $this->getFrontController()->getRouter()->assemble(array(), 'userRegister');
    $form = new Zend_Form();

    $form->setMethod(Zend_Form::METHOD_POST);
    $form->addPrefixPath('Ferrox_', 'Ferrox/');
    $form->setAction($action);
    $form->addElementPrefixPath('\Ferrox\Filter\\', 'Ferrox/Filter', 'filter');
    $form->addElement('text', 'username', array(
      'validators' => array(
        array('stringLength', false, array(3, 20)),
      ),
      'filters' => array('NormalizeUtf8'),
      'required' => TRUE,
      'label' => static::MSG_REGISTER_USERNAME,
    ));
    $form->addElement('text', 'email', array(
      'label' => static::MSG_REGISTER_EMAIL,
      'required' => TRUE,
    ));
    $form->addElement('submit', 'register', array(
      'label' => static::MSG_REGISTER_REGISTER,
      ));
    return $form;
  }


  public function registerAction () {
   $this->view->form = $this->registerActionForm();
  }

  public function registerActionPost () {
    var_dump($this->getRequest()->getPost());
    $form = $this->registerActionForm();
    if (!$form->isValid($this->getRequest()->getPost())) {
      $this->view->form = $form;
      return;
    }

    $values = $form->getValues();
    // Authenticate
  }
}