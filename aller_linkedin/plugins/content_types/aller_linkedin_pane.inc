<?php

/**
 * @file
 * Ctools context type plugin that shows LinkedIn profile by provided username.
 *
 */


/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Aller LinkedIn pane'),
  // 'single' => TRUE means has no subtypes.  
  'single' => TRUE,
  'category' => array(t('Aller LinkedIn'), -9),
  // This explicitly declares the config form. 
  'edit form' => 'aller_linkedin_pane_content_type_edit_form',
  // Name of a function which will render the block.
  'render callback' => 'aller_linkedin_pane_content_type_render'
);


/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *
 * @return
 *   An object with at least title and content members.
 */
function aller_linkedin_pane_content_type_render($subtype, $conf, $context = NULL) {
  // As we gonna use JavaScript API to output profiles, let's generate block
  // content with a separate function.
  $block = new stdClass();
  // Let's check if we have context, and we do then use it instead of predefined
  // username value.
  if (isset($context[0])) {
    $block->content = _aller_linkedin_js_content($context[0]);
  }
  else {
    $block->content = _aller_linkedin_js_content($conf['linkedin_username']);
  }
  
  return $block;
}


/**
 * 'Edit form' callback for the content type.
 */
function aller_linkedin_pane_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['linkedin_username'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkedIn username'),
    '#size' => 50,
    '#description' => t('A valid LinkedIn username.'),
    '#default_value' => !empty($conf['linkedin_username']) ? $conf['linkedin_username'] : 'buytaert',
  );
  // no submit
  return $form;
}


/**
 * Submit function, keep in mind that everything in formstate[conf] automatically gets saved 
 */
function aller_linkedin_pane_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['linkedin_username'] = $form_state['values']['linkedin_username'];
}


/**
 * JavaScript helper function to provide block content.
 */
function _aller_linkedin_js_content($linkedin_username) {
  $output = '
    <script type="text/javascript" src="http://platform.linkedin.com/in.js">
      api_key: ' . variable_get('aller_linkedin_apikey', '') . '
      onLoad: onLinkedInLoad
      authorize: true
      scope: r_basicprofile r_network
    </script>';
  
  $output .= '
    <script type="text/javascript">

      function onLinkedInLoad() {
          IN.Event.on(IN, "auth", onLinkedInAuth)
      }

      function onLinkedInAuth() {
        IN.API.Profile("url=http://www.linkedin.com/in/' . $linkedin_username . '")
        .fields("firstName", "lastName", "headline", "picture-url", "public-profile-url", "industry", "relation-to-viewer:(related-connections)")
        .result(displayProfiles)
        .error(displayProfileErrors);
      }

      function displayProfiles(profiles) {
          var member = profiles.values[0];
          document.getElementById("profile").innerHTML = 
            "<img style=\"float: right; margin: 0px 0px 10px 10px; \" src=\"" + member.pictureUrl + "\"><div class=\"name\">" + member.firstName + " " + member.lastName + "</div><div class=\"headline\">" + member.headline + "</div><a href=\"" + member.publicProfileUrl + "\"><img src=\"http://www.linkedin.com/img/webpromo/btn_viewmy_160x33.png\"></a><div class=\"clear\"></div>";
            
            if (typeof member.relationToViewer.relatedConnections != "undefined"){
              displayConnections(member.relationToViewer.relatedConnections);
            }   
              
      }

      function displayConnections(connections) {
        var members = connections.values; // The list of members you are connected to
        var connectionsDiv = document.getElementById("connections");
        
        connectionsDiv.innerHTML += "<div class=\"connected\">You have following connections:</div>";
        
        for (var member in members) {
          if (members[member].id != "private") {
            IN.API.Profile(members[member].id)
            .fields("public-profile-url", "firstName", "lastName", "picture-url", "industry")
            .result(displayConnection)
            .error(displayConnectionsErrors);
          } 
        }     
      }

      function displayConnection(member) {
        var person = member.values[0];
        var connectionsDiv = document.getElementById("connections");
        if (typeof person.pictureUrl == "undefined"){
          person.pictureUrl = "";
        }  
        connectionsDiv.innerHTML += "<a href=\"" + person.publicProfileUrl + "\"><img src=\"" + person.pictureUrl + "\" width=\"40\" height=\"40\"></a>";

        

      }

      function displayConnectionsErrors(error) { 
        document.getElementById("connections").innerHTML = "Error: " + error.message + "";
      }
      
      function displayProfileErrors(error) { 
        document.getElementById("profile").innerHTML = "Error: " + error.message + "";
      }
      
    </script>';
  
  $output .= '<script type="IN/Login"></script>';
  $output .= '<div id="profile"></div><div id="connections" style="clear: both;"></div>';
        
  return $output;
}
