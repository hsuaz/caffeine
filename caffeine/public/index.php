<?php
require 'Bootstrap.php';


header('content-type: text/plain');
use Ferrox\Caffeine\Application as Application;
Application::setConfig($config);

header('content-type: text/plain');
$user = Application::user();
$user->load(14);
var_dump($user);
var_dump(serialize($user));
