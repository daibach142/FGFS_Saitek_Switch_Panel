<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" >

<xsl:param name="current-date"/>

<xsl:template match="/plane">
	  <html>
  <body>
   <h2><xsl:value-of select="@name"/> Switch Panel data on <xsl:value-of select="$current-date"/></h2>
   <table border="1">
    <tr bgcolor="#9acd32">
      <th>Switch</th>
      <th>Control</th>
    </tr>
  	<xsl:apply-templates select="switch"/>
    </table> 
   </body> 

   <xsl:apply-templates select="magnetos"/>
   <xsl:apply-templates select="starter"/>
   <xsl:apply-templates select="gear-retarget"/>
</html> 
</xsl:template>
 
 <xsl:template match="switch"> 	
	    <tr>
		    <td><xsl:value-of select="@name"/></td>
		    <td><xsl:value-of select="normalize-space(.)"/></td>
		</tr>
</xsl:template> 

<xsl:template match="magnetos">
<h3>Magnetos: </h3><xsl:value-of select="normalize-space(.)"/>
</xsl:template>

<xsl:template match="starter">
<h3>Starter: </h3><xsl:value-of select="normalize-space(.)"/>
</xsl:template>

<xsl:template match="gear-retarget">
<h3>Gear Retargeted: </h3><xsl:value-of select="normalize-space(.)"/>
</xsl:template>

</xsl:stylesheet>


