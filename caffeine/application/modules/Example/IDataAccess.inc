<?php
/**
 * Data Access Interface.
 *
 * This file defines all methods that the database access layer will present
 * to the class.
 *
 * Classes should only rely on the interface, and not the concrete interface
 * so that automated testing can stub out the interface.
 */
declare(encoding=UTF8);
namespace Ferrox\Caffeine\Example;

use
  Ferrox\Caffeine\System\DataAccess\IDataTransferObject,
  Ferrox\Caffeine\System\Mapper as Mapper;

/**
 * MySql Adaptor
 * @author IceWolf
 */
interface IDataAccess extends
  Mapper\IMapper
{
  /**
   * New method
   * @param $username
   * @return unknown_type
   */
  public function findByUsername ($username);



  /////////////////////////////////////////////////////////////////////////
  // Inherited
  /**
   * IMapper Interface
   * @author IceWolf
   *
   */
  /**
   * Creates a row
   * @param \Ferrox\Caffeine\System\DataAccess\IDataTransferObject $dto
   * @return
   *  int|FALSE Inserted Id or false on error
   */
  public function insert(DataAccess\IDataTransferObject $dto);


  /**
   * Soft deletes a row
   * @param \Ferrox\Caffeine\System\DataAccess\IDataTransferObject $dto
   * @return
   *  boolean Success status
   */
  public function delete(DataAccess\IDataTransferObject $dto);


  /**
   * Retrieves a row based on id
   * @param \Ferrox\Caffeine\System\DataAccess\IDataTransferObject $dto
   * @return
   *  \Ferrox\Caffeine\System\DataAccess\IDataTransferObject
   *  The data result in a normalized manner
   */
  public function retrieve(DataAccess\IDataTransferObject $dto);


  /**
   * Updates a row
   * @param \Ferrox\Caffeine\System\DataAccess\IDataTransferObject $dto
   * @return
   *  boolean Success status
   */
  public function update(DataAccess\IDataTransferObject $dto);
}