BUILD STATUS
------------
Current build status:
[![Build Status](https://travis-ci.org/discoverygarden/islandora_markup_editor.png?branch=7.x)](https://travis-ci.org/discoverygarden/islandora_markup_editor)

Islandora Markup Editor
====================================

## Introduction

Allows for the creation of born digital markup content, made to be viewed and edited in the stand alone CWRC-Writer

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Image Annotation](https://github.com/Islandora/islandora_image_annotation)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.
Ensure islandora_markup_editor is installed and configured, as well as its validation.war (validation) service is deployed.

## CWRC-Writer Validator

A new dependancy has been added to the cwrc-writer with this latest iteration. The cwrc-validator is
a web app that is called by the new cwrc writer when saving a document, or can be called within
the interface of the editor by clicking the validate button. The following instructions will help
you install the validator:
* Build the .war file from source, or use the included .war file in the cwrc-validator repo
(found here: https://github.com/cwrc/cwrc-validator.git, /target/cwrcxmlval-0.0.1-SNAPSHOT.war).
* Rename the .war file to 'validator.war'.
* Deploy the war to tomcats webapp directory (ex: ../tomcat/webapps).
NOTE: Do not use the webapp manager to deploy this .war file.
* After a moment, the .war should deploy and can be tested by visiting
{base_url}:8080/validator/index.html
* If not already configured, Add a reverse proxy setting on the server.
ex: (Debian) edit sites-available/default add the following lines above the </VirtualHost> tag
  ProxyPass /validator/ http://localhost:8080/validator/
  ProxyPassReverse /validator/ http://localhost:8080/validator/
* Restart apache. The validator should now be available to test at
{base_url}/validator/index.html

This module requires the Islandora Critical Edition Solution Pack.
The saxon.jar file contained in the dependencies directory must by moved or
copied to the webapps directory of the tomcat server.

This will normally be /usr/local/fedora/tomcat/webapps/

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors
Current maintainers:

* [Alan Stanley](https://github.com/ajstanley)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

