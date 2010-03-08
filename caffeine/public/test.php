<?php
ini_set('html_errors', 1);
require_once 'PHPUnit/Framework.php';
echo 'Testing Framework Results';

set_include_path(
  get_include_path()
  . PATH_SEPARATOR . '/www/Caffeine/application/modules'
  . PATH_SEPARATOR . '/www/Caffeine/library/');

function __autoload($classname) {
  \var_dump ($classname);
  $name = \explode('\\', $classname);
  if ($name[0] == 'Ferrox' && $name[1] == 'Caffeine') {
    unset($name[0]);
    unset($name[1]);
    $classname = \implode(DIRECTORY_SEPARATOR, $name);
    require_once($classname . '.inc');
  }
  $name = \explode('_', $classname);
  if ($name[0] == 'Zend' || $name[0] == 'PHPUnit'){
    $classname = \str_replace('_', DIRECTORY_SEPARATOR, $classname);
    \var_dump ($classname);
    require_once($classname . '.php');
  }
}

function formatError ($error) {
  $output = '<div class="error">';
  $output .= '<span class="testName">' . $error->failedTest()->toString() . '</span>';
  $output .= '<span class="testMessage">' . $error->exceptionMessage() . '</span>';
  $output .= '<span class="testTrace">' . $error->thrownException()->getTraceAsString() . '</span>';
  $output .= '</div>';
  return $output;
}
function formatFailure ($error) {
  $output = '<div class="failure">';
  $output .= '<span class="testName">' . $error->failedTest()->toString() . '</span>';
  $output .= '<span class="testMessage">' . $error->exceptionMessage() . '</span>';
  $output .= '<span class="testTrace">' . $error->thrownException()->getTraceAsString() . '</span>';
  $output .= '</div>';
  return $output;
}
function formatPassed ($name) {
  $output = '<div class="success">';
  $output .= '<span class="testName">' . $name . "</span>";
  $output .= '</div>';
  return $output;
}
//require_once('User/test/MapperMySqlTest.php');
$suite = new PHPUnit_Framework_TestSuite();
$suite->addTestFile('/www/Caffeine/application/modules/User/test/MapperMySqlTest.php');
$result = $suite->run();

foreach ($result->errors() as $error) {
  echo formatError($error);
}
foreach ($result->failures() as $failure) {
  echo formatFailure($failure);
}
foreach ($result->skipped() as $skipped) {
  var_dump($skipped);
}

foreach ($result->passed() as $testname => $passed) {
  echo formatPassed($testname);
}

echo "END.";
?>
<style>
.error { background-color: #EE7777; }
.failure { background-color: #AA6666; }
.success { background-color: #77EE77; }
.testName { font-weight: bold; display: block;}
.testMessage { display: block; }
.testTrace { white-space: pre; display: block;}


</style>