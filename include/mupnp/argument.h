/******************************************************************
*
*	CyberLink for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: cargument.h
*
*	Revision:
*
*	02/23/05
*		- first revision
*
******************************************************************/

#ifndef _CG_UPNP_CARGUMENT_H_
#define _CG_UPNP_CARGUMENT_H_

#include <mupnp/typedef.h>
#include <mupnp/xml/xml.h>
#include <mupnp/util/list.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

/** Definition for argument element name */
#define CG_UPNP_ARGUMENT_ELEM_NAME "argument"

/** Definition for argument list element name */
#define CG_UPNP_ARGUMENTLIST_ELEM_NAME "argumentList"

/** Definition for argument passing direction (in) */
#define CG_UPNP_ARGUMENT_IN_DIR "in"

/** Definition for argument passing direction (out) */
#define CG_UPNP_ARGUMENT_OUT_DIR "out"

/** Definition for argument element name */
#define CG_UPNP_ARGUMENT_NAME "name"

/** Definition for argument element "direction" */
#define CG_UPNP_ARGUMENT_DIRECTION "direction"

/** Definition for argument related state variable */
#define CG_UPNP_ARGUMENT_RELATED_STATE_VARIABLE "relatedStateVariable"

/****************************************
* Data Type
****************************************/

/**
 * Data type definition for UPnP argument and UPnP argument list
 */
typedef struct _CgUpnpArgument {
	BOOL headFlag;
	struct _CgUpnpArgument *prev;
	struct _CgUpnpArgument *next;
	CgXmlNode *serviceNode;
	CgXmlNode *argumentNode;
	/**** Execution Data ****/
	CgString *value;
} CgUpnpArgument, CgUpnpArgumentList;

/****************************************
* Function (Argument)
****************************************/

/**
 * Create new argument
 */
CgUpnpArgument *mupnp_upnp_argument_new();

/**
 * Delete argument
 *
 * @param dev Argument
 */
void mupnp_upnp_argument_delete(CgUpnpArgument *dev);

/**
 * Get next argument using argument as iterator
 *
 * @param arg Argument node
 */
#define mupnp_upnp_argument_next(arg) (CgUpnpArgument *)mupnp_list_next((CgList *)arg)

/**
 * Check if XML node is argument node
 *
 * @param node XML node
 *
 * @return True if node is argument node, false otherwise
 */
#define mupnp_upnp_argument_isargumentnode(node) mupnp_xml_node_isname(node, CG_UPNP_ARGUMENT_ELEM_NAME)

/**
 * Set XML service node for argument
 *
 * @param arg Argument
 * @param node XML service node
 */
#define mupnp_upnp_argument_setservicenode(arg,node) (arg->serviceNode = node)

/**
 * Get service node from argument
 *
 * @param arg Argument
 */
#define mupnp_upnp_argument_getservicenode(arg) (arg->serviceNode)

/**
 * Set XML argument node for argument
 *
 * @param arg Argument
 * @param node XML argument node
 */
#define mupnp_upnp_argument_setargumentnode(arg,node) (arg->argumentNode = node)

/**
 * Get XML argument node from argument
 *
 * @param arg Argument
 */
#define mupnp_upnp_argument_getargumentnode(arg) (arg->argumentNode)

/**** name ****/

/**
 * Set name for argument
 *
 * @param arg Argument
 * @param value Argument name
 */
#define mupnp_upnp_argument_setname(arg, value) mupnp_xml_node_setchildnode(mupnp_upnp_argument_getargumentnode(arg), CG_UPNP_ARGUMENT_NAME, value)

/**
 * Get argument name
 *
 * @param arg Argument
 */
#define mupnp_upnp_argument_getname(arg) mupnp_xml_node_getchildnodevalue(mupnp_upnp_argument_getargumentnode(arg), CG_UPNP_ARGUMENT_NAME)

/**
 * Check if current argument name matches with the provided name
 *
 * @param arg Argument
 * @param name Name matched with argument name
 *
 * @return True if argument name equals with the provided name, false otherwise
 */
#define mupnp_upnp_argument_isname(arg, name) mupnp_streq(mupnp_upnp_argument_getname(arg), name)

/**** direction ****/

/**
 * Set direction for argument
 *
 * @param arg Argument
 * @param value New direction value
 */
#define mupnp_upnp_argument_setdirection(arg, value) mupnp_xml_node_setchildnode(mupnp_upnp_argument_getargumentnode(arg), CG_UPNP_ARGUMENT_DIRECTION, value)

/**
 * Get argument direction
 * 
 * @param arg Argument
 *
 * @return Argument direction
 */
#define mupnp_upnp_argument_getdirection(arg) mupnp_xml_node_getchildnodevalue(mupnp_upnp_argument_getargumentnode(arg), CG_UPNP_ARGUMENT_DIRECTION)

/**
 * Check if argument direction is "in"
 *
 * @param arg Argument
 *
 * @return True if argument directon is "in"
 */
#define mupnp_upnp_argument_isindirection(arg) mupnp_streq(mupnp_upnp_argument_getdirection(arg), CG_UPNP_ARGUMENT_IN_DIR)

/**
 * Check if argument direction is "out"
 *
 * @param arg Argument
 *
 * @return True if argument direction is "out"
 */
#define mupnp_upnp_argument_isoutdirection(arg) mupnp_streq(mupnp_upnp_argument_getdirection(arg), CG_UPNP_ARGUMENT_OUT_DIR)

/**** relatedStateVariable ****/

/**
 * Set related state variable
 *
 * @param arg Argument
 * @param value Related state variable name
 */
#define mupnp_upnp_argument_setrelatedstatevariable(arg, value) mupnp_xml_node_setchildnode(mupnp_upnp_argument_getargumentnode(arg), CG_UPNP_ARGUMENT_RELATED_STATE_VARIABLE, value)

/**
 * Get related state variable
 *
 * @param arg Argument
 *
 * @return String representing the related state variable
 */
#define mupnp_upnp_argument_getrelatedstatevariable(arg) mupnp_xml_node_getchildnodevalue(mupnp_upnp_argument_getargumentnode(arg), CG_UPNP_ARGUMENT_RELATED_STATE_VARIABLE)

/****************************************
* Function (Argument - Execution Data)
****************************************/

/**** value ****/

/**
 * Set argument value
 *
 * @param arg Argument
 * @param data Argument value
 */
#define mupnp_upnp_argument_setvalue(arg,data) mupnp_string_setvalue(arg->value, data)

/**
 * Get argument value
 *
 * @param arg Argument
 *
 * @return Argument value
 */
#define mupnp_upnp_argument_getvalue(arg) mupnp_string_getvalue(arg->value)

/**
 * Set argument value
 *
 * @param arg Argument
 * @param data Argument value
 */
#define mupnp_upnp_argument_setintvalue(arg,data) mupnp_string_setintvalue(arg->value, data)

/**
 * Get argument value
 *
 * @param arg Argument
 *
 * @return Argument value
 */
#define mupnp_upnp_argument_getintvalue(arg) mupnp_string_getintvalue(arg->value)

/**
 * Set argument value
 *
 * @param arg Argument
 * @param data Argument value
 */
#define mupnp_upnp_argument_setfloatvalue(arg,data) mupnp_string_setfloatvalue(arg->value, data)

/**
 * Get argument value
 *
 * @param arg Argument
 *
 * @return Argument value
 */
#define mupnp_upnp_argument_getfloatvalue(arg) mupnp_string_getfloatvalue(arg->value)

/**
 * Set argument value
 *
 * @param arg Argument
 * @param data Argument value
 */
#define mupnp_upnp_argument_setdoublevalue(arg,data) mupnp_string_setdoublevalue(arg->value, data)

/**
 * Get argument value
 *
 * @param arg Argument
 *
 * @return Argument value
 */
#define mupnp_upnp_argument_getdoublevalue(arg) mupnp_string_getdoublevalue(arg->value)

/****************************************
* Function (ArgumentList)
****************************************/

/**
 * Create new argument list
 */
CgUpnpArgumentList *mupnp_upnp_argumentlist_new();

/**
 * Delete argument list
 *
 * @param argumentList Argument list
 */
void mupnp_upnp_argumentlist_delete(CgUpnpArgumentList *argumentList);

/**
 * Clear argument list
 *
 * @param argList Argument list
 */
#define mupnp_upnp_argumentlist_clear(argList) mupnp_list_clear((CgList *)argList, (CG_LIST_DESTRUCTORFUNC)mupnp_upnp_argument_delete)

/**
 * Get argument list size
 *
 * @param argList Argument list
 */
#define mupnp_upnp_argumentlist_size(argList) mupnp_list_size((CgList *)argList)

/**
 * Get next argument from argument list
 *
 * @param argList Argument list
 */
#define mupnp_upnp_argumentlist_gets(argList) (CgUpnpArgument *)mupnp_list_next((CgList *)argList)

/**
 * Add argument into argument list
 *
 * @param argList Argument list
 * @param arg Argument
 */
#define mupnp_upnp_argumentlist_add(argList, arg) mupnp_list_add((CgList *)argList, (CgList *)arg)

/**
 * Get argument from argument list based on argument name
 *
 * @param argumentList argumentList
 * @param name Argument name
 *
 * @return Argument
 */
CgUpnpArgument *mupnp_upnp_argumentlist_get(CgUpnpArgumentList *argumentList, const char *name);

/**
 * Set argument values by using source argument list. If there is an argument with the 
 * name in argument list and source argument list argument data is set from source
 * argument list to the matching argument list.
 *
 * @param argumentList Argument list
 * @param srcArgumentList Source argument list
 */
void mupnp_upnp_argumentlist_set(CgUpnpArgumentList *argumentList, CgUpnpArgumentList *srcArgumentList);

#ifdef  __cplusplus
}
#endif

#endif
