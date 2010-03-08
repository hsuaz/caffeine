<?php
declare(encoding=UTF8);
namespace Ferrox\Caffeine\User;

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
  public function findByUsername ($username);
}