<?php 
/*  
SiteSeeker WordPress module
*/
require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'php' . DIRECTORY_SEPARATOR . 'class.seeker.inc');

class SiteSeekerSearch {
  /**
   * Constructor
   * @param web service url, web service username, web service password, search encoding, wordpress post object
   * @return n/a
   */
  function __construct($encoding, $post = null, $connection = null) {
	if ($connection == null)
		list($wsurl, $username, $password) = get_option('siteseeker_connection');
    $this->postID = ($post != null)?$post->ID:false;
    $this->ilang = "sv";
    $this->query = htmlspecialchars(isset($_GET['q'])?$_GET['q']:'');
    $this->query = stripslashes($this->query);
    $this->sortOrder = htmlspecialchars(isset($_GET['so'])?$_GET['so']:'');
    $this->category = htmlspecialchars(isset($_GET['cc'])?$_GET['cc']:'');
    $this->searchAll = htmlspecialchars(isset($_GET['hs'])?$_GET['hs']:'');
    $this->extension = htmlspecialchars(isset($_GET['ext'])?$_GET['ext']:'');
    $this->responseId = htmlspecialchars(isset($_GET['id'])?$_GET['id']:'');
    $this->spell = htmlspecialchars(isset($_GET['sp'])?$_GET['sp']:'');
    $this->encoding = $encoding;
    $empty_items = htmlspecialchars(isset($_GET['empty'])?$_GET['empty']:'');
    if (isset($_GET['uid'])) {
      $this->userIdentifier = htmlspecialchars($_GET['uid']);
    }
    else {
      $this->userIdentifier = md5($_SERVER['REMOTE_ADDR'].time());
    }
    $this->parameters = array();
    $this->parameters[] = new SearchParameter('metaAttributes', true);
    $this->parameters[] = new SearchParameter('clientIpAddress', $_SERVER['REMOTE_ADDR']);
    if ($empty_items) {
      $this->parameters[] = new SearchParameter('emptyItemsInOverview', true);
    }
    $this->search = new Connector($wsurl, $username, $password, $this->encoding, $this->ilang);
	$this->params = $this->search->getSearchContext(true);
  }

  /**
   * perform search request
   * @param query extension, interfacekey
   * @return result of query (200 == OK, 500 == error)
   */
  function performSearch($extension = "", $interfaceKey = "normal") {
    $unextendedQuery = $this->query;
    $searchableKeyword = "";
    $this->query .= " ".$extension;
    if (is_array($this->extension)) {
      foreach ($this->extension as $ext) {
        $this->query .= " ".str_replace(" ", "_", "hidden:meta:category:".$this->categoryGroupName($ext).";".$this->categoryName($ext));
      }
    }
    
    $doSearch = new doSearchRequest6($this->query,$this->batch,$this->sortOrder,$this->category,$doctype,-2,'','', $this->spell, $this->ilang,10,$this->userIdentifier,$_SERVER['REMOTE_ADDR'],$interfaceKey, '', $this->parameters, false, '', '');
    $http = $this->search->search($doSearch, 0);
	try {
    	$this->query = $unextendedQuery;
	} catch (SoapFault $e) {
		if ($e->message == 'Not Found')
			$http = 404;
		else
			$http = 500;
	}
    return $http;
  }

  /**
   * retrieve and pre-process search hits
   * @param value returned from performSearch
   * @return array of search hits
   **/
  function getSearchHits($http) {
    if ($http == 200) {
      $hits = $this->search->getHits();
      $rhits = array();
      if (is_array($hits)) {
        foreach ($hits as $hit) {
          $rhit = new rhit;
          // set attributes for each hit (meta values available as $hit->property['name'] in interface)
          $rhit->link = $hit->getSourceLink();
          $rhit->title = $hit->getTitle();
          $rhit->categoryIds = $hit->getCategoryIds();
          $rhit->hitExtract = $hit->getDescription();
          $rhit->responseId = $this->responseId;			
          $rhit->hitNr = $hit->getHitNr(); 
          $rhit->sessionId = $this->userIdentifier;
          foreach ($hit->getMetaAttributes()->getMetaAttribute() as $property) {
            $rhit->property[$property->getName()] = $property->getValue();
          }
          $rhits[]= $rhit;
        }
      }
    }
    // for click tracking
    if ($this->query) {      
      echo '<script type="text/javascript" src="tracker.php?action=activate&amp;id='.htmlspecialchars($this->userIdentifier).'&amp;response=&amp;url=&amp;hitnr=" defer="defer"></script>';
    }
    return $rhits;
  }

    /**
     * Pager to switch between batches
     * @param the parameter to use for paging
     * @return pager object
     */
  function getPageNavigation($param = "pg") {
    $url = $_GET;
    $editValue = htmlspecialchars($url[$param]);
    // collect and rewrite URL params
    foreach ($url as $urlParam => $value) {
      if ($urlParam != $param) {
        if (is_array($value)) {
          foreach ($value as $val) {
            $urlParams[] = '&'.$urlParam.'[]='.$val;
          }
        }
        else {
          $urlParams[] = '&'.$urlParam."=".$value;
        }
      }
    }
    $pager = new pager;
    if ($this->batch > 10) $pager->previous10 = '<a href="?'.$param.'='.($editValue-10).implode('',$urlParams).'">-10</a></span>';
    if ($this->batch > 1) $pager->previous .= '<a href="?'.$param.'='.($editValue-1).implode('',$urlParams).'">FÖREGÅENDE</a></span>';
    $firstBatch = ($this->batch-5);
    if ($firstBatch < 1) $firstBatch = 1;
    $lastBatch = ($this->batch+5);
    if ($lastBatch > $this->search->getResponse()->getNBatch()) $lastBatch = $this->search->getResponse()->getNBatch();
    for ($i=$firstBatch; $i<=$lastBatch; $i++) {
      if ($this->batch == $i) {
        if ($this->search->getResponse()->getNBatch() >1 ) {
          $pager->pages .= "<span class=\"pagenum current\">$i </span> ";
        }	
      }
      else {
        $pager->pages .="<span class=\"previous\"><a href=\"?".$param."=".$i.implode('',$urlParams)."\">$i</a></span> ";
      }
    }
    if ($this->batch < $this->search->getResponse()->getNBatch()) $pager->next = '<a href="?'.$param.'='.($editValue+1).implode('',$urlParams).'">NÄSTA</a></span>';
    if ($this->batch < ($this->search->getResponse()->getNBatch()-10)) $pager->next10 = '<a href="?'.$param.'='.($editValue+10).implode('',$urlParams).'">+10</a></span>';
    return $pager;
  }
	/** 
   * return categories pre-query
   * @param first letter of category name, category group
   * @return array of category objects
   */
  function showStaticCategories($letter, $group = null) {
    $rcats = array();
    $this->search = new Connector('', $this->encoding, $this->ilang);
    $searchContext = $this->search->getSearchContext(true);
    $this->params = $searchContext;
    $categories = $this->params->getCats()->Category;
    usort($categories, array($this,"cmp_param_obj_alpha"));		
    foreach ($categories as $category) {
      // To return only the categories starting with a specified letter.
      if ($letter == mb_substr($category->name, 0, 1)) {
         if (isset($group) && $group == $category->groupName || !isset($group)) {
           $rcats[] = $category;
         }
      }
    }
    return $rcats;
  }

	/** 
   * return categories
   * @param sortmode (alphabetical, sizewise)
   * @return array of category objects
   */
  function getCategories($sortMode) {
    $categories = $this->search->getCategories();
    $rcat = array();
    usort($categories, array($this, $sortMode == "size" ? "cmp_obj_size":"cmp_obj_alpha"));
    foreach ($categories as $cat) {
      $rcat = new rcat;
      // set category as active if $_GET['(cc|ext)'] selected
      if ($this->category) {
        foreach ($this->category as $selected) {
          if ($selected == $cat->getId()) {
            $rcat->selected = true;
          }
        }
      }
      if ($this->extension) {
        foreach ($this->extension as $selected) {
          if ($selected == $cat->getId()) {
            $rcat->selected = true;
          }
        }
      }
      $categoryIds = $this->addCategory($cat->getId());
      $rcat->name = $cat->getName();
      $rcat->id = $categoryIds;
      $rcat->size = $cat->getNHits();
      $rcat->searchAll = $this->searchAll;
      $rcat->query = $this->query;
      $rcat->postID = $this->postID;
      $rcat->group = $cat->getGroupName();
      $categoriesToReturn[] = $rcat;
    }
    return $categoriesToReturn;
  }

  function getMessage() {
    return $this->search->getResponse()->getMessage();
  }
  /**
   * To enable multiple selected categories (as extensions)
   * @param id 
   * @return aggregate category ids
   */
  function addCategory($id) {
    $categoryIds = "";
    if ($id) {
      if (is_array($this->extension)) {
        foreach ($this->extension as $selectedCategory) {
          if ($this->categoryGroup($selectedCategory) != $this->categoryGroup($id)) {
            $categoryIds = $selectedCategory."&ext[]=".$id;
          }
        }
        if ($categoryIds == "") {
          $categoryIds = $id;
        }
        return $categoryIds;
      }
      return $id;
    }
  }
  /**
   * Return best bets
   * @param n/a
   * @return array of best bet objects
   *
  */
  function getBestBets() {
    $rbets = array();
    foreach ($this->search->getBestBets() as $bet) {
      $rbet = new rbet;
      $rbet->nr = $bet->getBetNr();
      $rbet->url = $bet->getBetUrl();
      $rbet->description = $bet->getBetDescription();
      $rbets[] = $rbet;
    }
    return $rbets;
  }
	
  /**
   * return categories matching query
   * @param n/a
   * @return array of categories matching query
   */
  function getMatchingCategories() {
    if ($currentCategory = $_GET['catmatch']) {
      return $currentCategory;
    }
    if (is_object($this->search->getResponse()->getMatchingCats())) {
      $matchingCategories = $this->search->getResponse()->getMatchingCats()->getHitCategory();
      if (is_array($matchingCategories)) {
        return $matchingCategories;
      }
    }
    return "";
  }
	  /**
   * return spelling suggestions
   * @param n/a
   * @return array of spelling suggestions as html
   */
  function getSpellingSuggestions() {
    $spellingSuggestions = $this->search->getSpellingSuggestions();
    if (sizeof($spellingSuggestions) > 0) {
      foreach ($spellingSuggestions as $suggestion) {
        $all_suggestions[] = "<a href=\"?q=".$suggestion->getSpell()."&sp=1&uid=".$this->userIdentifier."&id=".$this->responseId."\">".$suggestion->getHtmlSpell()."</a>";
      }
      return $all_suggestions;
    }
  }
	  /**
   * return category name from id
   * @param id
   * @return name
   */
  function categoryName($id) {
    $name = "";	
    if (is_object($this->params)) {
      $categories = $this->params->getCats()->Category;
      if (is_array($categories)) {
        foreach ($categories as $category) {
          if ($category->id == $id && strpos($category->name, "Alla") === false) {
            $name = $category->name; 
          }
        }
      }
    }
    return $name;
  }
	  /**
   * return category group id from id
   * @param id
   * @return category group id
   */
  function categoryGroup($id) {
    $group = "";	
    if (is_object($this->params)) {
      $categories = $this->params->getCats()->Category;
      if (is_array($categories)) {
        foreach ($categories as $category) {
          if ($category->id == $id) {
            $group = $category->groupId; 
          }
        }
      }
    }
    return $group;
  }
	  /**
   * return category group name from id
   * @param id
   * @return category group name
   */
  function categoryGroupName($id) {
    $group = "";	
    if (is_object($this->params)) {
      $categories = $this->params->getCats()->Category;
      if (is_array($categories)) {
        foreach ($categories as $category) {
          if ($category->id == $id) {
            $group = $category->groupName; 
          }
        }
      }
    }
    return $group;
  }

  // Comparison functions for usort()
  static function cmp_obj_size($a, $b)
  {
    $al = strtolower($a->getNDocs());
    $bl = strtolower($b->getNDocs());
    if ($al == $bl) {
      return 0;
    }
    return ($al < $bl) ? +1 : -1;
  }
  static function cmp_obj_alpha($a, $b)
  {
    $al = strtolower($a->getName());
    $bl = strtolower($b->getName());
    if ($al == $bl) {
      return 0;
    }
    return ($al > $bl) ? +1 : -1;
  }
  static function cmp_param_obj_alpha($a, $b)
  {
    $al = strtolower($a->name);
    $bl = strtolower($b->name);
    if ($al == $bl) {
      return 0;
    }
    return ($al > $bl) ? +1 : -1;
  }
  static function cmp_hit_property($a, $b)
  {
    $al = sizeof($a->property);
    $bl = sizeof($b->property);
    if ($al == $bl) {
      return 0;
    }
    return ($al < $bl) ? +1 : -1;
  }
}

// return classes
class rhit {
  function __construct() {
    $rhit->link = "";
    $rhit->title = "";
    $rhit->categoryIds = array();
    $rhit->hitExtract = "";
    $rhit->responseId = "";			
    $rhit->hitNr = ""; 
    $rhit->sessionId = "";
    $rhit->property = array();
  }
}
class rcat {
  function __construct() {
    $rcat->name = "";
    $rcat->id = "";
    $rcat->size = "";
    $rcat->selected = null;
    $rcat->searchAll = "";
    $rcat->query = "";
    $rcat->postID = "";
    $rcat->group = "";
  }
}
class rbet {
  function __construct() {
    $rbet->nr = "";
    $rbet->url = "";
    $rbet->description = "";
  }
}
class pager {
  function __construct() {
    $pager->previous10 = "";
    $pager->previous = "";
    $pager->pages = "";
    $pager->next = "";
    $pager->next10 = "";
  }
}

function selfURL() { 
  $s = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : ""; 
  $protocol = strleft(strtolower($_SERVER["SERVER_PROTOCOL"]), "/").$s; 
  $port = ($_SERVER["SERVER_PORT"] == "80") ? "" : (":".$_SERVER["SERVER_PORT"]); 
  return $protocol."://".$_SERVER['SERVER_NAME'].$port.$_SERVER['REQUEST_URI']; 
} 

function strleft($s1, $s2) { 
  return substr($s1, 0, strpos($s1, $s2)); 
}
