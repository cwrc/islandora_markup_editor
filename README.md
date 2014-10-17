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

#### URL syntax
islandora/markupeditor/oa/HasTarget/{uri encoded}

For paging of results - unsupported as of 2014-10-06

islandora/markupeditor/oa/HasTarget/{uri encoded}?page.start={}&page.rows={}

#### HTTP Method
GET

#### Headers
Accept: application/json

#### Request parameters
| Name          | Description                                                  |
| ------------- | ------------------------------------------------------------ |
| page.start    | (optional) For paging of results - the results page    
| page.rows     | (optional) For paging of results - the number of rows per page    

#### Response: 200 OK
##### Content-Type: application/json
| Name          | Description                                                  |
| ------------- | ------------------------------------------------------------ |
| hasTarget     | The query 'hasTarget' uri  
| numFound      | The number of results in the 'oa_items' array
| page.start    | (optional) For paging of results - the results page of the current 'oa_items'    
| page.rows     | (optional) For paging of results - the number of rows per page    
| oa_items      | The number of results identified by the 'hasTarget' uri    


#### Example Response
```JSON
{
    "hasTarget": "islandora:b305fc90-a59e-415e-8c58-f7ee5d06ad2f",
    "numFound": "0",
    "page": {
        "start": "0",
        "rows": "5"
    },
    "oa_items": [{},{},...]
}
```





## Mint URI 

#### URL syntax
islandora/markupeditor/oa/mintURI

#### HTTP Method
GET

#### Headers
Accept: application/json

#### Request parameters
None

#### Response: 200 OK
##### Content-Type: application/json
| Name          | Description                                                  |
| ------------- | ------------------------------------------------------------ |
| uri           | OA minted annotation

#### Example Response
```JSON
{"uri","http:\/\/id.cwrc.ca\/cwrc:2755fb77-b11a-43b8-947c-43654b1cb6d6"}
```




## Create A New Annotation

#### URL syntax
islandora/markupeditor/oa/annotation

#### HTTP Method
POST

#### Headers
Accept: application/json

#### POST (form-data)
| Name          | Description                                                  |
| ------------- | ------------------------------------------------------------ |
| URI           | The identifier for the annotation 
| hasTarget     | hasTarget URI for the annotation
| annotation    | RDF XML serialiazation of the annotation 

#### Response: 201 Created
##### Content-Type: application/json
Returns the same response as a [GET Annotation](#response-200-ok) request.



## Update An Existing Annotation

#### URL syntax
islandora/markupeditor/oa/annotation/{URI}

#### HTTP Method
PUT

#### Headers
Accept: application/json

#### POST (form-data)
| Name          | Description                                                  |
| ------------- | ------------------------------------------------------------ |
| annotation    | RDF XML serialiazation of the annotation 

#### Response: 200 
##### Content-Type: application/json
Returns the same response as a [GET Annotation](#response-200-ok) request.




## Get An Existing Annotation

#### URL syntax
islandora/markupeditor/oa/annotation/{URI}

#### HTTP Method
GET

#### Headers
Accept: application/json

#### Request parameters
none

#### Response: 200
##### Content-Type: application/json
Returns the same response as a [GET Annotation](#response-200-ok) request.




## Delete An Existing Annotation

#### URL syntax
islandora/markupeditor/oa/annotation/{URI}

#### HTTP Method
DELETE

#### Headers
Accept: application/json

#### Request parameters
none

#### Response: 200
##### Content-Type: application/json
Returns the same response as a [GET Annotation](#response-200-ok) request.








### Common Responses
Unless otherwise specified each end-point can return the following responses.

#### Response: 401 Unauthorized
##### No response body.
In general 401 means an anonymous user attempted some action and was denied.
Either the action is not granted to anonymous uses in the Drupal permissions,
or XACML denied the action for the anonymous user.

#### Response: 403 Forbidden
##### No response body.
In general 403 means an authenticated user attempted some action and was denied.
Either the authenticated user does not have a Drupal role that grants him/her
permission to perform that action, or XACML denied the action for that user.

#### Response: 404 Not Found
##### No response body.
In general a 404 will occur, when a user tries to perform an action on a
object or datastream that doesn't not exist. Or XACML is hiding that
object or datastream from the user.

404 Responses can be returned even if the user was **not** determined to have
permission to perform the requested action, as the resource must be first
fetched from fedora before the users permission can be determined.

#### Response: 500 Internal Server Error

##### Content-Type: application/json
| Name          | Description                                                  |
| ------------- | ------------------------------------------------------------ |
| message       | A detail description of the error.

Any problem can trigger a 500 error, but in general you'll find that this is
typically an error returned by Fedora.

