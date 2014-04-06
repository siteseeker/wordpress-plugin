<?php

/* Copyright (c) 2001-2008 Euroling AB */
/* Property of Euroling AB - Distribution prohibited */

require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'ssws.inc');
require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'cache.inc');

//ini_set('display_errors', true); error_reporting(-1);

class Connector {

  private $siteSeeker;
  private $searchRequest;
  private $statisticsRequest;
  private $searchResponse;
  private $cachedPageResponse;
  private $searchContext;
  private $version = 7;
  private $interfaceLanguage;

  function __construct($wsurl, $username, $password, $encoding, $interfaceLanguage) {
    ini_set('default_socket_timeout', 600);

    switch ($encoding) {
    case 'UTF-8':
      break;
    case 'ISO-8859-1':
      break;
    default:
      $encoding = 'ISO-8859-1';
    }

    $wsdl = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'SiteSeeker.wsdl';
    $this->interfaceLanguage = $interfaceLanguage;

    $this->siteSeeker = SiteSeeker::getInstance($wsdl, 
	  				  $wsurl, 
	  				  $username,
	  				  $password,
	  				  $encoding);
    $this->searchResponse = false;
    $this->cache = CacheFactory::getCache("search");
  }

 
  //****************//
  // Public methods //
  //****************//

  public function getVersion() {
    return $this->version;
  }

  public function getResponse() {
    if ($this->searchResponse) 
      return $this->searchResponse;
    if ($this->cachedPageResponse)
      return $this->cachedPageResponse;
  }

  public function getNHits() {
    return $this->searchResponse->getNHits();
  }

  public function getHits() {
    if ($this->searchResponse->getHits())
      return $this->searchResponse->getHits()->getHit();
    return array();
  }

  public function getHitsByCategory($order) {
    if ($order != 3 &&
	$order != 5 &&
	$order != 6) {
      return array('-1' => $this->getHits());
    }

    if (!$this->searchResponse->getHits()) return array();

    $res = array();
    foreach ($this->getHits() as $hit) {

      $catIds = $hit->getCategoryIds();

      if ($order == 3) {
	$res[$catIds[0]][] = $hit;
      } else if ($order == 5) {
	$res[$catIds[0]][] = $hit;
      } else if ($order == 6) {
	foreach ($catIds as $categoryId) 
	  $res[$categoryId][] = $hit;
      }
    }
    return $res;
  }

  public function getBestBets() {
    if (!is_object($this->searchResponse->getBets())) 
      return array();
    return $this->searchResponse->getBets()->getBet();
  }
  
  public function getLanguages() {
    if (!is_object($this->searchResponse->getLanguages())) 
      return array();
    return $this->searchResponse->getLanguages()->getHitPartition();
  }

  public function getDocTypes() {
    if (!is_object($this->searchResponse->getDocTypes())) 
      return array();
    return $this->searchResponse->getDocTypes()->getHitPartition();
  }

  public function getSearchContext($forceInitialization = false) {
    if ($this->searchContext) return $this->searchContext;
    if (!$forceInitialization) throw new SearcherException('Search context not initialized. Either call search(...) first, or set $forceInitialization to true to force initialization (possibly returning stale data)');
    $cacheKey = "SearchContext".$this->interfaceLanguage.$this->siteSeeker->location;
    $this->searchContext = $this->getSearchContextFromCache($cacheKey);
    return $this->searchContext;
  }

  public function getCategories() {
    if (!is_object($this->searchResponse->getCats()) ||
	!is_array($this->searchResponse->getCats()->getHitCategory())) 
      return array();
    $ret = array();
    foreach ($this->searchResponse->getCats()->getHitCategory() as $category) {
      $id = $category->getId();
      $ret["$id"] = $category;
    }
    return $ret;
  }

  public function getSpellingSuggestions() {
    if (!is_object($this->searchResponse->getSpells())) 
      return array();
    return $this->searchResponse->getSpells()->getSpell();
  }

  public function getQueryRefinements() {
    if (!is_object($this->searchResponse->getQueryRefinements())) 
      return array();
    return $this->searchResponse->getQueryRefinements();
  }

  public function getStatistics() {
    if ($this->searchResponse)
      return $this->searchResponse->response->getJsonResponse();
    else
      return "[]";
  }

  public function search($request, $isXml, $getContext = true) {
    $debug = false;
    if ($request instanceof doSearchRequest6) {
      $wsRequest = $request;
    } else {
      $wsRequest = $this->createRequest($request, $isXml);
      $debug = $request['debug'];
    }
    if (!$wsRequest) return 500;
    $wsRequest->setIlang($this->interfaceLanguage);

    $cacheKey = "SearchContext".$this->interfaceLanguage.$this->siteSeeker->location;
    $retryCount = 2;
    $wsResponse = false;
    try {
      for ($i = 0; !$wsResponse && $i <= $retryCount; $i++)
        {
          if ($getContext) $this->searchContext = $this->getSearchContextFromCache($cacheKey);
          $wsResponse = $this->siteSeeker->doSearch($wsRequest);
          if ($getContext && 
              $wsResponse->SearchResponse->getIndexLastModified() != null && 
              strcmp($this->searchContext->getTime(), $wsResponse->SearchResponse->getIndexLastModified()->t) != 0) {
            $wsResponse = false;
            if ($getContext) $this->cache->put($cacheKey, false);
            continue; 
          }
          $this->searchRequest = $wsRequest;
        }
    }
    catch (Exception $e) {
      if ($debug) { /* Ska detta vara med till release? */
	print_r($e);
	echo $this->siteSeeker->__getLastRequest()."\n\n";
	echo $this->siteSeeker->__getLastResponse();
      }
      if ($e->getMessage() == 'HTTP Error: 401 Unauthorized')
	return 401;
      return 500;
    }
    if (!$wsResponse) return 500;

    $this->searchResponse = $wsResponse->SearchResponse;

    if ($debug) {
      header('Content-Type: text/xml');
      echo $this->siteSeeker->__getLastResponse();
      exit;
    }

    return 200;
  } // function search

  public function notify($urls) {
    if (!is_array($urls)) {
      return false;
    }
    $wsRequest = new NotifyRequest($urls, 1);
    $wsResponse = $this->siteSeeker->doNotify($wsRequest);
    return $wsResponse->response->getOk();
  }

  public function topQueries($request, $nQueries) {
    $params['type'] = "topQueries";
    $params['ilang'] = $this->interfaceLanguage;
    $params['nqueries'] = $nQueries;

    $wsRequest = $this->createStatisticsRequest($params);
    
    if (!$wsRequest) return 500;
  
    $retryCount = 2;
    $wsResponse = false;
    try {
      for ($i = 0; !$wsResponse && $i <= $retryCount; $i++)
        {
          $wsResponse = $this->siteSeeker->doStatistics($wsRequest);
          $this->statisticsRequest = $wsRequest;
        }
    }
    catch (Exception $e) {
      if ($e->getMessage() == 'HTTP Error: 401 Unauthorized') {
	return 401;
      }
      return 500;
    }
    if (!$wsResponse) {
      return 500;
    }
    
    $this->searchResponse = $wsResponse;
    return 200;
  } // function topQueries

  public function logClick($userIdentifier, $mode, $hitNr, $url, $responseId) {
    if ($mode == 'hit') $mode = 0;
    else if ($mode == 'bestbet') $mode = 1;
    else if ($mode == 'pageloading') $mode = 2;
    else if ($mode !== 0 && $mode !== 1 && $mode !== 2) return false;
    $wsRequest = new doClickRequest($userIdentifier, $mode, $hitNr, $url, $responseId);
    if (!$wsRequest) return false;
    try {
      $wsResponse = $this->siteSeeker->doClick($wsRequest);
    } catch (Exception $e) {
      return false;
    }
    if (!$wsResponse) return false;
    return true;
  }

  public function logConversion($userIdentifier, $url, $responseId) {
    $mode = 3;
    $hitNr = 0;
    $wsRequest = new doClickRequest($userIdentifier, $mode, $hitNr, $url, $responseId);
    if (!$wsRequest) return false;
    try {
      $wsResponse = $this->siteSeeker->doClick($wsRequest);
    } catch (Exception $e) {
      return false;
    }
    if (!$wsResponse) return false;
    return true;
  }

  public function getCachedPage($url, $request, $target, $baseUrl, $hitNr, $directLink = false, $resid) {
    $cachedPageRequest = new getCachedPageRequest5($url, // $url
						   $request['query'], // $query, 
						   $request['ilang'], // $ilang
						   $resid, // $responseid, 
						   $request['uaid'], // $userIdentifier, 
						   '', // $userTokens
						   $target,
						   $baseUrl,
						   $hitNr,
						   $directLink);
    try {
      $wsResponse = $this->siteSeeker->getCachedPage($cachedPageRequest);
    } catch (Exception $e) {
      if ($e->getMessage() == 'HTTP Error: 401 Unauthorized')
	return 401;
      if ($request['debug']) {
	print_r($e);
	echo $this->siteSeeker->__getLastResponse();
      }
      return 500;
    }
    if (!$wsResponse) return 500;

    $this->cachedPageResponse = $wsResponse->CachedPageResponse;
    
    return 200;
  }

  //*****************//
  // Private methods //
  //*****************//

  private function getSearchContextFromCache($cacheKey) {
    $searchContext = $this->cache->get($cacheKey);	
    if (empty($searchContext)) {
      $paramRequest = new getParamRequest6();
      $paramRequest->setGetCategories(true);
      $wsResponse = $this->siteSeeker->getParam($paramRequest);
      if ($wsResponse)
        $searchContext = $wsResponse->ParamResponse;
      $this->cache->put($cacheKey, $searchContext);
    }
    return $searchContext;

  }

  private function createRequest($request, $isXml = false) {
    if(is_array($request)) {
      $language = $request['filter']['language']['language'];
      if (empty($request['filter']['language']['group'])) $language = -2;
      $parameters = array();
      if ($isXml) 
        $parameters[] = new SearchParameter('outputXml', true);
      $parameters[] = new SearchParameter('metaAttributes', true);
      $parameters[] = new SearchParameter('anyFiltersInOverview', true);
      $parameters[] = new SearchParameter('matchingAttachmentsOnly', false);
      $parameters[] = new SearchParameter('isRecommendationQuery', false);
      // avkommentera för att få teaser-bilder:    
      $parameters[] = new SearchParameter('useTeasers', true);
      $parameters[] = new SearchParameter('includeConversions', true);

      $sortOrder = $request['view']['sort']['order'];
      if ($sortOrder == 3) $sortOrder = 1;
      else if ($sortOrder == 6) $sortOrder = 5;

      // Datum på formatet 2008-08-27T14:45:05Z
      $dateBegin = '';
      $dateEnd = '';
      if ($request['filter']['date']['group']) {
        $dateBegin = $request['filter']['date']['begin']['composit'];
        $dateEnd = $request['filter']['date']['end']['composit'];
      }

      $searchRequest = new doSearchRequest6(
              $request['query'] , // $query, 
					    $request['view']['hits']['batch'], // $batchNumber, 
					    $sortOrder,
					    $request['filter']['category']['categories'], // $category, 
					    $request['filter']['format']['doctype'], // $doctype, 
					    $request['filter']['language']['language'], // $language, 
                                            0, // $age OBS! Ålder uttrycks som vanligt datumintervall i mallgränssnittet
					    0, // $responseid, 
					    !empty($request['spelling'])?true:false, // $spell, 
					    $request['maxQueryRefinements'], // $maxQueryRefinements, 
					    $request['ilang'], // $ilang
					    $request['view']['hits']['perpage'], // $nHits,
					    $request['uaid'], // $userIdentifier, 
					    $request['ipad'], // $clientIpAddress,
					    $request['origin'], // $interfaceKey, 
					    '', // $userTokens
					    $parameters, // $parameters = array());
					    ($request['filter']['language']['stemming'] == 2),
                                            $dateBegin, // $request['filter']['date']['group']?$request['filter']['date']['begin']['composit']:'',
                                            $dateEnd // $request['filter']['date']['group']?$request['filter']['date']['end']['composit']:''
					    );
      return $searchRequest;
    }
    return null;
  }

  /**
   * createStatisticsRequest 
   * 
   * @param mixed $type 
   * @param mixed $ilang 
   * @param mixed $nQueries 
   * @access public
   * @return void
   */
  private function createStatisticsRequest($params) {
    $parameters = array();
    foreach ( $params as $key => $value ) {
      $parameters[] = new SearchParameter($key, $value);
    }
    return new StatisticsRequest($parameters, 3);
  }

} // class seeker
