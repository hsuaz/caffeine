<?php
declare(encoding="UTF-8");
namespace Ferrox\Caffeine\System\Api;



interface IModule {
  public function getVersion ();
  public function getDescription ();
  public function getId ();
  public function getApi ();
  public function loadRoutes ($section);
}
