<?php

// Define path to application directory
defined('APPLICATION_PATH')
    || define('APPLICATION_PATH', realpath(dirname(__FILE__) . '/../application'));

// Define application environment
defined('APPLICATION_ENV')
    || define('APPLICATION_ENV', (getenv('APPLICATION_ENV') ? getenv('APPLICATION_ENV') : 'development'));

// Ensure library/ is on include_path
set_include_path(implode(PATH_SEPARATOR, array(
    realpath(APPLICATION_PATH . '/../library'),
    get_include_path(),
)));

/** Zend_Application */
require_once 'Zend/Application.php';



// Create application, bootstrap, and run
$application = new Zend_Application(
    APPLICATION_ENV,
    APPLICATION_PATH . '/config/application.ini'
);

require_once 'Autoloader.php';
require_once 'ModuleAutoloader.php';
$defaultAutoloader = new Autoloader();
$application->getAutoloader()->pushAutoloader($defaultAutoloader, 'FurAffinityApi_');
$application->getAutoloader()->pushAutoloader($defaultAutoloader, 'Zend_');
$moduleAutoloader = new ModuleAutoloader();
$application->getAutoloader()->pushAutoloader($moduleAutoloader, 'Module_');
$application->bootstrap()
            ->run();
