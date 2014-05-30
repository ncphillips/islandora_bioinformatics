<?xml version="1.0" encoding="UTF-8"?>
<!-- MADS -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:foxml="info:fedora/fedora-system:def/foxml#" xmlns:mads="http://www.loc.gov/mads/v2">
    <xsl:template match="/">
        <xsl:param name="prefix">MADS_</xsl:param>
        <xsl:param name="suffix">_ms</xsl:param>
            <xsl:for-each select="mads:mads/mads:authority">
               <xsl:for-each select="./mads:titleInfo">
                 <xsl:variable name="TITLE" select="./mads:title"/>

                 <xsl:if test="$TITLE!= ''">
                    <field>
                        <xsl:attribute name="title">
                            <xsl:value-of select="concat($prefix, 'title', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$TITLE"/>
                    </field>
                </xsl:if>
              </xsl:for-each>
               <xsl:variable name="IDENTIFIER" select="./mads:identifier"/>
               <xsl:if test="$IDENTIFIER != ''">
                    <field>
                        <xsl:attribute name="identifier">
                            <xsl:value-of select="concat($prefix, 'identifier', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$IDENTIFIER"/>
                    </field>
                </xsl:if> 
            </xsl:for-each> 

            <xsl:for-each select="mads:mads/mads:affiliation">
                 <xsl:variable name="ORGANIZATION" select="./mads:organization"/>

                 <xsl:if test="$ORGANIZATION!= ''">
                    <field>
                        <xsl:attribute name="organization">
                            <xsl:value-of select="concat($prefix, 'organization', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$ORGANIZATION"/>
                    </field>
                </xsl:if>

               <xsl:variable name="CITY" select="./mads:address/mads:city"/>
               <xsl:variable name="STREET" select="./mads:address/mads:street"/>
               <xsl:variable name="COUNTRY" select="./mads:address/mads:country"/>
               <xsl:variable name="POSTCODE" select="./mads:address/mads:postcode"/>
               <xsl:if test="$CITY!= ''">
                    <field>
                        <xsl:attribute name="city">
                            <xsl:value-of select="concat($prefix, 'city', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$CITY"/>
                    </field>
                </xsl:if> 
                <xsl:if test="$STREET!= ''">
                    <field>
                        <xsl:attribute name="street">
                            <xsl:value-of select="concat($prefix, 'street', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$STREET"/>
                    </field>
                </xsl:if> 
                <xsl:if test="$COUNTRY!= ''">
                    <field>
                        <xsl:attribute name="country">
                            <xsl:value-of select="concat($prefix, 'country', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$COUNTRY"/>
                    </field>
                </xsl:if> 
                <xsl:if test="$POSTCODE!= ''">
                    <field>
                        <xsl:attribute name="city">
                            <xsl:value-of select="concat($prefix, 'postcode', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$POSTCODE"/>
                    </field>
                </xsl:if> 
            </xsl:for-each> 
    </xsl:template>
</xsl:stylesheet>