<?php
namespace Ferrox\System\Api;

interface IDispatcher  {
  public function dispatch ($route, $routeConfig, $request);
}