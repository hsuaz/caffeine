<?php
declare(encoding='UTF-8');
namespace Ferrox\System\Net;

class HttpStatus {
  const HTTP_OK              = 200;
  const HTTP_CREATED         = 201;
  const HTTP_ACCEPTED        = 202;
  const HTTP_NO_CONTENT      = 204;
  const HTTP_RESET_CONTENT   = 205;
  const HTTP_PARTIAL_CONTENT = 206;

  const HTTP_MULTIPLE_CHOICES   = 300;
  const HTTP_MOVED_PERMANENTLY  = 301;
  const HTTP_FOUND              = 302;
  const HTTP_SEE_OTHER          = 303;
  const HTTP_NOT_MODIFIED       = 304;
  const HTTP_TEMPORARY_REDIRECT = 307;

  const HTTP_BAD_REQUEST              = 400;
  const HTTP_FORBIDDEN                = 403;
  const HTTP_UNAUTHORIZED             = 401;
  const HTTP_PAYMENT_REQUIRED         = 402;
  const HTTP_NOT_FOUND                = 404;
  const HTTP_METHOD_NOT_ALLOWED       = 405;
  const HTTP_NOT_ACCEPTABLE           = 406;
  const HTTP_REQUEST_TIMEOUT          = 408;
  const HTTP_CONFLICT                 = 409;
  const HTTP_GONE                     = 410;
  const HTTP_LENGTH_REQUIRED          = 411;
  const HTTP_PRECONDITION_FAILED      = 412;
  const HTTP_REQUEST_ENTITY_TOO_LARGE = 413;
  const HTTP_REQUEST_URI_TOO_LONG     = 414;
  const HTTP_UNSUPPORTED_MEDIA_TYPE   = 415;
  const HTTP_EXPECTATION_FAILED       = 417;
  const HTTP_IM_A_TEAPOT              = 418;

  const HTTP_SERVER_ERROR        = 500;
  const HTTP_NOT_IMPLEMENTED     = 501;
  const HTTP_BAD_GATEWAY         = 502;
  const HTTP_SERVICE_UNAVAILABLE = 503;
  const HTTP_GATEWAY_TIMEOUT     = 504;

  const HTTP_SITE_OFFLINE         = 599;

  static protected $status = array(
    self::HTTP_OK => 'OK',
    self::HTTP_CREATED => 'Created',
    self::HTTP_ACCEPTED => 'Accepted',
    self::HTTP_NO_CONTENT => 'No Content',
    self::HTTP_RESET_CONTENT => 'Reset Content',
    self::HTTP_PARTIAL_CONTENT => 'Partial Content',

    self::HTTP_MULTIPLE_CHOICES => 'Multiple Choices',
    self::HTTP_MOVED_PERMANENTLY => 'Moved Permanently',
    self::HTTP_FOUND => 'Found',
    self::HTTP_SEE_OTHER => 'See Other',
    self::HTTP_NOT_MODIFIED => 'Not Modified',
    self::HTTP_TEMPORARY_REDIRECT => 'Temporary Redirect',

    self::HTTP_BAD_REQUEST => 'Bad Request',
    self::HTTP_UNAUTHORIZED => 'Unauthorized',
    self::HTTP_PAYMENT_REQUIRED => 'Payment Required',
    self::HTTP_FORBIDDEN => 'Forbidden',
    self::HTTP_NOT_FOUND => 'Not Found',
    self::HTTP_METHOD_NOT_ALLOWED => 'Method Not Allowed',
    self::HTTP_NOT_ACCEPTABLE => 'Not Acceptable',
    self::HTTP_REQUEST_TIMEOUT => 'Request Timeout',
    self::HTTP_CONFLICT => 'Conflict',
    self::HTTP_GONE => 'Gone',
    self::HTTP_LENGTH_REQUIRED => 'Length Required',
    self::HTTP_PRECONDITION_FAILED => 'Precondition Failed',
    self::HTTP_REQUEST_ENTITY_TOO_LARGE => 'Request Entity Too Large',
    self::HTTP_REQUEST_URI_TOO_LONG => 'Request URI Too Long',
    self::HTTP_UNSUPPORTED_MEDIA_TYPE => 'Unsupported Media Type',
    self::HTTP_EXPECTATION_FAILED => 'Expectation Failed',
    self::HTTP_IM_A_TEAPOT => 'I\'m A Teapot',
    self::HTTP_SERVER_ERROR => 'Unknown Server Error',
    self::HTTP_NOT_IMPLEMENTED => 'Not Implemented',
    self::HTTP_BAD_GATEWAY => 'Bad Gateway',
    self::HTTP_SERVICE_UNAVAILABLE => 'Service Unavailable',
    self::HTTP_GATEWAY_TIMEOUT => 'Gateway Timeout',

    self::HTTP_SITE_OFFLINE => 'Site Temporarily Offline',
  );

  static public function getDescription($statusCode) {
    if (\array_key_exists($statusCode, static::$status)) {
      return static::$status[$statusCode];
    }
    return '';
  }

  /**
   * Sends a given error code
   * @param $statusCode int HTTP Status Code
   * @return
   */
  static public function send ($statusCode) {
    if (\array_key_exists($statusCode, static::$status)) {
      \header ('HTTP/1.1 ' . $statusCode . ' ' . static::$status[$statusCode]);
    } else {
      throw new Exception ('Unknown Status Code');
    }
  }


  /**
   * Check if the code is a valid HTTP Status Code
   * @param $code int
   *  Code to check
   * @return boolean
   */
  static public function isValidCode ($code) {
    if (!\is_numeric($code)) {
      return FALSE;
    }

    if (\array_key_exists(static::$status, $code)) {
      return TRUE;
    }

    return FALSE;
  }
}
