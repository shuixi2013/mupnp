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
*	File: csocket.cpp
*
*	Revision:
*
*	01/17/05
*		- first revision
*	10/26/05
*		- Added changes when the parametes are null.
*
*	03/19/06 Theo Beisch
*		- Added NULL check for CG_LIST_DESTRUCTORFUNC
******************************************************************/

#include <mupnp/util/list.h>
#include <mupnp/util/log.h>

/****************************************
* mupnp_list_header_init
****************************************/

void mupnp_list_header_init(CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == list)
		return;

	list->headFlag = TRUE;			
	list->prev = list->next = list;

	mupnp_log_debug_l5("Leaving...\n");
}

/****************************************
* mupnp_list_node_init
****************************************/

void mupnp_list_node_init(CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == list)
		return;

	list->headFlag = FALSE;			
	list->prev = list->next = list;

	mupnp_log_debug_l5("Leaving...\n");
}

/****************************************
* mupnp_list_size
****************************************/

int mupnp_list_size(CgList *headList)
{
	CgList *list;
	int listCnt;
	
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == headList)
		return 0;

	listCnt = 0;
	for (list = mupnp_list_next(headList); list != NULL; list = mupnp_list_next(list))
		listCnt++;
	
	mupnp_log_debug_l5("Leaving...\n");
	
	return listCnt;
}

/****************************************
* mupnp_list_get
****************************************/

CgList *mupnp_list_get(CgList *headList, int index)
{
	CgList *list;
	int n;
	
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == headList)
		return NULL;

	list = mupnp_list_next(headList);
	for (n=0; n<index; n++) {
		if (NULL == list)
			break;
		list = mupnp_list_next(list);
	}
		
	mupnp_log_debug_l5("Leaving...\n");
	
	return list;
}

/****************************************
* mupnp_list_insert
****************************************/

void mupnp_list_insert(
CgList *prevList,
CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if ((NULL == prevList) || (NULL == list))
		return;

	list->prev = prevList;
	list->next = prevList->next;
	prevList->next->prev = list;
	prevList->next = list;

	mupnp_log_debug_l5("Leaving...\n");
}

/****************************************
* mupnp_list_add
****************************************/

void mupnp_list_add(
CgList *headList,
CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if ((NULL == headList) || (NULL == list))
		return;

	if (NULL == headList->prev)
		return;
	
	mupnp_list_insert(headList->prev, list);

	mupnp_log_debug_l5("Leaving...\n");
}

/****************************************
* mupnp_list_remove
****************************************/

void mupnp_list_remove(CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == list)
		return;

	if ((NULL == list->prev) || (NULL == list->next))
		return;
	
	list->prev->next = list->next;
	list->next->prev = list->prev;
	list->prev = list->next = list;

	mupnp_log_debug_l5("Leaving...\n");
}

/****************************************
* mupnp_list_prev_circular
****************************************/

CgList *mupnp_list_prev_circular (
CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == list)
		return NULL;

	if (NULL == list->prev)
		return NULL;
	
	if (list->prev->headFlag)
		list = list->prev;

	mupnp_log_debug_l5("Leaving...\n");

	return list->prev;
}

/****************************************
* mupnp_list_prev
****************************************/

CgList *mupnp_list_prev(
CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == list)
		return NULL;

	if (NULL == list->prev)
		return NULL;
	
	if (list->prev->headFlag == TRUE)
		return NULL;
	
	mupnp_log_debug_l5("Leaving...\n");

	return list->prev;
}

/****************************************
* mupnp_list_next_circular
****************************************/

CgList *mupnp_list_next_circular(
CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == list)
		return NULL;

	if (NULL == list->next)
		return NULL;
	
	if (list->next->headFlag == TRUE)
		list = list->next;

	mupnp_log_debug_l5("Leaving...\n");
	
	return list->next;
}

/****************************************
* mupnp_list_next
****************************************/

CgList *mupnp_list_next(
CgList *list)
{
	mupnp_log_debug_l5("Entering...\n");

	if (NULL == list)
		return NULL;

	if (NULL == list->next)
		return NULL;
	
	if (list->next->headFlag == TRUE)
		return NULL;

	mupnp_log_debug_l5("Leaving...\n");

	return list->next;
}

/****************************************
* mupnp_list_clear
****************************************/

void mupnp_list_clear(CgList *headList, CG_LIST_DESTRUCTORFUNC destructorFunc)
{
	CgList *list;

	mupnp_log_debug_l5("Entering...\n");

	if (NULL == headList)
		return;

	list = mupnp_list_next(headList);
	while(list != NULL) {
		mupnp_list_remove(list);
		//Theo Beisch: use destructorFunc or just free(listElement)
		if (destructorFunc != NULL){
			destructorFunc(list);
		} else {
			free(list);
		}
		list = mupnp_list_next(headList);
	}

	/*** list header must be deleted by user ***/
	/* free(headList); */

	mupnp_log_debug_l5("Leaving...\n");
}
