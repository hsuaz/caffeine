<?php
class SMS64 {
 static function encode ($input) {
   $input = strtoupper($input);
   $input = preg_replace('[^\x32-\x95]', '', $input);
   $output = '';
   $length = strlen($input);
   for ($i = 0; $i < $length; $i++) {
     $output .= chr(ord($input[$i]) - 0x20);
   }
   return $output;
 }

 static function decode ($input) {
   $output = '';
   $length = strlen($input);
   for ($i = 0; $i < $length; $i++) {
     $val = ord($input[$i]);
     $output .= ($val <= 0x3F) ? chr($val + 0x20) : ' ';
   }
   return $output;
 }
}

class BitStream {
  protected $_bitData = array();
  protected $_lastInt = 0;
  protected $_bitPos = 31;
  protected $_bitInit = 31;

  protected $_readPosition = array(0,31);

  public function __construct () {
    $this->_bitInit = (PHP_INT_SIZE << 3) - 1;
    $this->_bitPos = $this->_bitInit;
    $this->_readPosition = array(0, $this->_bitInit);
  }

  public function addBits ($value, $bits) {
    if ($this->_bitPos - $bits > 0) {
      $this->_bitPos -= $bits;
      $this->_lastInt |= ($value << ($this->_bitPos));
    } else {
      $this->_lastInt |= ($value >> ($bits - $this->_bitPos));
      $mask = $this->_bitData[] = $this->_lastInt;
      $mask = $mask << ($bits - $this->_bitPos);
      $value ^= $mask;
      $bits -= $this->_bitPos;
      $this->_bitPos = $this->_bitInit;
      $this->_lastInt = 0;
      $this->_bitPos -= $bits;
      $this->_lastInt |= ($value << ($this->_bitPos));
    }
  }

  public function __toString() {
    $size = $this->_bitInit;
    $output = '';
    foreach ($this->_bitData as $value) {
      $output .= sprintf("%0{$size}s ", base_convert($value, 10, 2));
    }
    if ($this->_bitPos != $this->_bitInit) {
      $size = $this->_bitInit - $this->_bitPos;
      $output .= sprintf("%0{$size}s", base_convert(($this->_lastInt >> $this->_bitPos), 10, 2));
    }
    return $output;
  }

  public function reset() { $this->_readPosition = array(0, $this->_bitInit); }

  protected function _readLastInt ($start, $length, &$output) {
    $activeByte = $this->_lastInt;
    // Start past end
    if ($start < $this->_bitPos) {
      $output = 0;
      return 0;
    }
    // Too many bits?
    if ($start - $this->_bitPos < $length) {
      $length = $start - $this->_bitPos;
    }

    // Read
    $output = (($activeByte >> $start) << $length) ^ ($activeByte >> ($start - $length));
    return $length;
  }

  public function read($length, &$count=NULL) {
    // Int overflow?
    if ($length > $this->_bitInit) {
      $length = $this->_bitInit;
      trigger_error(sprintf('Maximum read length of %d exceeded.', $this->_bitInit), E_USER_NOTICE);
    }

    // Can pull from current byte?
    $oLength = $length;
    $intPos = $this->_readPosition[0];
    $bitPos = $this->_readPosition[1];
    $activeByte = ($intPos < count($this->_bitData)) ? $this->_bitData[$intPos] : FALSE;
    // Last byte
//    var_dump($intPos, count($this->_bitData));
//    var_dump($activeByte);

    if (!$activeByte) {
      $value = 0;
      $read = $this->_readLastInt($bitPos, $length, $value);
      if (isset($count)) { $count = $read; }
      if ($read == 0) {
        return FALSE;
      }
      $this->_readPosition[1] -= $read;
      return $value;
    }

    if ($length <= $bitPos) {
      $value = (($activeByte >> $bitPos) << $length) ^ ($activeByte >> ($bitPos - $length));
      $this->_readPosition[1] -= $length;
      if (isset($count)) { $count = $length; }
    } else {
      $value = (($activeByte) ^ (($activeByte >> $bitPos) << $bitPos));
      $length -= $bitPos;
      $intPos = ++$this->_readPosition[0];
      $bitPos = $this->_readPosition[1] = $this->_bitInit;
      $activeByte = ($intPos < count($this->_bitData)) ? $this->_bitData[$intPos] : FALSE;
      if ($activeByte) {
        $value = $value << $length;
        $value |= (($activeByte >> $bitPos) << $length) ^ ($activeByte >> ($bitPos - $length));
        $bitsRead = $length;
        if (isset($count)) { $count = $oLength; }
      } else {
        $lvalue = 0;
        $read = $this->_readLastInt($bitPos, $length, $lvalue);
        $bitsRead = $read;
        $value = ($value << $read) | $lvalue;
        if (isset($count)) { $count = ($oLength - $length) + $bitsRead; }
      }
      $this->_readPosition[1] -= $bitsRead;
    }
    return $value;
  }
}

header('content-type: text/plain');


class ECC4B64 {
  const WORD_SIZE = 10;
  const ERROR_CHAR = 63;
  const MAX_CHAR = 63;
  static protected $_map = array (
    0 => 0,
    1 => 261,
    2 => 518,
    3 => 771,
    4 => 840,
    5 => 589,
    6 => 334,
    7 => 75,
    8 => 336,
    9 => 85,
    10 => 854,
    11 => 595,
    12 => 536,
    13 => 797,
    14 => 30,
    15 => 283,
    16 => 608,
    17 => 869,
    18 => 102,
    19 => 355,
    20 => 296,
    21 => 45,
    22 => 814,
    23 => 555,
    24 => 816,
    25 => 565,
    26 => 310,
    27 => 51,
    28 => 120,
    29 => 381,
    30 => 638,
    31 => 891,
    32 => 896,
    33 => 645,
    34 => 390,
    35 => 131,
    36 => 200,
    37 => 461,
    38 => 718,
    39 => 971,
    40 => 720,
    41 => 981,
    42 => 214,
    43 => 467,
    44 => 408,
    45 => 157,
    46 => 926,
    47 => 667,
    48 => 480,
    49 => 229,
    50 => 998,
    51 => 739,
    52 => 680,
    53 => 941,
    54 => 174,
    55 => 427,
    56 => 176,
    57 => 437,
    58 => 694,
    59 => 947,
    60 => 1016,
    61 => 765,
    62 => 510,
    63 => 251,
    );

    static $_parityFix = array(0,0,0,32,0,16,8,4,0,2,1,0,0,0,0,0);

    static public function encode ($string) {
      $strlen = strlen($string);
      $bitStream = new BitStream();
      for ($i = 0; $i < $strlen; $i++) {
        $val = ord($string[$i]) % (self::MAX_CHAR + 1);
        $val = self::$_map[$val];
        $bitStream->addBits($val, self::WORD_SIZE);
      }
      return $bitStream;
    }

    static public function decode ($bitStream) {
      $bitStream->reset();
      $count = 0;
      $output = '';
      do {
        $value = $bitStream->read(self::WORD_SIZE, $count);
        if ($value === FALSE) { continue; }
        if ($count != self::WORD_SIZE && $value !== FALSE) {
          $output .= chr(self::ERROR_CHAR);
        } else {
          $byteValue = self::extractByte($value);
          $newChar = is_integer($byteValue) ? chr($byteValue) : chr(self::ERROR_CHAR);
          $output .= $newChar;
        }
      } while ($value !== FALSE);
      return $output;
    }


    static protected function extractByte ($value){
      $data = (($value & 0x80) >> 2) | (($value & 0x38) >> 1) | (($value & 0x3));
      if ($data >= self::MAX_CHAR) { return FALSE; }

      if (self::$_map[$data] != $value) {
        return self::_errorCorrect($value, $data);
      }
      return $data;
    }


    static protected function _calcParity ($value) {
      $bit = array (
       ($value & 0x20) >> 5,
       ($value & 0x10) >> 4,
       ($value & 0x08) >> 3,
       ($value & 0x04) >> 2,
       ($value & 0x02) >> 1,
       ($value & 0x01),
      );

      $p = array ();
      $p[] = ($bit[0] + $bit[1] + $bit[3] + $bit[4]) % 2;
      $p[] = ($bit[0] + $bit[2] + $bit[3] + $bit[5]) % 2;
      $p[] = ($bit[1] + $bit[2] + $bit[3]) % 2;
      $p[] = ($bit[4] + $bit[5]) % 2;
      return array($p, $bit);
    }


    static protected function _errorCorrect ($value, $data) {
      $pIn = array(
        ($value >> 9) & 1,
        ($value >> 8) & 1,
        ($value >> 6) & 1,
        ($value >> 2) & 1,
      );
      list($pCalc, $dataBits) = self::_calcParity($data);
      $targetBit = 0;
      $errorCount = 0;
      for ($i = 3; $i >= 0; --$i) {
        $errorBit = (($pIn[$i] ^ $pCalc[$i]));
        $errorCount += $errorBit;
        $targetBit = ($targetBit << 1) | $errorBit;
      }
      var_dump ($pIn, $pCalc, $targetBit, $errorCount);
      if ($errorCount > 1) {
        var_dump ($data, self::$_parityFix[$targetBit]);
        $data ^= self::$_parityFix[$targetBit];
      }
      return $data;
    }
}

class Encoder {
  static public function encode ($input) {
    $base64 = SMS64::encode($input);
    $bitStream = ECC4B64::encode($base64);
    return $bitStream;
  }
  static public function decode ($bitStream) {
    $output = ECC4B64::decode($bitStream);
    $output = SMS64::decode($output);
    return $output;
  }
}

class RotatingBitStream {
  private $_bitData = NULL;

  public function __construct (BitStream $bitStream) {
    $this->_bitData = $bitStream;
  }

  public function read() {
    $value = $this->_bitData->read(1);
    if ($value === FALSE) {
      $this->reset();
      $value = $this->_bitData->read(1);
    }
    return $value;
  }

  public function reset() {
    $this->_bitData->reset();
  }
}


class Timer {
  static $_times = array();
  static $_starts = array();
  static $_ends = array();
  static function Start($key) {
    return;
    if (!array_key_exists($key, self::$_times)){
      self::$_times[$key] = array();
    }
    self::$_starts[$key] = microtime(TRUE);
  }
  static function End($key) {
    return;
    self::$_times[$key][] = microtime(TRUE) - self::$_starts[$key];
  }
  static function Display() {
    return;
    foreach (self::$_times as $key => $value) {
      $total = 0;
      foreach ($value as $item) {
        $total += $item;
      }
      $avg = $total / count($value);
      echo "$key excecuted $total avg $avg\n";
    }
  }
}

class EmbedImage {
  private $_message = NULL;
  private $_messageStream = NULL;
  private $_inputFileName = '';
  private $_outputFileName = '';
  private $_fh = NULL;
  private $_fo = NULL;
  private $_options = array();

  private $_bitsPerLine = 100;
  private $_intensity = 3;
  private $_useCb = TRUE;
  private $_useCr = FALSE;
  private $_useY = FALSE;
  private $_delta = 0;

  static private $_incrMap = array(array(), array());

  public function __construct($message, $imageDataFile, $imageOutputFile = NULL, $options = array()) {
    $this->_message = Encoder::encode($message);
    $this->_messageStream = new RotatingBitStream($this->_message);
    $this->_inputFileName = $imageDataFile;

    if (!$imageOutputFile) {
      $this->_outputFileName = $this->_inputFileName . '.encyuv';
    }

    $this->_options = $options;
    foreach ($options as $key => $value) {
      if (method_exists($this, 'set' . ucfirst($key))) {
        $method = 'set' . ucfirst($key);
        $this->$method($value);
      }
    }
    $this->buildIncrMap ();
  }
  protected function buildIncrMap () {
    for ($i = 0; $i <= 255; $i++) {
      $value = $i - $this->_intensity;
      if ($value < 0) $value = 0;
      self::$_incrMap[0][chr($i)] = chr($value);
      $value = $i + $this->_intensity;
      if ($value > 255) $value = 255;
      self::$_incrMap[1][chr($i)] = chr($value);
    }
  }

  public function test() {
    $this->openFileHandles();
    $this->xSize = 1024;
    $this->ySize = 1243;
    $p = $this->loadBlockLine(8);
    $p = $this->processBlockLine($p, 8);
  }
  public function hexData ($p) {
    if (is_array($p)) {
      return array_map(array($this, 'hexData'), $p);
    }
    $output = '';
    for ($i = 0; $i < strlen($p); $i++) {
      $output .= sprintf('%02x ', ord($p[$i]));
    }
    return $output;
  }
  protected function setBitsPerLine ($val) { $this->_bitsPerLine = $val; }
  protected function setIntensity ($val) { $this->_intensity = $val; }
  protected function setUseCb ($val) { $this->_useCb = $val; }
  protected function setUseCr ($val) { $this->_useCr = $val; }
  protected function setOutputFile ($val) { $this->_outputFileName = $val; }

  public function __destroy() {
    if (is_resource($this->_fo)) fclose($this->_fo);
    if (is_resource($this->_fh)) fclose($this->_fh);
  }


  protected function openFileHandles() {
    $this->_fh = fopen($this->_inputFileName, 'rb');
    $this->_fo = fopen($this->_outputFileName, 'wb');
  }

  protected function getBlockDelta () {
    $delta = $this->_delta;
    if ($delta) return $delta;
    $delta = min($this->xSize, $this->ySize) / $this->_bitsPerLine;
    return $delta;
  }
  public function encode() {
    $this->openFileHandles();
    $this->xSize = 1024;
    $this->ySize = 1243;
    $delta = $this->getBlockDelta();
    $startY = 0;
    $endY = 0;
    $y = 0;
    do {
      Timer::Start('Encode() Loop');
      $endY = round($y + $delta);
      $lineHeight = $endY - $startY + 1;
      $pixelData = $this->loadBlockLine($lineHeight);
      $pixelData = $this->processBlockLine($pixelData, $delta);
      $this->saveBlockLine($pixelData, $this->xSize);
      $startY = $endY + 1;
      $y += $delta;
      Timer::End('Encode() Loop');
    } while (!empty($pixelData));
    fclose($this->_fh);
    fclose($this->_fo);
  }

  protected function saveBlockLine($pixelData, $maxX) {
    Timer::Start(__METHOD__);
    $output = array();
    $x = count($pixelData);
    $maxY = ($x == 0) ? 0 : count($pixelData[$x-1]);
    for ($y = 0; $y < $maxY; ++$y) {
      for ($x = 0; $x < $maxX; ++$x) {
        $output[] = $pixelData[$x][$y];
      }
    }

    $output = implode('', $output);
    fwrite($this->_fo, $output);
    Timer::End(__METHOD__);
  }

  protected function processBoxel(&$pixelData, $bit) {
    Timer::Start(__METHOD__);
    $cbValue = $this->_intensity * ($bit ? 1 : -1) * (int) $this->_useCb;
    $crValue = $this->_intensity * ($bit ? 1 : -1) * (int) $this->_useCr;

    foreach ($pixelData as $x => $xRow) {
      foreach ($xRow as $y => $pixel) {
        if ($this->_useCb) $pixelData[$x][$y][1] = self::$_incrMap[$bit][$pixel[1]];
        if ($this->_useCr) $pixelData[$x][$y][2] = self::$_incrMap[$bit][$pixel[2]];
      }
    }
    Timer::End(__METHOD__);
  }

  protected function processBlockLine($pixelData, $delta) {
    Timer::Start(__METHOD__);
     $x = 0;
     $startX = 0;
     $endX = round($x+$delta);
     $processed = array();
     do {
       $chunkSize = $endX - $startX + 1;
       $chunk = array_slice($pixelData, $startX, $chunkSize);
       $bit = $this->_messageStream->read();
       $this->processBoxel($chunk, $bit);
       $processed = array_merge($processed, $chunk);

       // Get Block Boundaries in Pixels
       $x += $delta;
       $startX = $endX + 1;
       $endX = round($x+$delta);
     } while ($x < $this->xSize);
    Timer::End(__METHOD__);
    return $processed;

  }

  protected $_readBuffer = '';
  protected $_readPos = 0;
  protected $_readBufferSize = 0;
  protected function getInputImageData($size) {
    $output = '';

    if (($size) > ($this->_readBufferSize - $this->_readPos)) {
      $output .= substr($this->_readBuffer, $this->_readPos);
      $size -= strlen($output);
      $this->_readBuffer = fread($this->_fh, 65535);
      $this->_readBufferSize = strlen($this->_readBuffer);
      $this->_readPos = 0;
      if ($this->_readBuffer) {
        $output .= $this->getInputImageData($size);
        return $output;
      }
    }
    $part = substr($this->_readBuffer, $this->_readPos, $size);
    $this->_readPos += strlen($part);
    return $output . $part;
  }

  protected function loadBlockLine($blockSize) {
    Timer::Start(__METHOD__);
    $dataSize = $this->xSize * $blockSize * 3;
    $rawData = $this->getInputImageData($dataSize);
    if (!$rawData) {
      Timer::End(__METHOD__);
      return array();
    }
    $size = strlen($rawData);
    $curX = 0;
    $curY = 0;
    $data = array();
    for ($i = 0; $i < $size; $i+=3) {
        $data[$curX][$curY] = $rawData[$i] . $rawData[$i+1] . $rawData[$i+2];
      if (++$curX >= $this->xSize) {
        $curX = 0;
        ++$curY;
      }
    }
    Timer::End(__METHOD__);
    return $data;
  }

  public function display() {
    Timer::Display();
//    header('Content-type: image/jpeg');
    $cmd = "C:\\PROGRA~1\\IMAGEM~1.9-Q\\convert.exe -quality 95 -size {$this->xSize}x{$this->ySize} -depth 8 YCbCr:{$this->_outputFileName} jpg:-";
    passthru($cmd, $ret);

  }
}

//$cmd = '"C:\PROGRA~1\IMAGEM~1.9-Q\convert.exe" -quality 95 -size 1024x1243 -depth 8 YCbCr:C:\www\testImage1.yuv.encyuv jpg:C:\www\testScript.jpg';
//system($cmd, $ret);
//var_dump($cmd, $ret);
//die;
//phpinfo();die;
set_time_limit(0);
$inFile = 'C:\www\testImage1.yuv';
//$encFile = new EmbedImage('20080213001412 ICE-WOLF^', $inFile);
//$encFile->test();s
//$encFile->encode();
//$encFile->display();
//die;
$optionSet = array();
foreach (array(60, 90, 100, 110, 120, 150) as $bpl) {
  foreach (array(1, 2, 3, 5, 7, 9) as $int) {
    foreach (array(array(0,1), array(1,0), array(1,1)) as $val) {
      list($useCr,$useCb) = $val;
      $options = array(
        'bitsPerLine' => $bpl,
        'intensity' => $int,
        'useCb' => $useCb,
        'useCr' => $useCr,
        'outputFile' => $inFile . sprintf('-%s%s-bp%03d-i%02d.encyuv', $useCb?'Cb':'', $useCr?'Cr':'', $bpl, $int),
      );
      $ei = new EmbedImage('20080213001412 ICE-WOLF^', $inFile, NULL, $options);
      $ei->encode();
    }
  }
}
