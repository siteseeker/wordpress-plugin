<?php
/*
Plugin Name: SiteSeeker Search Integration
Plugin URI: http://wordpress.org
Description: Plugin for SiteSeeker search function.
Author: Euroling AB
Author URI: http://www.siteseeker.se
*/
// Hook for adding admin menus
add_action('admin_menu', 'siteseeker_admin_add_page');
add_action('admin_init', 'siteseeker_admin_init');

// action function for above hook
function siteseeker_admin_add_page() {
    // Add a new submenu under Options:
    add_options_page('SiteSeeker Search Integration', 'SiteSeeker', 'manage_options', 'siteseeker', 'render_siteseeker');
}

// add the admin settings and such
function siteseeker_admin_init() {
	add_settings_section('siteseeker_connection_section', 'Connection Settings', 'render_siteseeker_connection_section', 'siteseeker');
	add_settings_field('siteseeker_wsurl', 'Web service URL', 'render_siteseeker_wsurl', 'siteseeker', 'siteseeker_connection_section');
	add_settings_field('siteseeker_username', 'Username', 'render_siteseeker_username', 'siteseeker', 'siteseeker_connection_section');
	add_settings_field('siteseeker_password', 'Password', 'render_siteseeker_password', 'siteseeker', 'siteseeker_connection_section');
	register_setting('siteseeker', 'siteseeker_connection', 'validate_connection_settings');
}

// section render callback
function render_siteseeker_connection_section() {
	echo '<p>Enter the web service URL and optionally the username and password for the web service below. The connection settings will be validated when saving, so a working connection to the SiteSeeker service must exist.</p>';
}

// field render callbacks
function render_siteseeker_wsurl() {
	$options = get_option('siteseeker_connection');
	echo "<input id='siteseeker_wsurl' name='siteseeker_connection[wsurl]' size='40' type='text' value='{$options['wsurl']}' />";
}
function render_siteseeker_username() {
	$options = get_option('siteseeker_connection');
	echo "<input id='siteseeker_wsurl' name='siteseeker_connection[username]' size='40' type='text' value='{$options['username']}' />";
}
function render_siteseeker_password() {
	$options = get_option('siteseeker_connection');
	$password = empty($options['password']) ? '' : 'DONOTCHANGEPASSWORD';
	echo "<input id='siteseeker_wsurl' name='siteseeker_connection[password]' size='40' type='password' value='$password' />";
}

// form validation
function validate_connection_settings($input) {
	$settings = get_option('siteseeker_connection');
	if (isset($input['wsurl']))
		if (preg_match('#^https?://[^/:]+(:[0-9]+|/ws/[a-z0-9_-]+)/?$#', $input['wsurl']))
			$settings['wsurl'] = $input['wsurl'];
		else
			add_settings_error('siteseeker_wsurl', 'wsurl_empty_or_malformed', 'The web service URL is either empty or malformed.');
	if (isset($input['username']))
		$settings['username'] = $input['username'];
	if (isset($input['password']) && $input['password'] != 'DONOTCHANGEPASSWORD')
		$settings['password'] = $input['password'];
	if (test_connection($settings['wsurl'], $settings['username'], $settings['password']))
		return $settings;
	return $settings;
}

function test_connection($wsurl, $username, $password) {
	require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'SiteSeeker.inc'); 
	$_GET['q'] = "siteseeker:version";
	try {
		$testSearch = new SiteSeekerSearch('UTF-8', null, array($wsurl, $username, $password));
		$http = $testSearch->performSearch();
		if ($http == 200)
			return true;
	} catch (SoapFault $e) {
	    add_settings_error('siteseeker', 'connection_failed', 'Could not connect to SiteSeeker: ' . $e->getMessage());
	}
	return false;
}

// display the admin options page
function render_siteseeker() {
?>
<div>
<h2>SiteSeeker Search Integration</h2>
<p>SiteSeeker provide search services for your blog.</p>
<form action="options.php" method="post">
<?php settings_fields('siteseeker'); ?>
<?php do_settings_sections('siteseeker'); ?>
<input name="Submit" type="submit" value="<?php esc_attr_e('Save settings'); ?>" />
</form></div>
<?php
}

/*


// mt_options_page() displays the page content for the Test Options submenu
function mt_options_page() {
	include(dirname(__FILE__).'SiteSeeker.inc'); 
	if (isset($_POST['testme'])) {
		$_GET['q'] = "siteseeker:version";
		$testSearch = new SiteSeekerSearch('UTF-8');
		$http = $testSearch->performSearch();
		if ($http == 200) {
			$resultOfTest = $testSearch->showMessage();
		}
		else {
			$resultOfTest = "Ett fel uppstod. Kontrollera inställningarna och försök igen!";
		}
	}
	if (isset($_POST['ws-url']) && !isset($_POST['testme'])) {
		echo '<div id="message" class="updated fade"><p>Inställningarna uppdaterades.</p></div>';
		$newConf = file_get_contents(dirname(__FILE__) . '/interface/serverconf.inc');
		$newConf = preg_replace('/\[\'username\'\] = \'.*?\'/','[\'username\'] = \''.$_POST['ws-username'].'\'', $newConf);
		$newConf = preg_replace('/\[\'url\'\] = \'.*?\'/','[\'url\'] = \''.$_POST['ws-url'].'\'', $newConf);
		$newConf = preg_replace('/\[\'password\'\] = \'.*?\'/','[\'password\'] = \''.$_POST['ws-password'].'\'', $newConf);
		$newConf = preg_replace('/\[\'charset\'\] = \'.*?\'/','[\'charset\'] = \''.$_POST['charset'].'\'', $newConf);
		file_put_contents(dirname(__FILE__) . '/interface/serverconf.inc', $newConf);
		$newHitTemplate = stripslashes($_POST['hitTemplate']);
		file_put_contents(dirname(__FILE__) . '/hitTemplate.inc', $newHitTemplate);
		
	}
	include(dirname(__FILE__) . '/interface/serverconf.inc');
	$hitTemplate = file_get_contents(dirname(__FILE__) . '/hitTemplate.inc');
	echo "<div class=\"wrap\">";
	echo "<div id=\"icon-options-general\" class=\"icon32\"><br /></div>";
	echo "<h2>SiteSeeker-inställningar</h2>";
	echo "<h3>Webbtjänst-URL</h3>";
	echo "<table class=\"form-table\">";
	echo "<form action=\"\" method=\"post\">";
	echo "	<tr>";
	echo "		<th>Webbtjänst-URL för SiteSeeker</th> <td><input type=\"text\" name =\"ws-url\" size=\"30\" class=\"regular-text code\" value = \"".$SEARCHER_CONF['url']."\"></td>";
	echo "	</tr>";
	echo "	<tr>";
	echo "		<th>Ev. webbtjänstanvändare</th> <td><input type=\"text\" name =\"ws-username\" size=\"30\" class=\"regular-text code\" value = \"".$SEARCHER_CONF['username']."\"></td>";
	echo "	</tr>";
	echo "	<tr>";
	echo "		<th>Ev. lösenord för webbtjänsten</th> <td><input type=\"text\" name =\"ws-password\" size=\"30\" class=\"regular-text code\" value = \"".$SEARCHER_CONF['password']."\"></td>";
	echo "	</tr>";
	echo "</table>";
	echo "<table class=\"form-table\">";
	echo "<h3>Utseende för träfflistan</h3>";
	echo "	<tr>";
	echo "		<th>HTML-kod för träffutseende</th> <td><textarea name =\"hitTemplate\" cols=\"80\" rows =\"17\" class=\"regular-text code\">".$hitTemplate."</textarea></td>";
	echo "	</tr>";

	echo "</table><br />";
	echo "<input type =\"submit\" value =\"Spara inställningarna\" class=\"button-primary\">";
	echo "</form>";	
	echo "<h3>Test av uppkoppling till sökmotorn</h3>";
	echo "<table class=\"form-table\">";
	echo "<form action=\"\" method=\"post\">";
	echo "	<tr>";
	if (isset($resultOfTest)) {
		echo $resultOfTest;
	}
	echo "	</tr>";
	echo "</table>";
	echo "<input type=\"hidden\" name = \"testme\" value=\"true\">";
	echo "<input type =\"submit\" value =\"Testa anslutningen\" class=\"button-secondary action\">";
	echo "</form>";	
	echo "</div>";

}
*/

require_once(dirname(__FILE__) . DIRECTORY_SEPARATOR . 'SiteSeeker.SearchPage.php');

?>
