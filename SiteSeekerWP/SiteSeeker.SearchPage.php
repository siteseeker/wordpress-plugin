<?php

require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'SiteSeeker.inc');

class SiteSeekerSearchPage {
	
	public static function init()
	{
		add_filter('get_search_form', array('SiteSeekerSearchPage', 'get_form'));
		add_action('template_redirect', array('SiteSeekerSearchPage', 'template_redirect'), 1);
	}
	
	public static function template_redirect()
	{
		if ( stripos($_SERVER['REQUEST_URI'], '/search/') === FALSE)
		{
			return;
		}
		
		add_action('wp_title', array('SiteSeekerSearchPage', 'get_title'));

		get_header();
		
		try {
			$search = new SiteSeekerSearch('UTF-8', null, array($wsurl, $username, $password));
			$http = $search->performSearch();
			if ($http == 200) {
				$hits = $search->getSearchHits($http);
				echo $search->getMessage();
				foreach ($hits as $hit) {
					echo $hit->hitNr . '<a href="' . $hit->link . '">' . $hit->title .'</a>';
					echo $hit->hitExtract;
				}
				$pager = $search->getPageNavigation();
				echo $pager->previous;
				echo $pager->pages;
				echo $pager->next;			
			}
		} catch (SoapFault $e) {
			echo 'Could not connect to SiteSeeker: ' . $e->getMessage();
		}
		
		echo get_sidebar();

		get_footer();
		exit;
	}
	
	public static function get_title()
	{
		if (isset($_GET['q']))
		{
			@header("HTTP/1.1 200 OK",1);
			@header("Status: 200 OK", 1);
			return $_GET['q'].' -';
		}
	}
	
	public static function get_form($form)
	{
		if ($form == '') return;
		
		$form = 'hej';

		return $form;
	}
}

SiteSeekerSearchPage::init();


?>