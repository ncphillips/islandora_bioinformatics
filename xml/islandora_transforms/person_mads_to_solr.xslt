<?xml version="1.0" encoding="UTF-8"?>
<!-- MADS -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:foxml="info:fedora/fedora-system:def/foxml#" xmlns:mads="http://www.loc.gov/mads/v2">
    <xsl:template match="/">
        <xsl:param name="prefix">MADS_</xsl:param>
        <xsl:param name="suffix">_ms</xsl:param>
            <xsl:for-each select="mads:mads/mads:authority">
                <xsl:for-each select="./mads:name[@type='personal']">
                    <xsl:variable name="GIVEN" select="./mads:namePart[@type = 'given']"/>
                    <xsl:variable name="FAMILY" select="./mads:namePart[@type = 'family']"/>
                    <xsl:variable name="DATE" select="./mads:namePart[@type = 'date']"/>
                    <xsl:variable name="FULLNAME" select="concat($GIVEN, ' ', $FAMILY)"/>
                    
                   <xsl:if test="$GIVEN != ''">
                   <field>
                        <xsl:attribute name="name">
                            <xsl:value-of select="concat($prefix, 'given', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$GIVEN"/>
                     </field>
                   </xsl:if>

                   <xsl:if test="$FAMILY!= ''">
                   <field>
                        <xsl:attribute name="name">
                            <xsl:value-of select="concat($prefix, 'family', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$FAMILY"/>
                     </field>
                   </xsl:if>

                   <xsl:if test="$DATE!= ''">
                   <field>
                        <xsl:attribute name="name">
                            <xsl:value-of select="concat($prefix, 'date', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$DATE"/>
                     </field>
                   </xsl:if>

                   <xsl:if test="$FULLNAME!= ''">
                   <field>
                        <xsl:attribute name="name">
                            <xsl:value-of select="concat($prefix, 'fullname', $suffix)"/>
                        </xsl:attribute>
                        <xsl:value-of select="$FULLNAME"/>
                     </field>
                   </xsl:if>

                </xsl:for-each>      
           </xsl:for-each>
    </xsl:template>
</xsl:stylesheet>