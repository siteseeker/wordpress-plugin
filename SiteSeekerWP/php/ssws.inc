<?php 

/* Copyright (c) 2007-2008 Euroling AB */
/* Property of Euroling AB - Distribution prohibited */

define ('WS_VERSION', 7);

class SearcherException extends Exception {
  protected $rootException;
  public function __construct($message = '', Exception $rootCause = null, $code = 500) {
    parent::__construct($message, $code); 
    $this->rootException = $rootCause;
  }
}

class UnauthorizedException extends SearcherException {
  public function __construct($message = '', Exception $rootCause = null) {
    parent::__construct($message, $rootCause, 401); 
  }
}

class SiteSeeker extends SoapClient { 

  public static function getInstance($wsdl, $location, $username = '', $password = '', $encoding = 'ISO-8859-1') {
    $classmap = array('SearchResponse'=>'SearchResponse',
                      'Hit'=>'Hit',
                      'ArrayOfHit'=>'ArrayOfHit',
                      'Bet'=>'Bet',
                      'DoSearchRequest6'=>'DoSearchRequest6',
                      'DoSearchRequest6'=>'DoSearchRequest3',
                      'DoSearchRequest6'=>'DoSearchRequest',
		      'Bet'=>'Bet',
                      'Age' => 'Age',
		      'ArrayOfBet'=>'ArrayOfBet',
		      'ArrayOfSpell'=>'ArrayOfSpell',
		      'ArrayOfHitPartition'=>'ArrayOfHitPartition',
		      'HitCategory'=>'HitCategory',
		      'ArrayOfHitCategory'=>'ArrayOfHitCategory',
		      'HitPartition' => 'HitPartition',
		      'Attachments' => 'Attachments',
		      'Spell'=>'Spell',
		      'GetCachedPageRequest5' => 'GetCachedPageRequest5',
                      'ParamResponse' => 'ParamResponse',
                      'GetParamRequest6' => 'GetParamRequest6',
		      'CachedPageResponse' => 'CachedPageResponse',
		      'Thumbnail' => 'Thumbnail',
                      'ArrayOfMetaAttr' => 'ArrayOfMetaAttr',
                      'MetaAttr' => 'MetaAttr',
                      'ArrayOfQueryRefinements'=>'ArrayOfQueryRefinements',
                      'QueryRefinement'=>'QueryRefinement',
                      'QueryCompletionResponse'=>'QueryCompletionResponse',
                      'StatisticsResponse'=>'StatisticsResponse',
                      'NotifyResponse'=>'NotifyResponse',
		      );
    $options = array('trace' => true, 
		     'classmap' => $classmap, 
		     'features' => SOAP_SINGLE_ELEMENT_ARRAYS,
		     'location' => $location,
		     'encoding' => $encoding,
		     'uri' => 'urn:siteseeker',
		     'login' => $username, 
		     'password' => $password,
		     );

    return new SiteSeeker($wsdl, $options);
  }

  private $wsdlUrl;

  public function __construct($wsdl, $options) {
    $this->wsdlUrl = $wsdl;
    $this->location = $options['location'];

    parent::__construct($wsdl, $options);
  }

  public function doQueryCompletion(QueryCompletionRequest $request)
  { 
    $soapRequestType = new SoapVar($request, SOAP_ENC_OBJECT, 'QueryCompletionRequest', 'urn:siteseeker');
    $args = array();
    $args['parameters']['request'] = $soapRequestType;
    return $this->Call('doQueryCompletion', $args);
  }

    public function doStatistics(StatisticsRequest $request)
    { 
      $soapRequestType = new SoapVar($request, SOAP_ENC_OBJECT, 'StatisticsRequest', 'urn:siteseeker');
      $args = array();
      $args['parameters']['request'] = $soapRequestType;
      $this->_encoding = "UTF-8";
      return $this->Call('doStatistics', $args);
    }

    public function doNotify(NotifyRequest $request)
    { 
      $soapRequestType = new SoapVar($request, SOAP_ENC_OBJECT, 'NotifyRequest', 'urn:siteseeker');
      $args = array();
      $args['parameters']['request'] = $soapRequestType;
      $this->_encoding = "UTF-8";
      return $this->Call('doNotify', $args);
    }

    public function doSearch(doSearchRequest6 $request)
    { 
      $soapRequest6Type = new SoapVar($request, SOAP_ENC_OBJECT, 'doSearchRequest6', 'urn:siteseeker');
  
      $args = array();
      $args['parameters']['request'] = $soapRequest6Type;
      return $this->Call('doSearch', $args);
    }

    public function doShortSearch(doSearchRequest6 $request)
    { 
      $soapRequest6Type = new SoapVar($request, SOAP_ENC_OBJECT, 'doSearchRequest6', 'urn:siteseeker');
      $args = array();
      $args['parameters']['request'] = $soapRequest6Type;
      return $this->Call('doShortSearch', $args);
    }

    public function getParam( $request  = null )
    { 
      $args = array();
      $args['parameters']['request'] = $request;
      return $this->Call('getParam', $args);
    }

    public function getStatsAccess( $request  = null )
    { 
      $args = array();
      $args['parameters']['request'] = $request;
      return $this->Call('getStatsAccess', $args);
    }

    public function getCachedPage(getCachedPageRequest5 $request)
    {
      $args = array();
      $args['parameters']['request'] = new SoapVar($request, SOAP_ENC_OBJECT, 'getCachedPageRequest5', 'urn:siteseeker');
      return $this->Call('getCachedPage', $args);
    }

    public function doClick( $request  = null )
    { 
      $args = array();
      $args['parameters']['request'] = $request;
      return $this->Call('doClick', $args);
    }

    public function Ping( )
    { 
      return $this->Call('Ping');
    }

    private function Call($methodName, $args) {
      return @$this->__call($methodName, $args,
        array('uri'=>'urn:siteseeker',
              'soapaction'=>''));
    }
  } 

  // -------- Begin SOAP Types ------- 

  class FilterArray
  { 
    private $v = null; 

    public function getV()
    {
      return $this->v;
    }
    public function setV($value)
    {
      $this->v = $value;
    }

  } 

  class FilterNum
  { 
    private $v = null; 

    public function __construct($value) 
    {
      $this->v = $value;
    }

    public function getV()
    {
      return $this->v;
    }
    public function setV($value)
    {
      $this->v = $value;
    }
  } 

  class FilterBool
  { 
    private $v = null; 

    public function __construct($value) 
    {
      $this->v = $value;
    }

    public function getV()
    {
      return $this->v;
    }
    public function setV($value)
    {
      $this->v = $value;
    }
  } 

  class DateRange
  { 
    private $s = null; 
    private $e = null; 

    public function __construct($begin, $end) 
    {
      $this->s = $begin;
      $this->e = $end;
    }

    public function getS()
    {
      return $this->s;
    }
    public function setS($value)
    {
      $this->s = $value;
    }

    public function getE()
    {
      return $this->e;
    }
    public function setE($value)
    {
      $this->e = $value;
    }
  } 

  class MetaAttr
  { 
    private $name = null; 
    private $value = null; 

    public function getName()
    {
      return $this->name;
    }
  public function setName($value)
  {
    $this->name = $value;
  }

  public function getValue()
  {
    return $this->value;
  }
  public function setValue($value)
  {
    $this->value = $value;
  }

} 


class ArrayOfMetaAttr
{ 
  private $MetaAttribute = null; 

  public function getMetaAttribute()
  {
    return $this->MetaAttribute;
  }
  public function setMetaAttribute($value)
  {
    $this->MetaAttribute = $value;
  }

} 


class SearchParameter
{ 
  private $key = null; 
  private $value = null; 

  public function __construct($key, $value) {
    $this->key = $key;
    if ($value === true) 
      $this->value = 'true';
    else
      $this->value = $value;
  }

  public function getKey()
  {
    return $this->key;
  }
  public function setKey($value)
  {
    $this->key = $value;
  }

  public function getValue()
  {
    return $this->value;
  }
  public function setValue($value)
  {
    $this->value = $value;
  }

} 

class QueryCompletionRequest 
{
  public function __construct($parameters, 
                              $version) 
  {
    $this->parameters = $parameters;
    $this->version = $version;
  }
  private $parameters = null; 
  private $version = null; 

  public function getParameters()
  {
    return $this->parameters;
  }
  public function setParameters($parameters)
  {
    $this->parameters = $parameters;
  }
  public function getVersion()
  {
    return $this->version;
  }
  public function setVersion($version)
  {
    $this->version = $version;
  }
}

class QueryCompletionResponse {
  private $message = null;
  private $jsonResponse = null;
  public function getMessage() {
    return $this->message();
  }
  public function setMessage($message) {
    $this->message = $message;
  }
  public function getJsonResponse() {
    return $this->jsonResponse;
  }
  public function setJsonResponse($jsonResponse) {
    $this->jsonResponse = $jsonResponse;
  }
}

class StatisticsRequest 
{
  public function __construct($parameters, 
                              $version) 
  {
    $this->parameters = $parameters;
    $this->version = $version;
  }
  private $parameters = null; 
  private $version = null; 

  public function getParameters()
  {
    return $this->parameters;
  }
  public function setParameters($parameters)
  {
    $this->parameters = $parameters;
  }
  public function getVersion()
  {
    return $this->version;
  }
  public function setVersion($version)
  {
    $this->version = $version;
  }
}

class StatisticsResponse {
  private $message = null;
  private $jsonResponse = null;
  public function getMessage() {
    return $this->message();
  }
  public function setMessage($message) {
    $this->message = $message;
  }
  public function getJsonResponse() {
    return $this->jsonResponse;
  }
  public function setJsonResponse($jsonResponse) {
    $this->jsonResponse = $jsonResponse;
  }
}

class NotifyRequest 
{
  public function __construct($urls, 
                              $version) 
  {
    $this->urls = $urls;
    $this->version = $version;
  }
  private $urls = null; 
  private $version = null; 

  public function getUrls()
  {
    return $this->urls;
  }
  public function setUrls($urls)
  {
    $this->urls = $urls;
  }
  public function getVersion()
  {
    return $this->version;
  }
  public function setVersion($version)
  {
    $this->version = $version;
  }
}

class NotifyResponse {
  private $ok = null;
  public function getOk() {
    return $this->ok;
  }
  public function setOk($ok) {
    $this->ok = $ok;
  }
}

class doSearchRequest6
{ 
  public function __construct(
                              $query, 
                              $batchNumber, 
                              $sortOrder,
			                        $category, 
                              $format, 
                              $language, 
                              $age, 
                              $responseId, 
                              $spell, 
                              $maxQueryRefinements, 
                              $ilang, 
                              $nHits,
			                        $userIdentifier, 
                              $clientIpAddress,
                              $interfaceKey, 
                              $userTokens = '', 
                              $parameters = array(), 
                              $useStemming = true,
                              $dateBegin = '', 
                              $dateEnd = '') {

    $this->ilang = $ilang;
    $this->batchNumber = $batchNumber;
    $this->query = $query;
    $this->sortOrder = $sortOrder;
    $this->category = $category;
    if (!empty($format))
      $this->format = new FilterNum($format);
    if ($language >= 0)
      $this->language = new FilterNum($language);
    if (!empty($age)) {
      // Age(id=0) == när som helst
      $this->age = new FilterNum($age);
    } else if (!empty($dateBegin) && !empty($dateEnd)) {
      // Datumintervall kan endast användas om ålder sätts till 0 (dvs när som helst)
      $date = new DateRange($dateBegin, $dateEnd);
      $this->date = $date;
    }
    $this->userIdentifier = $userIdentifier;
    $this->userTokens = $userTokens;
    $this->responseId = $responseId;
    $this->spellSuggestion = $spell;
    $this->nHits = $nHits;
    $this->parameters = $parameters;
    $this->parameters[] = new SearchParameter('clientIpAddress', $clientIpAddress);
    $this->parameters[] = new SearchParameter('interfaceKey', $interfaceKey); 
    $this->parameters[] = new SearchParameter('maxQueryRefinements', $maxQueryRefinements); 
    if ( isset($_SERVER['HTTP_REFERER']) ) {
      $this->parameters[] = new SearchParameter('clientReferer', $_SERVER['HTTP_REFERER']); 
    }
    $this->version = WS_VERSION;
    $this->stemming = new FilterBool($useStemming);
  }

  private $query = null; 
  private $ilang = null; 
  private $category = null; 
  private $language = null; 
  private $format = null; 
  private $stemming = null; 
  private $date = null; 
  private $sortOrder = null; 
  private $batchNumber = null; 
  private $hitsIds = null; 
  private $nHits = null; 
  private $version = null; 
  private $parameters = null; 
  private $spellSuggestion = null; 
  private $queryRefinement = null; 

  public function getParameters()
  {
    return $this->parameters;
  }
  public function setParameters($value)
  {
    $this->parameters = $value;
  }
  public function addParameter($value)
  {
    $this->parameters[] = $value;
  }

  public function getQuery()
  {
    return $this->query;
  }
  public function setQuery($value)
  {
    $this->query = $value;
  }

  public function getIlang()
  {
    return $this->ilang;
  }
  public function setIlang($value)
  {
    $this->ilang = $value;
  }
  public function getUserIdentifier()
  {
    return $this->userIdentifier;
  }
  public function setUserIdentifier($value)
  {
    $this->userIdentifier = $value;
  }
  public function getCategory()
  {
    return $this->category;
  }
  public function setCategory($value)
  {
    $this->category = $value;
  }

  public function getLanguage()
  {
    return $this->language;
  }
  public function setLanguage($value)
  {
    $this->language = $value;
  }

  public function getFormat()
  {
    return $this->format;
  }
  public function setFormat($value)
  {
    $this->format = $value;
  }

  public function getStemming()
  {
    return $this->stemming;
  }
  public function setStemming($value)
  {
    $this->stemming = $value;
  }

  public function getDate()
  {
    return $this->date;
  }
  public function setDate($value)
  {
    $this->date = $value;
  }

  public function getSortOrder()
  {
    return $this->sortOrder;
  }
  public function setSortOrder($value)
  {
    $this->sortOrder = $value;
  }

  public function getBatchNumber()
  {
    return $this->batchNumber;
  }
  public function setBatchNumber($value)
  {
    $this->batchNumber = $value;
  }

  public function getHitsIds()
  {
    return $this->hitsIds;
  }
  public function setHitsIds($value)
  {
    $this->hitsIds = $value;
  }

  public function getNHits()
  {
    return $this->nHits;
  }
  public function setNHits($value)
  {
    $this->nHits = $value;
  }

  public function getUserTokens()
  {
    return $this->userTokens;
  }
  public function setUserTokens($value)
  {
    $this->userTokens = $value;
  }

  public function getVersion()
  {
    return $this->version;
  }
  public function setVersion($value)
  {
    $this->version = $value;
  }

  public function getSpellSuggestion()
  {
    return $this->spellSuggestion;
  }
  public function setSpellSuggestion($value)
  {
    $this->spellSuggestion = $value;
  }

  public function getQueryRefinement()
  {
    return $this->queryRefinement;
  }
  public function setQueryRefinement($value)
  {
    $this->queryRefinement = $value;
  }
} 

class Hit
{ 
  private $title = null; 
  private $description = null; 
  private $metaDescription = null; 
  private $sourceLink = null; 
  private $sourceLinkHighlighted = null; 
  private $specialText = null; 
  private $contextLink = null; 
  private $dcIdentifier = null; 
  private $teaserImageUrl = null; 
  private $pageId = null; 
  private $inFrame = null; 
  private $doctypeId = null; 
  private $modDate = null; 
  private $size = null; 
  private $langId = null; 
  private $categoryIds = null;

  public function getTitle()
  {
    return $this->title;
  }
  public function setTitle($value)
  {
    $this->title = $value;
  }

  public function getDescription()
  {
    return $this->description;
  }
  public function setDescription($value)
  {
    $this->description = $value;
  }

  public function getMetaDescription()
  {
    return $this->metaDescription;
  }
  public function setMetaDescription($value)
  {
    $this->metaDescription = $value;
  }

  public function getSpecialText()
  {
    return $this->specialText;
  }
  public function setSpecialText($value)
  {
    $this->specialText = $value;
  }

  public function getSourceLink()
  {
    return $this->sourceLink;
  }
  public function setSourceLink($value)
  {
    $this->sourceLink = $value;
  }

  public function getSourceLinkHighlighted()
  {
    return $this->sourceLinkHighlighted;
  }
  public function setSourceLinkHighlighted($value)
  {
    $this->sourceLinkHighlighted = $value;
  }

  public function getContextLink()
  {
    return $this->contextLink;
  }
  public function setContextLink($value)
  {
    $this->contextLink = $value;
  }

  public function getDcIdentifier()
  {
    return $this->dcIdentifier;
  }
  public function setDcIdentifier($value)
  {
    $this->dcIdentifier = $value;
  }

  public function getTeaserImageUrl()
  {
    return $this->teaserImageUrl;
  }
  public function setTeaserImageUrl($value)
  {
    $this->teaserImageUrl = $value;
  }

  public function getPageId()
  {
    return $this->pageId;
  }
  public function setPageId($value)
  {
    $this->pageId = $value;
  }

  public function getInFrame()
  {
    return $this->inFrame;
  }
  public function setInFrame($value)
  {
    $this->inFrame = $value;
  }

  public function getDoctypeId()
  {
    return $this->doctypeId;
  }
  public function setDoctypeId($value)
  {
    $this->doctypeId = $value;
  }

  public function getCategoryIds()
  {
    return $this->categoryIds;
  }
  public function setCategoryIds($value)
  {
    $this->categoryIds = $value;
  }

  public function getModDate()
  {
    return $this->modDate;
  }
  public function setModDate($value)
  {
    $this->modDate = $value;
  }

  public function getSize()
  {
    return $this->size;
  }
  public function setSize($value)
  {
    $this->size = $value;
  }

  public function getLangId()
  {
    return $this->langId;
  }
  public function setLangId($value)
  {
    $this->langId = $value;
  }

  private $hitNr = null; 

  public function getHitNr()
  {
    return $this->hitNr + 1;
  }
  public function setHitNr($value)
  {
    $this->hitNr = $value;
  }

  private $rank = null; 

  public function getRank()
  {
    return $this->rank;
  }
  public function setRank($value)
  {
    $this->rank = $value;
  }

  private $metaAttributes = null; 

  public function getMetaAttributes()
  {
    return $this->metaAttributes;
  }
  public function setMetaAttributes($value)
  {
    $this->metaAttributes = $value;
  }
  public function getMetaAttribute($name)
  {
    if ($this->metaAttributes && 
        $attributes = $this->metaAttributes->getMetaAttribute()) {
      foreach ($attributes as $attribute) {
        if ($attribute->getName() == $name) { 
          return $attribute->getValue();
        }
      }
    }
    return false;
  }

  private $matchesQuery = null; 

  public function getMatchesQuery()
  {
    return $this->matchesQuery;
  }
  public function setMatchesQuery($value)
  {
    $this->matchesQuery = $value;
  }
  
  private $attachments = null; 
  
  public function getAttachments()
  {
    return $this->attachments;
  }
  public function setAttachments($value)
  {
    $this->attachments = $value;
  }
  
} 


class ShortHit
{ 
  private $title = null; 

  public function getTitle()
  {
    return $this->title;
  }
  public function setTitle($value)
  {
    $this->title = $value;
  }

  private $sourceLink = null; 

  public function getSourceLink()
  {
    return $this->sourceLink;
  }
  public function setSourceLink($value)
  {
    $this->sourceLink = $value;
  }

  private $dcIdentifier = null; 

  public function getDcIdentifier()
  {
    return $this->dcIdentifier;
  }
  public function setDcIdentifier($value)
  {
    $this->dcIdentifier = $value;
  }

  private $pageId = null; 

  public function getPageId()
  {
    return $this->pageId;
  }
  public function setPageId($value)
  {
    $this->pageId = $value;
  }

  private $inFrame = null; 

  public function getInFrame()
  {
    return $this->inFrame;
  }
  public function setInFrame($value)
  {
    $this->inFrame = $value;
  }

  private $doctypeId = null; 

  public function getDoctypeId()
  {
    return $this->doctypeId;
  }
  public function setDoctypeId($value)
  {
    $this->doctypeId = $value;
  }

  private $categoryIds = null; 

  public function getCategoryIds()
  {
    return $this->categoryIds;
  }
  public function setCategoryIds($value)
  {
    $this->categoryIds = $value;
  }

  private $modDate = null; 

  public function getModDate()
  {
    return $this->modDate;
  }
  public function setModDate($value)
  {
    $this->modDate = $value;
  }

  private $size = null; 

  public function getSize()
  {
    return $this->size;
  }
  public function setSize($value)
  {
    $this->size = $value;
  }

  private $langId = null; 

  public function getLangId()
  {
    return $this->langId;
  }
  public function setLangId($value)
  {
    $this->langId = $value;
  }

  private $hitNr = null; 

  public function getHitNr()
  {
    return $this->hitNr + 1;
  }
  public function setHitNr($value)
  {
    $this->hitNr = $value;
  }

  private $rank = null; 

  public function getRank()
  {
    return $this->rank;
  }
  public function setRank($value)
  {
    $this->rank = $value;
  }

} 


class Category
{ 
  private $id = null; 

  public function getId()
  {
    return $this->id;
  }
  public function setId($value)
  {
    $this->id = $value;
  }

  private $name = null; 

  public function getName()
  {
    return $this->name;
  }
  public function setName($value)
  {
    $this->name = $value;
  }

  private $nDocs = null; 

  public function getNDocs()
  {
    return $this->nDocs;
  }
  public function setNDocs($value)
  {
    $this->nDocs = $value;
  }

  private $groupId = null; 

  public function getGroupId()
  {
    return $this->groupId;
  }
  public function setGroupId($value)
  {
    $this->groupId = $value;
  }

  private $groupName = null; 

  public function getGroupName()
  {
    return $this->groupName;
  }
  public function setGroupName($value)
  {
    $this->groupName = $value;
  }

} 


class HitCategory
{ 
  // Denna funktion kan användas för att skapa en HitCategory utifrån kontexten,
  // dvs en kategori som ej skickas i översikten
  public function __construct($id, $name, $groupId, $groupName, $nDocs) {
    $this->id = $id;
    $this->name = $name;
    $this->nameHighlighted = $name; // Det finns inget markerat namn eftersom kontexten är statisk
    $this->groupId = $groupId;
    $this->groupName = $groupName;
    $this->nDocs = $nDocs;
  }
  
  private $id = null; 

  public function getId()
  {
    return $this->id;
  }
  public function setId($value)
  {
    $this->id = $value;
  }

  private $name = null; 

  public function getName()
  {
    return $this->name;
  }
  public function setName($value)
  {
    $this->name = $value;
  }

  private $nameHighlighted = null; 

  public function getNameHighlighted()
  {
    return $this->nameHighlighted;
  }
  public function setNameHighlighted($value)
  {
    $this->nameHighlighted = $value;
  }

  private $nDocs = null; 

  public function getNDocs()
  {
    return $this->nDocs;
  }
  public function setNDocs($value)
  {
    $this->nDocs = $value;
  }

  private $groupId = null; 

  public function getGroupId()
  {
    return $this->groupId;
  }
  public function setGroupId($value)
  {
    $this->groupId = $value;
  }

  private $groupName = null; 

  public function getGroupName()
  {
    return $this->groupName;
  }
  public function setGroupName($value)
  {
    $this->groupName = $value;
  }

  private $nHits = null; 

  public function getNHits()
  {
    return $this->nHits;
  }
  public function setNHits($value)
  {
    $this->nHits = $value;
  }

  private $nHitsInBatch = null; 

  public function getNHitsInBatch()
  {
    return $this->nHitsInBatch;
  }
  public function setNHitsInBatch($value)
  {
    $this->nHitsInBatch = $value;
  }

} 


class HitPartition
{ 
  private $id = null; 

  public function getId()
  {
    return $this->id;
  }
  public function setId($value)
  {
    $this->id = $value;
  }

  private $name = null; 

  public function getName()
  {
    return $this->name;
  }
  public function setName($value)
  {
    $this->name = $value;
  }

  private $nDocs = null; 

  public function getNDocs()
  {
    return $this->nDocs;
  }
  public function setNDocs($value)
  {
    $this->nDocs = $value;
  }

  private $nHits = null; 

  public function getNHits()
  {
    return $this->nHits;
  }
  public function setNHits($value)
  {
    $this->nHits = $value;
  }

} 


class Bet
{ 
  const bestBet = 'BESTBET';
  const relatedBet = 'RELATEDBET';

  private $title = null; 

  public function getTitle()
  {
    return $this->title;
  }
  public function setTitle($value)
  {
    $this->title = $value;
  }

  private $description = null; 

  public function getDescription()
  {
    return $this->description;
  }
  public function setDescription($value)
  {
    $this->description = $value;
  }

  private $url = null; 

  public function getUrl()
  {
    return $this->url;
  }
  public function setUrl($value)
  {
    $this->url = $value;
  }

  private $keywords = null; 

  public function getKeywords()
  {
    return $this->keywords;
  }
  public function setKeywords($value)
  {
    $this->keywords = $value;
  }

  private $isExternal = null; 

  public function getIsExternal()
  {
    return $this->isExternal;
  }
  public function setIsExternal($value)
  {
    $this->isExternal = $value;
  }

  private $langId = null; 

  public function getLangId()
  {
    return $this->langId;
  }
  public function setLangId($value)
  {
    $this->langId = $value;
  }

  private $betNr = null; 

  public function getBetNr()
  {
    return $this->betNr;
  }
  public function setBetNr($value)
  {
    $this->betNr = $value;
  }

  private $type = null;

  public function getType()
  {
    return $this->type;
  }
  public function setType($value)
  {
    $this->type = $value;
  }

} 


class Spell
{ 
  private $spell = null; 

  public function getSpell()
  {
    return $this->spell;
  }
  public function setSpell($value)
  {
    $this->spell = $value;
  }

  private $htmlspell = null; 

  public function getHtmlspell()
  {
    return $this->htmlspell;
  }
  public function setHtmlspell($value)
  {
    $this->htmlspell = $value;
  }

  private $autoCorrect = null; 

  public function getAutoCorrect()
  {
    return $this->autoCorrect;
  }
  public function setAutoCorrect($value)
  {
    $this->autoCorrect = $value;
  }

} 

class QueryRefinement
{
    private $query = null;
    private $newTerms = null;

    public function getQuery() {
      return $this->query;
    }

    public function setQuery($query) {
      $this->query = $query;
    }

    public function getNewTerms() {
      return $this->newTerms;
    }

    public function setNewTerms($newTerms) {
      $this->newTerms = $newTerms;
    }
}


class ArrayOfHit
{ 
  private $Hit = null; 

  public function getHit()
  {
    return $this->Hit;
  }
  public function setHit($value)
  {
    $this->Hit = $value;
  }
} 


class ArrayOfShortHit
{ 
  private $Hit = null; 

  public function getHit()
  {
    return $this->Hit;
  }
  public function setHit($value)
  {
    $this->Hit = $value;
  }
} 


class ArrayOfHitCategory
{ 
  private $HitCategory = null; 

  public function getHitCategory()
  {
    return $this->HitCategory;
  }
  public function setHitCategory($value)
  {
    $this->HitCategory = $value;
  }

} 


class ArrayOfHitPartition
{ 
  private $HitPartition = null; 

  public function getHitPartition()
  {
    return $this->HitPartition;
  }
  public function setHitPartition($value)
  {
    $this->HitPartition = $value;
  }

} 


class ArrayOfBet
{ 
  private $Bet = null; 

  public function getBet()
  {
    if (is_array($this->Bet))
      return $this->Bet;
    else
      return array($this->Bet);
  }
  public function setBet($value)
  {
    $this->Bet = $value;
  }

} 


class ArrayOfSpell
{ 
  private $Spell = null; 

  public function getSpell()
  {
    return $this->Spell;
  }
  public function setSpell($value)
  {
    $this->Spell = $value;
  }

} 


class ArrayOfQueryRefinements
{ 
  private $queryRefinements = null; 

  public function getQueryRefinements()
  {
    return $this->queryRefinements;
  }
  public function setQueryRefinements($value)
  {
    $this->queryRefinements = $value;
  }
} 


class Time
{ 
  private $t = null; 

  public function getT()
  {
    return $this->t;
  }
  public function setT($value)
  {
    $this->t = $value;
  }

} 


class SearchResponse
{ 
  private $query = null; 

  public function getQuery()
  {
    return $this->query;
  }
  public function setQuery($value)
  {
    $this->query = $value;
  }

  private $message = null; 

  public function getMessage()
  {
    return $this->message;
  }
  public function setMessage($value)
  {
    $this->message = $value;
  }

  private $ok = null; 

  public function getOk()
  {
    return $this->ok;
  }
  public function setOk($value)
  {
    $this->ok = $value;
  }

  private $nHits = null; 

  public function getNHits()
  {
    return $this->nHits;
  }
  public function setNHits($value)
  {
    $this->nHits = $value;
  }

  private $first = null; 

  public function getFirst()
  {
    return $this->first;
  }
  public function setFirst($value)
  {
    $this->first = $value;
  }

  private $nHitsBatch = null; 

  public function getNHitsBatch()
  {
    return $this->nHitsBatch;
  }
  public function setNHitsBatch($value)
  {
    $this->nHitsBatch = $value;
  }

  private $batchNumber = null; 

  public function getBatchNumber()
  {
    return $this->batchNumber;
  }
  public function setBatchNumber($value)
  {
    $this->batchNumber = $value;
  }

  private $nBatch = null; 

  public function getNBatch()
  {
    return $this->nBatch;
  }
  public function setNBatch($value)
  {
    $this->nBatch = $value;
  }

  private $hits = null; 

  public function getHits()
  {
    return $this->hits;
  }
  public function setHits($value)
  {
    $this->hits = $value;
  }

  private $imageHits = null; 

  public function getImageHits()
  {
    return $this->imageHits;
  }
  public function setImageHits($value)
  {
    $this->imageHits = $value;
  }

  private $cats = null; 

  public function getCats()
  {
    return $this->cats;
  }
  public function setCats($value)
  {
    $this->cats = $value;
  }

  private $matchingCats = null; 

  public function getMatchingCats()
  {
    return $this->matchingCats;
  }
  public function setMatchingCats($value)
  {
    $this->matchingCats = $value;
  }

  private $languages = null; 

  public function getLanguages()
  {
    return $this->languages;
  }
  public function setLanguages($value)
  {
    $this->languages = $value;
  }

  private $docTypes = null; 

  public function getDocTypes()
  {
    return $this->docTypes;
  }
  public function setDocTypes($value)
  {
    $this->docTypes = $value;
  }

  private $ages = null; 

  public function getAges()
  {
    return $this->ages;
  }
  public function setAges($value)
  {
    $this->ages = $value;
  }

  private $bets = null; 

  public function getBets()
  {
    return $this->bets;
  }
  public function setBets($value)
  {
    $this->bets = $value;
  }

  private $spells = null; 

  public function getSpells()
  {
    return $this->spells;
  }
  public function setSpells($value)
  {
    $this->spells = $value;
  }

  private $queryRefinements = null; 

  public function getQueryRefinements()
  {
    return $this->queryRefinements;
  }
  public function setQueryRefinements($value)
  {
    $this->queryRefinements = $value;
  }

  private $responseId = null; 

  public function getResponseId()
  {
    return $this->responseId;
  }
  public function setResponseId($value)
  {
    $this->responseId = $value;
  }

  private $indexLastModified = null; 

  public function getIndexLastModified()
  {
    return $this->indexLastModified;
  }
  public function setIndexLastModified($value)
  {
    $this->indexLastModified = $value;
  }

} 


class ShortSearchResponse
{ 
  private $message = null; 

  public function getMessage()
  {
    return $this->message;
  }
  public function setMessage($value)
  {
    $this->message = $value;
  }

  private $ok = null; 

  public function getOk()
  {
    return $this->ok;
  }
  public function setOk($value)
  {
    $this->ok = $value;
  }

  private $nHits = null; 

  public function getNHits()
  {
    return $this->nHits;
  }
  public function setNHits($value)
  {
    $this->nHits = $value;
  }

  private $hits = null; 

  public function getHits()
  {
    return $this->hits;
  }
  public function setHits($value)
  {
    $this->hits = $value;
  }

  private $bets = null; 

  public function getBets()
  {
    return $this->bets;
  }
  public function setBets($value)
  {
    $this->bets = $value;
  }

  private $spells = null; 

  public function getSpells()
  {
    return $this->spells;
  }
  public function setSpells($value)
  {
    $this->spells = $value;
  }

} 


class DocType
{ 
  private $id = null; 

  public function getId()
  {
    return $this->id;
  }
  public function setId($value)
  {
    $this->id = $value;
  }

  private $key = null; 

  public function getKey()
  {
    return $this->key;
  }
  public function setKey($value)
  {
    $this->key = $value;
  }

  private $description = null; 

  public function getDescription()
  {
    return $this->description;
  }
  public function setDescription($value)
  {
    $this->description = $value;
  }

  private $nrHits = null; 

  public function getNrHits()
  {
    return $this->nrHits;
  }
  public function setNrHits($value)
  {
    $this->nrHits = $value;
  }

} 


class Lang
{ 
  private $id = null; 

  public function getId()
  {
    return $this->id;
  }
  public function setId($value)
  {
    $this->id = $value;
  }

  private $key = null; 

  public function getKey()
  {
    return $this->key;
  }
  public function setKey($value)
  {
    $this->key = $value;
  }

  private $description = null; 

  public function getDescription()
  {
    return $this->description;
  }
  public function setDescription($value)
  {
    $this->description = $value;
  }

  private $nrHits = null; 

  public function getNrHits()
  {
    return $this->nrHits;
  }
  public function setNrHits($value)
  {
    $this->nrHits = $value;
  }

} 


class Age
{ 
  private $id = null; 

  public function getId()
  {
    return $this->id;
  }
  public function setId($value)
  {
    $this->id = $value;
  }

  private $key = null; 

  public function getKey()
  {
    return $this->key;
  }
  public function setKey($value)
  {
    $this->key = $value;
  }

  private $description = null; 

  public function getDescription()
  {
    return $this->description;
  }
  public function setDescription($value)
  {
    $this->description = $value;
  }

  private $date = null; 

  public function getDate()
  {
    return $this->date;
  }
  public function setDate($value)
  {
    $this->date = $value;
  }

  private $nrHits = null; 

  public function getNrHits()
  {
    return $this->nrHits;
  }
  public function setNrHits($value)
  {
    $this->nrHits = $value;
  }

} 


class getParamRequest6
{ 
  public function __construct() {
    $this->version = WS_VERSION;
  }
  
  private $version = null; 

  public function getVersion()
  {
    return $this->version;
  }
  public function setVersion($value)
  {
    $this->version = $value;
  }

  private $lastQuery = null; 

  public function getLastQuery()
  {
    return $this->lastQuery;
  }
  public function setLastQuery($value)
  {
    $this->lastQuery = $value;
  }

  private $ilang = null; 

  public function getIlang()
  {
    return $this->ilang;
  }
  public function setIlang($value)
  {
    $this->ilang = $value;
  }

  private $getCategories = null; 

  public function getGetCategories()
  {
    return $this->getCategories;
  }
  public function setGetCategories($value)
  {
    $this->getCategories = $value;
  }

  private $getBestBets = null; 

  public function getGetBestBets()
  {
    return $this->getBestBets;
  }
  public function setGetBestBets($value)
  {
    $this->getBestBets = $value;
  }

  private $getDoctypes = null; 

  public function getGetDoctypes()
  {
    return $this->getDoctypes;
  }
  public function setGetDoctypes($value)
  {
    $this->getDoctypes = $value;
  }

  private $getDates = null; 

  public function getGetDates()
  {
    return $this->getDates;
  }
  public function setGetDates($value)
  {
    $this->getDates = $value;
  }

  private $getLanguages = null; 

  public function getGetLanguages()
  {
    return $this->getLanguages;
  }
  public function setGetLanguages($value)
  {
    $this->getLanguages = $value;
  }

} 

class Attachments
{ 
  private $nAttachments = null; 
  
  public function getNAttachments()
  {
    return $this->nAttachments;
  }
  public function setNAttachments($value)
  {
    $this->nAttachments = $value;
  }
  
  private $attachments = null; 
  
  public function getAttachments()
  {
    return $this->attachments;
  }
  public function setAttachments($value)
  {
    $this->attachments = $value;
  }

} 

class ArrayOfCategory
{ 
  private $Category = null; 

  public function getCategory()
  {
    return $this->Category;
  }
  public function setCategory($value)
  {
    $this->Category = $value;
  }

} 


class ArrayOfDocType
{ 
  private $DocType = null; 

  public function getDocType()
  {
    return $this->DocType;
  }
  public function setDocType($value)
  {
    $this->DocType = $value;
  }

} 


class ArrayOfLang
{ 
  private $Lang = null; 

  public function getLang()
  {
    return $this->Lang;
  }
  public function setLang($value)
  {
    $this->Lang = $value;
  }

} 


class ArrayOfAge
{ 
  private $Age = null; 

  public function getAge()
  {
    return $this->Age;
  }
  public function setAge($value)
  {
    $this->Age = $value;
  }

} 


class DateInfo
{ 
  private $s = null; 

  public function getS()
  {
    return $this->s;
  }
  public function setS($value)
  {
    $this->s = $value;
  }

  private $e = null; 

  public function getE()
  {
    return $this->e;
  }
  public function setE($value)
  {
    $this->e = $value;
  }

  private $unknownDateExists = null; 

  public function getUnknownDateExists()
  {
    return $this->unknownDateExists;
  }
  public function setUnknownDateExists($value)
  {
    $this->unknownDateExists = $value;
  }

} 


class ParamResponse
{ 
  private $ok = null; 

  public function getOk()
  {
    return $this->ok;
  }
  public function setOk($value)
  {
    $this->ok = $value;
  }

  private $notUpdated = null; 

  public function getNotUpdated()
  {
    return $this->notUpdated;
  }
  public function setNotUpdated($value)
  {
    $this->notUpdated = $value;
  }

  private $time = null; 

  public function getTime()
  {
    return $this->time;
  }
  public function setTime($value)
  {
    $this->time = $value;
  }

  private $cats = null; 

  public function getCats()
  {
    return $this->cats;
  }
  public function setCats($value)
  {
    $this->cats = $value;
  }

  private $bets = null; 

  public function getBets()
  {
    return $this->bets;
  }
  public function setBets($value)
  {
    $this->bets = $value;
  }

  private $docs = null; 

  public function getDocs()
  {
    return $this->docs;
  }
  public function setDocs($value)
  {
    $this->docs = $value;
  }

  private $dates = null; 

  public function getDates()
  {
    return $this->dates;
  }
  public function setDates($value)
  {
    $this->dates = $value;
  }

  private $ages = null; 

  public function getAges()
  {
    return $this->ages;
  }
  public function setAges($value)
  {
    $this->ages = $value;
  }

  private $langs = null; 

  public function getLangs()
  {
    return $this->langs;
  }
  public function setLangs($value)
  {
    $this->langs = $value;
  }

} 


class getStatsAccessRequest
{ 
  public function __construct() {
    $this->version = WS_VERSION;
  }

  private $version = null; 

  public function getVersion()
  {
    return $this->version;
  }
  public function setVersion($value)
  {
    $this->version = $value;
  }

  private $ilang = null; 

  public function getIlang()
  {
    return $this->ilang;
  }
  public function setIlang($value)
  {
    $this->ilang = $value;
  }

} 


class StatsAccessResponse
{ 
  private $sessionKey = null; 

  public function getSessionKey()
  {
    return $this->sessionKey;
  }
  public function setSessionKey($value)
  {
    $this->sessionKey = $value;
  }

  private $infoUrl = null; 

  public function getInfoUrl()
  {
    return $this->infoUrl;
  }
  public function setInfoUrl($value)
  {
    $this->infoUrl = $value;
  }

} 


class getCachedPageRequest5
{
  public function __construct($url, $query, $ilang, $responseId, $userIdentifier, $userTokens, $target, $baseUrl, $hitNr, $directLink = false) {
    $this->url = $url;
    $this->ilang = $ilang;
    $this->query = $query;
    $this->userIdentifier = $userIdentifier;
    $this->userTokens = $userTokens;
    $this->responseId = $responseId;
    $this->hitNr = $hitNr;
    $this->version = WS_VERSION;
    $this->parameters[] = new SearchParameter('target', $target);
    $this->parameters[] = new SearchParameter('baseUrl', $baseUrl);
    $this->parameters[] = new SearchParameter('directLink', $directLink?'true':'false');
  }

  private $version = null; 

  public function getVersion()
  {
    return $this->version;
  }
  public function setVersion($value)
  {
    $this->version = $value;
  }

  private $ilang = null; 

  public function getIlang()
  {
    return $this->ilang;
  }
  public function setIlang($value)
  {
    $this->ilang = $value;
  }

  private $query = null; 

  public function getQuery()
  {
    return $this->query;
  }
  public function setQuery($value)
  {
    $this->query = $value;
  }

  private $url = null; 

  public function getUrl()
  {
    return $this->url;
  }
  public function setUrl($value)
  {
    $this->url = $value;
  }

  private $hitNr = null; 

  public function getHitNr()
  {
    return $this->hitNr + 1;
  }
  public function setHitNr($value)
  {
    $this->hitNr = $value;
  }

  private $responseId = null; 

  public function getResponseId()
  {
    return $this->responseId;
  }
  public function setResponseId($value)
  {
    $this->responseId = $value;
  }

  private $userIdentifier = null; 

  public function getUserIdentifier()
  {
    return $this->userIdentifier;
  }
  public function setUserIdentifier($value)
  {
    $this->userIdentifier = $value;
  }

  private $userTokens = null;

  public function getUserTokens()
  {
    return $this->userTokens;
  }
  public function setUserTokens($value)
  {
    $this->userTokens = $value;
  }

  private $parameters = null; 

  public function getParameters()
  {
    return $this->parameters;
  }
  public function setParameters($value)
  {
    $this->parameters = $value;
  }
  public function addParameter($value)
  {
    $this->parameters[] = $value;
  }

} 


class Thumbnail
{ 
  private $image = null; 

  public function getImage()
  {
    return $this->image;
  }
  public function setImage($value)
  {
    $this->image = $value;
  }

  private $mimeType = null; 

  public function getMimeType()
  {
    return $this->mimeType;
  }
  public function setMimeType($value)
  {
    $this->mimeType = $value;
  }

} 


class CachedPageResponse
{ 
  private $nmarks = null; 

  public function getNmarks()
  {
    return $this->nmarks;
  }
  public function setNmarks($value)
  {
    $this->nmarks = $value;
  }

  private $page = null; 

  public function getPage()
  {
    return $this->page;
  }
  public function setPage($value)
  {
    $this->page = $value;
  }

  private $query = null; 

  public function getQuery()
  {
    return $this->query;
  }
  public function setQuery($value)
  {
    $this->query = $value;
  }

  private $type = null; 

  public function getType()
  {
    return $this->type;
  }
  public function setType($value)
  {
    $this->type = $value;
  }

  private $dcIdentifier = null; 

  public function getDcIdentifier()
  {
    return $this->dcIdentifier;
  }
  public function setDcIdentifier($value)
  {
    $this->dcIdentifier = $value;
  }

  private $ok = null; 

  public function getOk()
  {
    return $this->ok;
  }
  public function setOk($value)
  {
    $this->ok = $value;
  }

  private $message = null; 

  public function getMessage()
  {
    return $this->message;
  }
  public function setMessage($value)
  {
    $this->message = $value;
  }

  private $thumbnail = null; 

  public function getThumbnail()
  {
    return $this->thumbnail;
  }
  public function setThumbnail($value)
  {
    $this->thumbnail = $value;
  }

} 


class doClickRequest
{ 
  public function __construct($userIdentifier, $mode,
			      $hitNr, $url, $responseId) {
    $this->userIdentifier = $userIdentifier;
    $this->mode = $mode;
    $this->hitNr = $hitNr;
    $this->url = $url;
    $this->responseId = $responseId;
    $this->version = WS_VERSION;
  }
  
  private $version = null; 

  public function getVersion()
  {
    return $this->version;
  }
  public function setVersion($value)
  {
    $this->version = $value;
  }

  private $url = null; 

  public function getUrl()
  {
    return $this->url;
  }
  public function setUrl($value)
  {
    $this->url = $value;
  }

  private $userIdentifier = null; 

  public function getUserIdentifier()
  {
    return $this->userIdentifier;
  }
  public function setUserIdentifier($value)
  {
    $this->userIdentifier = $value;
  }

  private $responseId = null; 

  public function getResponseId()
  {
    return $this->responseId;
  }
  public function setResponseId($value)
  {
    $this->responseId = $value;
  }

  private $hitNr = null; 

  public function getHitNr()
  {
    return $this->hitNr;
  }
  public function setHitNr($value)
  {
    $this->hitNr = $value;
  }

  private $mode = null; 

  public function getMode()
  {
    return $this->mode;
  }
  public function setMode($value)
  {
    $this->mode = $value;
  }

} 


class ClickResponse
{ 
  private $ok = null; 

  public function getOk()
  {
    return $this->ok;
  }
  public function setOk($value)
  {
    $this->ok = $value;
  }

} 
