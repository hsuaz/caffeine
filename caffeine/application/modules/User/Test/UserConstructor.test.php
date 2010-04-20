<?php
/**
 * User Object Class
 */
declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User;

/**
 * @backupGlobals enabled
 * @backupStaticAttributes enabled
 * @group User
 */
class UserConstructorTest extends \PHPUnit_Framework_TestCase
{
  /**
   * @test
   * @expectedException \PHPUnit_Framework_Error_Warning
   * @dataProvider constructorTypeHintErrorProvider
   * @name Invalid Type Hints
   */
  public function constructorTypeHintError ($options) {
    $user = new User($options);
  }

  public function constructorTypeHintErrorProvider () {
    return array(
      array(NULL),
      array(1),
      array("string"),
      array(TRUE),
    );
  }


  /**
   * @test
   * @expectedException \Ferrox\Caffeine\User\Exception
   * @dataProvider invalidConstructorOptionsProvider
   */
  public function invalidConstructorOptions ($options) {
    // Invalid Options
    $user = new User($options);
  }

  public function invalidConstructorOptionsProvider() {
    return array(
      array(array()),
      array(new \stdClass()),
      array(array('dataProvider' => NULL)),
      array(array('dataProvider' => 1)),
      array(array('dataProvider' => "String")),
      array(array('dataProvider' => array())),
      array(array('dataProvider' => (object) array())),
      array(array('notDataProvider' => NULL)),
    );
  }


  /**
   * @test
   */
  public function validContructor () {
    $dataAccess = $this->getMock('Ferrox\Caffeine\User\IDataAccess');
    $options = array('dataAccess' => $dataAccess);
    $user = new User($options);
  }
}