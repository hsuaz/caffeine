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
class UserTest extends \PHPUnit_Framework_TestCase
{
  protected $user = NULL;

  /**
   * @setUp
   */
  public function setUp() {
     $dataAccess = $this->getMock('Ferrox\Caffeine\User\IDataAccess');
     $options = array('dataAccess' => $dataAccess);
    $this->user = new User($options);
  }

  /**
   * @tearDown
   */
  public function tearDown() {
    $this->user = NULL;
  }

  protected function assertInitialState() {
    $this->assertSame(0, $this->user->id);
    $this->assertSame('Guest', $this->user->username);
    $this->assertSame(md5(''), $this->user->passwordHash);
    $this->assertType('string', $this->user->passwordSalt);
    $this->assertSame(User::STATUS_ACTIVE, $this->user->status);
  }

  /**
   * @test
   */
  public function initialState() {
    $this->assertInitialState();
  }


  /**
   * @test
   */
  public function setId() {
    // $this->markTestIncomplete();
    $this->markTestSkipped();
  }

  /**
   * @depends setId
   * @test
   */
  public function reset() {
    // Set all the vars and see if they are reverted;
    $this->user->id = 1;
    $this->user->reset();
    $this->assertInitialState();
  }
}