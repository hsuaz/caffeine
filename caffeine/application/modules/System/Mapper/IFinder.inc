<?php
declare(encoding=UTF8);
namespace Ferrox\Caffeine\System\Mapper;

/**
 * IFinder Interface
 * @author IceWolf
 *
 */
interface IFinder
{
  /**
   * Returns a row based on the id passed in
   * @param $id int Id
   * @return \Ferrox\Caffeine\System\DataAccess\IDataTransferObject
   */
  public function find ($id);
}