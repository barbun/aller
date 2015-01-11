INTRODUCTION
------------
Maintainer:
  Roman Barbun (https://drupal.org/user/1485424)

OVERVIEW
--------

This small module provides a reusable panel pane to pull LinkedIn profiles with
LinkedIn API. 


SETTING UP
----------
1) Enable the module and "Aller LinkedIn" content type will appear when adding 
content to a panel. You have 2 options to pull profile at the moment - using
predefined username or by using panel context.

FOR EXAMPLE: when you go to the pane settings and provide a value for "LinkedIn 
username", then that user profile will be pulled by default, but 
if you specify optional path element in panel basic Path settings 
(e.g. "!username") and assign "String" context to that argument, then you can pull
user profile dynamically by passing his LinkedIn username to URL.
(e.g. panelpage/buytaert).

If you have connections with specified user then they will be displayed right 
under his profile.
 
2) To be able to use LinkedIn API you need to provide your application 
credentials. Please be sure to go to "admin/config/development/aller_linkedin"
and provide them.

3) Set up permission "Aller LinkedIn settings".

DEPENDENCIES
------------
This module is dependant on CTools contrib module.

TO-DO:
1) Add better handling of contexts (not only arguments, but also use user object
 etc).
2) Add direct message form and connect button.
3) Add "Viewers of this profile also viewed" block and another relative blocks.
4) Theme the output a bit better.