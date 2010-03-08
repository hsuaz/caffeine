<?php
namespace Ferrox\System\Api\Output;

interface IOutputFormatter
{
  public function send($data = array());
}