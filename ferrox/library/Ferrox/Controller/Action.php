<?php
declare(encoding="UTF-8");
namespace Ferrox\Controller;

class Action
  extends \Zend_Controller_Action
{
  /**
   * Dispatch the requested action
   *
   * @param string $action Method name of action
   * @return void
   */
  public function dispatch($action)
  {
    // Notify helpers of action preDispatch state
    $this->_helper->notifyPreDispatch();

    $this->preDispatch();
    if ($this->getRequest()->isDispatched()) {
      if (null === $this->_classMethods) {
        $this->_classMethods = \get_class_methods($this);
      }

      if ($this->getRequest()->isPost()) {
        $action = $action . 'Post';
      }

      if ($this->getInvokeArg('useCaseSensitiveActions') || \in_array($action, $this->_classMethods)) {
        if ($this->getInvokeArg('useCaseSensitiveActions')) {
            \trigger_error('Using case sensitive actions without word separators is deprecated; please do not rely on this "feature"');
        }
        $this->$action();
      } else {
        $this->__call($action, array());
      }
      $this->postDispatch();
    }

    // whats actually important here is that this action controller is
    // shutting down, regardless of dispatching; notify the helpers of this
    // state
    $this->_helper->notifyPostDispatch();
  }
}
