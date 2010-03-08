<?php
declare(encoding=UTF8);
namespace Ferrox\Caffeine\System\Mapper;
use
  Ferrox\Caffeine\System\DataAccess as DataAccess;


/**
 * IMapper Interface
 * @author IceWolf
 *
 */
interface IMapper {
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