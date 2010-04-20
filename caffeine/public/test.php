<style>
.exception { background-color: #EE7777; }
.error { background-color: #EEAA77; }
.fail { background-color: #AA6666; }
.skip { background-color: #999933; }
.success { background-color: #77EE77; }
.testName { font-weight: bold; display: block;}
.testMessage { display: block; }
.errorTrace {
  white-space: pre;
  display: block;
  font-family: Consolas,"Courier New","san-serif";
  font-size: 8pt;
  background: #EEEEEE;
}


.clear {
  clear: both;
}

.test {
  padding-bottom: 0.2em;
  border-bottom: 1px solid #AAA;
  clear: both;
}
.testName {
  float: left;
  width: 80%;
}
.testResult {
  float: right;
  width: 15%;
}
.resultBlock { clear: both; }
.errorDetail {
  border: 1px solid red;
  background: #EE9999;
}
.testSuiteGroups {
  font-style: italic;
  margin-left: -1em;
  padding-left: 1em;
  background: #AAAACC;
  color: black;
}

.testSuiteTitle {
  margin-left: -1em;
  padding-left: 1em;
  background: #AAAACC;
  color: black;
  font-weight: bold;
}
.testSuite {
  border: 1px solid black;
  margin-right: 0;
  margin-top: 0.2em;
  margin-bottom: 0.2em;
  padding-right: 0;
  padding-left: 1em;
  padding-bottom: 0.2em;
  padding-right: 0.2em;
}
</style>
<?php
ini_set('html_errors', 1);
echo 'Testing Framework Results';

function _initAutoloaders () {
  // Minimize the search path
  set_include_path(
   realpath('../') . PATH_SEPARATOR .
   realpath('../library')
  );

  // Include the Autoloader
  require_once 'library/Zend/Loader/Autoloader.php';

  // Set up the autoloading functionality
  $autoloader = Zend_Loader_Autoloader::getInstance();

  // set up the Ferrox Caffeine Autoloader
  require_once 'application/Autoloader.inc';
  $caffeineAutoload = new \Ferrox\Caffeine\Autoloader();
  $autoloader->pushAutoloader($caffeineAutoload, 'Ferrox\Caffeine');

  require_once 'library/Ferrox/Autoloader.php';
  $ferroxAutoload = new \Ferrox\Autoloader();
  $autoloader->pushAutoloader($ferroxAutoload, 'Ferrox\\');
  $autoloader->pushAutoloader($ferroxAutoload, 'Zend_');
  $autoloader->pushAutoloader($ferroxAutoload, 'PHPUnit_');

  set_include_path(
     realpath('../')
  );
}
_initAutoloaders();


class HtmlDisplay
  implements
    PHPUnit_Framework_TestListener
{
  const CEXCEPTION = 'exception';
  const CFAIL      = 'fail';
  const CINCOMPLETE = 'incomplete';
  const CSKIP      = 'skip';
  const CSUCCESS     = 'success';
  const EXCEPTION = '<span class="exception">※ Exception</span>';
  const FAIL      = '<span class="fail">☒ Failed </span>';
  const INCOMPLETE = '<span class="incomplete">☐ Incomplete</span>';
  const SKIP      = '<span class="skip">◯ Skipped</span>';
  const START     = '<span class="start">.</span>';
  const SUCCESS     = '<span class="success">☑ Success</span>';
  public function __construct() {
  }
  protected $status;
  protected $statusCss;
  protected $errorDetail;
  protected $errorTrace;

  protected function status($status, $css) {
    $this->status = $status;
    $this->statusCss = $css;
  }


  protected function formatError(Exception $e, $desc) {
    $this->errorDetail = htmlspecialchars($desc, NULL, 'UTF-8');
    $this->errorTrace  = htmlspecialchars($e->getTraceAsString(), NULL, 'UTF-8');
  }


  public function addError(PHPUnit_Framework_Test $test, Exception $e, $time)
  {
    $this->status(self::EXCEPTION, self::CEXCEPTION);
    $this->formatError($e, $e->getMessage());
  }


  public function addFailure(PHPUnit_Framework_Test $test, PHPUnit_Framework_AssertionFailedError $e, $time)
  {
    $this->status(self::FAIL, self::CFAIL);
    $this->formatError ($e, $e->getDescription());
  }


  public function addIncompleteTest(PHPUnit_Framework_Test $test, Exception $e, $time)
  {
    $this->status(self::INCOMPLETE, self::CINCOMPLETE);
  }


  public function addSkippedTest(PHPUnit_Framework_Test $test, Exception $e, $time)
  {
    $this->status(self::SKIP, self::CSKIP);
  }


  public function startTest(PHPUnit_Framework_Test $test)
  {
    echo "<div class='test'>";
    echo "<div class='testName'>" . $test->getName() . "</div>";
    $this->status(self::SUCCESS, self::CSUCCESS);
    $this->errorDetail = '';
    $this->errorTrace = '';
  }

  public function endTest(PHPUnit_Framework_Test $test, $time) {
    echo "<div class='testResult {$this->statusCss}'>{$this->status}</div>";
    if (!empty($this->errorDetail)) {
      echo "<div class='resultBlock'>";
      echo "<div class='errorDetail'>{$this->errorDetail}</div>";
      echo "<div class='errorTrace'>{$this->errorTrace}</div>";
      echo "</div>";
    }
    echo "<div class='clear'></div>";
    echo "</div>";
  }

  public function startTestSuite(PHPUnit_Framework_TestSuite $suite) {
    echo "<div class='testSuite'>";
    echo "<div class='testSuiteTitle'>", $suite->getName(),  "</div>";
    echo "<div class='testSuiteGroups'>", implode(' ', $suite->getGroups()),  "</div>";
  }
  public function endTestSuite(PHPUnit_Framework_TestSuite $suite) {
    echo "</div>";
  }

}



$paths = array(
      '../library/Ferrox',
      '../application');
$testCollector = new PHPUnit_Runner_IncludePathTestCollector($paths,  array('test.php'));

$suite = new PHPUnit_Framework_TestSuite();
$suite->addTestFiles($testCollector->collectTests());
$result = new PHPUnit_Framework_TestResult;
$result->addListener(new HtmlDisplay());
$suite->run($result);
echo "END.";
?>

