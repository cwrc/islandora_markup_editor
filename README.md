BUILD STATUS
------------
Current build status:
[![Build Status](https://travis-ci.org/discoverygarden/islandora_markup_editor.png?branch=7.x)](https://travis-ci.org/discoverygarden/islandora_markup_editor)

Islandora Markup Editor
===================================

## Introduction

Allows for the creation of born digital markup content, made to be viewed and edited in the stand alone CWRC-Writer

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Paged Content](https://github.com/Islandora/islandora_paged_content)
* [Islandora Critical Edition](https://github.com/discoverygarden/islandora_critical_edition)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.
Ensure islandora_critical_edition is installed and configured, as well as its validation.war (validation) service is deployed.

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





# Documentation: CWRC-Writer API
#


## setup info for the cwrc-writer/viewer

#### URL syntax
islandora/markupeditor/setup/{PID}

#### HTTP Method
GET


#### Response: 200 OK
##### Content-Type: application/json

#### Example Response
```JSON
{
    "uid": "1",
    "position": "1",
    "pages": "islandora:b305fc90-a59e-415e-8c58-f7ee5d06ad2f",
    "title": "Sample xml doc",
    "no_edit": false,
    "page_count": 1,
    "islandora_anno_stroke_width": "1%",
    "create_entity_callbacks": {
        "places": "/islandora/object/islandora%3Aentity_collection/manage/overview/ingest",
        "events": "/islandora/object/islandora%3Aentity_collection/manage/overview/ingest",
        "organizations": "/islandora/object/islandora%3Aentity_collection/manage/overview/ingest",
        "people": "/islandora/object/islandora%3Aentity_collection/manage/overview/ingest"
    }
}
```



## persist document data

#### URL syntax
islandora/markupeditor/save_data/{PID}

#### HTTP Method
PUT

#### Request parameters
#### Request parameters
| Name          | Description                                                  |
| ------------- | ------------------------------------------------------------ |
| text          | content to persist
| schema        | schema describing the content
| valid         | (optional)

#### Response: 200 OK
##### Content-Type: application/json

response is the document



# Documentation: Open Annotation API


## set of annotations with a given 'hasTarget'

moved to https://github.com/cwrc/cwrc_entities
