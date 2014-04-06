<?php

class Logger {

  private static $instance = null;
  private $debug = false;
  private $info = false;

  public static function getInstance() {
    if (self::$instance == null) {
      self::$instance = new Logger();
    }
    return self::$instance;
  }

  public function error($msg) {
    trigger_error($msg, E_USER_ERROR);
  }

  public function info($msg) {
    if ($this->info)
      trigger_error($msg, E_USER_NOTICE);
  }

  public function warning($msg) {
    trigger_error($msg, E_USER_WARNING);
  }

  public function debug($msg, $caller = null) {
    if ($this->debug) {
      $c = '';
      if ($caller != null && is_object($caller)) {
	$c = " " . get_class($caller) . ": ";
      }
      trigger_error($msg, E_USER_NOTICE);
    }
  }
  private function getRequestMsg() {
    if (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on')
      $protocol = 'https://';
    else
      $protocol = 'http://';
    return $protocol . $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'];
  }
}

