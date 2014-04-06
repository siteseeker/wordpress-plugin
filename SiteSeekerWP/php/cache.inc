<?php

require_once('logger.inc');

$GLOBALS["CACHE_LITE_EXIST"] = true;
if (! @include_once('Cache/Lite.php')) {
  Logger::getInstance()->warning('Failed to include Cache/Lite.php');
  $GLOBALS["CACHE_LITE_EXIST"] = false;
}

interface Cache {
  public function get($key);
  public function put($key, $value);
}

class CacheFactory {
  public static function getCache($group, $lifeTime = 900) {
    if ($GLOBALS["CACHE_LITE_EXIST"]) {
      $cacheDir = '';
      if (isset($_ENV['SITESEEKER_COMMON'])) {
        $cacheDir .= $_ENV['SITESEEKER_COMMON'];
      }
      $cacheDir .= '/tmp/Cache_Lite/' . (empty($group)? '' : $group. '/') . '/';
      if (!is_writable($cacheDir)) {
        if (!file_exists($cacheDir)) {
          mkdir($cacheDir, 0700, true);
        }
        chmod($cacheDir, 0700);
      }
      try {
        return new CacheLiteAdapter($group, $cacheDir, $lifeTime);
      } catch (Exception $e) {
        Logger::getInstance()->warning('Failed to create new CacheLiteAdapter');
      }
    }  
    return SimpleCache::getInstance();
  }
}

class CacheLiteAdapter implements Cache {

  private $cacheLite;
  private $group;
  public function __construct($group, $cacheDir, $lifeTime) {
    $this->group = $group;
    $options = array('cacheDir' => $cacheDir,
                     'lifeTime' => $lifeTime,
		     'caching' => true,
		     'automaticSerialization' => 'enable',
		     'pearErrorMode' => 2,
                     'memoryCaching' => false);
      $this->cacheLite = new Cache_Lite($options);
  }

  public function get($key) {
    $data = $this->cacheLite->get($key, $this->group);
    return $data;
  }

  public function put($key, $value) {
    $ret = $this->cacheLite->save($value, $key, $this->group);
    if(!$ret) {
      Logger::getInstance()->warning("Failed to save cache: " . $ret);
    }
  }
}


class SimpleCache implements Cache {
  
  private static $instance = null;
  private $cache;

  public static function getInstance() {
    if (self::$instance == null) {
      self::$instance = new SimpleCache();
    }
    return self::$instance;
  }

  private function __construct() {
    $this->cache = array();
  }

  public function get($key) {
    if (isset($this->cache[$key]))
      return $this->cache[$key];
    return false;
  }

  public function put($key, $value) {
    $this->cache[$key] = $value;
  }
}

