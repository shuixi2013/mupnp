/******************************************************************
*
*	CyberNet for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: chttp_attribute_list.cpp
*
*	Revision:
*
*	02/07/05
*		- first revision
*
******************************************************************/

#include <mupnp/xml/xml.h>
#include <mupnp/util/log.h>

/****************************************
* mupnp_xml_attributelist_new
****************************************/

CgXmlAttributeList *mupnp_xml_attributelist_new()
{
	CgXmlAttributeList *attrList;

	mupnp_log_debug_l4("Entering...\n");

	attrList = (CgXmlAttributeList *)malloc(sizeof(CgXmlAttributeList));

	if ( NULL != attrList )
	{
		mupnp_list_header_init((CgList *)attrList);
		attrList->name = NULL;
		attrList->value = NULL;
	}

	return attrList;

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_xml_attributelist_delete
****************************************/

void mupnp_xml_attributelist_delete(CgXmlAttributeList *attrList)
{
	mupnp_log_debug_l4("Entering...\n");

	mupnp_xml_attributelist_clear(attrList);
	free(attrList);

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_xml_attributelist_getattribute
****************************************/

CgXmlAttribute *mupnp_xml_attributelist_get(CgXmlAttributeList *attrList, const char *name)
{
	CgXmlAttribute *attr;
	const char *attrName;
		
	mupnp_log_debug_l4("Entering...\n");

	if (name == NULL)
		return NULL;
		
	for (attr = mupnp_xml_attributelist_gets(attrList); attr != NULL; attr = mupnp_xml_attribute_next(attr)) {
		attrName = mupnp_xml_attribute_getname(attr);
		if (attrName == NULL)
			continue;
		if (mupnp_strcasecmp(attrName, name) == 0)
			return attr;
	}
	
	return NULL;

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_xml_attributelist_setattribute
****************************************/

void mupnp_xml_attributelist_set(CgXmlAttributeList *attrList, const char *name, const char *value)
{
	CgXmlAttribute *attr;
	
	mupnp_log_debug_l4("Entering...\n");

	attr = mupnp_xml_attributelist_get(attrList, name);
	if (attr == NULL) {
		attr = mupnp_xml_attribute_new();
		mupnp_xml_attributelist_add(attrList, attr);
		mupnp_xml_attribute_setname(attr, name);
	}
	
	mupnp_xml_attribute_setvalue(attr, value);

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_xml_attributelist_getvalue
****************************************/

const char *mupnp_xml_attributelist_getvalue(CgXmlAttributeList *attrList, const char *name)
{
	CgXmlAttribute *attr;

	mupnp_log_debug_l4("Entering...\n");

	attr = mupnp_xml_attributelist_get(attrList, name);
	if (attr == NULL)
		return NULL;
	return mupnp_xml_attribute_getvalue(attr);

	mupnp_log_debug_l4("Leaving...\n");
}
