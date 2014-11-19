<?xml version="1.0" encoding="UTF-8"?>

<!--
 * JCA
 * Mon 30-Sep-2013
 * given a CWRC entity - person, organization, or title - output a DC format datastream
 * designed to work with 1 entity per file
-->


<xsl:stylesheet 
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:mods="http://www.loc.gov/mods/v3" exclude-result-prefixes="mods"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  version="2.0"
  >

  <xsl:output encoding="UTF-8" method="xml" indent="yes" omit-xml-declaration="no" />


  <!--
  * PID value passed into the transform 
  -->
  <xsl:param name="PID_PARAM" select="'zzzz'"/>


  <!--
  * build DC
  -->
  <xsl:template match="/entity | /mods:modsCollection/mods:mods | /mods:mods">
    <oai_dc:dc xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd" >
      <dc:title>
        <xsl:call-template name="GET_DC_TITLE" />
      </dc:title>
      <dc:identifier>
        <xsl:value-of select="$PID_PARAM" />
      </dc:identifier>
    </oai_dc:dc> 
  </xsl:template>


  <!--
  * select the appropriate DC title template
  -->
  <xsl:template name="GET_DC_TITLE">
   <xsl:apply-templates select="person | organization | mods:titleInfo" mode="entity_dc_title" />
  </xsl:template>


  <!--
  * build the DC title - person entity
  * privelege identity/displayLabel
  * over the identity/preferredForm/namePart[@partType="given"] concatenated with the identity/preferredForm/namePart[@partType="family"]
  * over the identity/preferredForm/namePart
  -->
  <xsl:template match="person" mode="entity_dc_title">

    <xsl:choose>
      <!-- displayLabel -->
      <xsl:when test="identity/displayLabel">
        <xsl:value-of select="identity/displayLabel" />
      </xsl:when>
      <!-- family and given -->
      <xsl:when test="identity/preferredForm/namePart/@family or identity/preferredForm/namePart/@given">
        <xsl:if test="identity/preferredForm/namePart/@family">
          <xsl:value-of select="identity/preferredForm/namePart[partType='family']" />
        </xsl:if>
        <xsl:if test="identity/preferredForm/namePart/@family and identity/preferredForm/namePart/@given">
          <xsl:text> </xsl:text>
        </xsl:if>
        <xsl:if test="identity/preferredForm/namePart/@given">
          <xsl:value-of select="identity/preferredForm/namePart[type='given']" />
        </xsl:if>
      </xsl:when>
      <!-- namePart -->
      <xsl:when test="identity/preferredForm/namePart">
          <xsl:value-of select="identity/preferredForm/namePart" />
      </xsl:when>
      <xsl:otherwise> 
        <xsl:text>zzzz ERROR unknown label</xsl:text>
      </xsl:otherwise >
    </xsl:choose>
  </xsl:template>


  <!--
  * build the DC title - organization entity
  * privelege identity/displayLabel over the identity/preferredForm/namePart
  * Note: all organization entities appear to use the <namePart> element for the organization name, with no organization entities using the <displayLabel> element
  -->
 <xsl:template  match="organization" mode="entity_dc_title">

    <xsl:choose>
      <!-- displayLabel -->
      <xsl:when test="identity/displayLabel">
        <xsl:value-of select="identity/displayLabel" />
      </xsl:when>
      <!-- namePart -->
      <xsl:when test="identity/preferredForm/namePart">
          <xsl:value-of select="identity/preferredForm/namePart" />
      </xsl:when>
      <xsl:otherwise> 
        <xsl:text>zzzz ERROR unknown label</xsl:text>
      </xsl:otherwise >
    </xsl:choose>
  </xsl:template>


  <!--
  * build the DC title - Title entity
  * All information for title will come from the following XPath in each MODS record: /mods/titleInfo/title
  * 
  -->
 <xsl:template match="mods:titleInfo" mode="entity_dc_title">
    <xsl:choose>
      <xsl:when test="not(@type) and mods:title">
        <xsl:value-of select="mods:title"/>
      </xsl:when>
      <xsl:when test="not(@type) and @usage='primary' and mods:title ">
        <xsl:value-of select="mods:title"/>
      </xsl:when>
     <xsl:when test="@type='alternative' or @type='abbreviated' or @type='translated' or @type='uniform'">
        <!-- multiple titles, don't use type='alternative' -->
      </xsl:when>
      <xsl:otherwise> 
        <xsl:text>zzzz ERROR unknown label jca</xsl:text>
      </xsl:otherwise >
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>

