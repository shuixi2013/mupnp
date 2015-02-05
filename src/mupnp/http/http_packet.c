/******************************************************************
*
*	CyberNet for C
*
*	Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006-2007 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*	File: chttp_packet.cpp
*
*	Revision:
*
*	01/25/05
*		- first revision
*	08/21/05
*		- Thanks for Theo Beisch <theo.beisch@gmx.de>
*		- Changed mupnp_http_packet_read() to store length in HTTPPacket.
*	10/25/05
*		- The following functions move to define macros.
*		  mupnp_http_packet_setcontent()
*		  mupnp_http_packet_setcontentpointer()
*		  mupnp_http_packet_getcontent()
*		- mupnp_http_packet_setncontent() is added.
*	10/31/05
*		- mupnp_http_packet_sethost:
*		  port was an excess parameter for s(n)printf when port <= 0
*	11/11/05
*		- Added mupnp_http_packet_setheaderlonglong() and mupnp_http_packet_getheaderlonglong().
*		- Extended mupnp_http_packet_setcontentlength() and mupnp_http_packet_getcontentlength() to 64bit
*		  when the compiler is supported C99 or the platform is WIN32.
*	02/01/07
*		- Fixed mupnp_http_request_post() not to hung up when the request method is HEAD.
*		- Added a onlyHeader parameter to mupnp_http_response_read() and mupnp_http_response_packet().
*	03/18/07
*		- Changed the following functions to use CgInt64.
*		  mupnp_http_packet_setheaderlonglong()
*		  mupnp_http_packet_getheaderlonglong()
*	11/16\07  Satoshi Konno <skonno@cybergarage.org>
*		- Fixed mupnp_http_packet_read_body()not to lost data when the response packet is huge.
*	12/13/07  Aapo makela <aapo.makela@nokia.com>
*		- Fix to mupnp_http_packet_sethost() not to crash in out-of-memory situation
*
******************************************************************/

#include <mupnp/http/http.h>
#include <mupnp/util/log.h>

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <mupnp/net/interface.h>

#include <limits.h>

#define READBUF_LENGTH 1024

/****************************************
* mupnp_http_packet_new
****************************************/

CgHttpPacket *mupnp_http_packet_new()
{
	CgHttpPacket *httpPkt;

	mupnp_log_debug_l4("Entering...\n");

	httpPkt= (CgHttpPacket *)malloc(sizeof(CgHttpPacket));

	if ( NULL != httpPkt )
	{
		mupnp_http_packet_init(httpPkt);
	}

	mupnp_log_debug_l4("Leaving...\n");

	return httpPkt;
}

/****************************************
* mupnp_http_packet_delete
****************************************/

void mupnp_http_packet_delete(CgHttpPacket *httpPkt)
{
	mupnp_log_debug_l4("Entering...\n");

	mupnp_http_packet_clean(httpPkt);
	free(httpPkt);

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_init
****************************************/

void mupnp_http_packet_init(CgHttpPacket *httpPkt)
{
	mupnp_log_debug_l4("Entering...\n");

	httpPkt->headerList = mupnp_http_headerlist_new();
	httpPkt->content = mupnp_string_new();

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_clean
****************************************/

void mupnp_http_packet_clean(CgHttpPacket *httpPkt)
{
	mupnp_log_debug_l4("Entering...\n");

	mupnp_http_headerlist_delete(httpPkt->headerList);
	mupnp_string_delete(httpPkt->content);

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_clear
****************************************/

void mupnp_http_packet_clear(CgHttpPacket *httpPkt)
{
	mupnp_log_debug_l4("Entering...\n");

	mupnp_http_headerlist_clear(httpPkt->headerList);
	mupnp_string_setvalue(httpPkt->content, NULL);

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_setheader
****************************************/

void mupnp_http_packet_setheadervalue(CgHttpPacket *httpPkt, const char* name, const char *value)
{
	mupnp_log_debug_l4("Entering...\n");

	mupnp_http_headerlist_set(httpPkt->headerList, name, value);

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_setheaderinteger
****************************************/

void mupnp_http_packet_setheaderinteger(CgHttpPacket *httpPkt, const char* name, int value)
{
	char svalue[CG_STRING_INTEGER_BUFLEN];

	mupnp_log_debug_l4("Entering...\n");

	mupnp_http_packet_setheadervalue(httpPkt, name, mupnp_int2str(value, svalue, sizeof(svalue)));

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_setheaderlong
****************************************/

void mupnp_http_packet_setheaderlong(CgHttpPacket *httpPkt, const char* name, long value)
{
	char svalue[CG_STRING_LONG_BUFLEN];

	mupnp_log_debug_l4("Entering...\n");

	mupnp_http_packet_setheadervalue(httpPkt, name, mupnp_long2str(value, svalue, sizeof(svalue)));

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
 * mupnp_http_packet_setheadersizet
 ****************************************/

void mupnp_http_packet_setheadersizet(CgHttpPacket *httpPkt, const char* name, size_t value)
{
	char svalue[CG_STRING_LONG_BUFLEN];
  
	mupnp_log_debug_l4("Entering...\n");
  
	mupnp_http_packet_setheadervalue(httpPkt, name, mupnp_sizet2str(value, svalue, sizeof(svalue)));
  
	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
 * mupnp_http_packet_setheaderssizet
 ****************************************/

void mupnp_http_packet_setheaderssizet(CgHttpPacket *httpPkt, const char* name, ssize_t value)
{
	char svalue[CG_STRING_LONG_BUFLEN];
  
	mupnp_log_debug_l4("Entering...\n");
  
	mupnp_http_packet_setheadervalue(httpPkt, name, mupnp_ssizet2str(value, svalue, sizeof(svalue)));
  
	mupnp_log_debug_l4("Leaving...\n");
}


/****************************************
* mupnp_http_packet_getheadervalue
****************************************/

const char *mupnp_http_packet_getheadervalue(CgHttpPacket *httpPkt, const char* name)
{
	return mupnp_http_headerlist_getvalue(httpPkt->headerList, name);
}

/****************************************
* mupnp_http_packet_getheadervalue
****************************************/

int mupnp_http_packet_getheaderinteger(CgHttpPacket *httpPkt, const char* name)
{
	const char *value;

	mupnp_log_debug_l4("Entering...\n");

	value = mupnp_http_packet_getheadervalue(httpPkt, name); 

	mupnp_log_debug_l4("Leaving...\n");

	return (value != NULL) ? atoi(value) : 0;
}

/****************************************
* mupnp_http_packet_getheadervalue
****************************************/

long mupnp_http_packet_getheaderlong(CgHttpPacket *httpPkt, const char* name)
{
	const char *value;

	mupnp_log_debug_l4("Entering...\n");

	value = mupnp_http_packet_getheadervalue(httpPkt, name); 

	mupnp_log_debug_l4("Leaving...\n");

	return (value != NULL) ? atol(value) : 0;
}

/****************************************
 * mupnp_http_packet_getheadersizet
 ****************************************/

size_t mupnp_http_packet_getheadersizet(CgHttpPacket *httpPkt, const char* name)
{
	const char *value;
  
	mupnp_log_debug_l4("Entering...\n");
  
	value = mupnp_http_packet_getheadervalue(httpPkt, name);
  
	mupnp_log_debug_l4("Leaving...\n");
  
	return (value != NULL) ? atol(value) : 0;
}

/****************************************
 * mupnp_http_packet_getheadersizet
 ****************************************/

ssize_t mupnp_http_packet_getheaderssizet(CgHttpPacket *httpPkt, const char* name)
{
	const char *value;
  
	mupnp_log_debug_l4("Entering...\n");
  
	value = mupnp_http_packet_getheadervalue(httpPkt, name);
  
	mupnp_log_debug_l4("Leaving...\n");
  
	return (value != NULL) ? atol(value) : 0;
}

/****************************************
* mupnp_http_packet_getheadervalue
****************************************/

void mupnp_http_packet_sethost(CgHttpPacket *httpPkt, const char *addr, int port)
{
	char *host;
	size_t hostMaxLen;

	mupnp_log_debug_l4("Entering...\n");
	
	if (addr == NULL)
		return;

	hostMaxLen = mupnp_strlen(addr) + CG_NET_IPV6_ADDRSTRING_MAXSIZE + CG_STRING_INTEGER_BUFLEN;
	host = malloc(sizeof(char) * hostMaxLen);

	if (host == NULL)
		/* Memory allocation failure */
		return;

#if defined(HAVE_SNPRINTF)
	if (0 < port && port != CG_HTTP_DEFAULT_PORT) {
		if (mupnp_net_isipv6address(addr) == TRUE)
			snprintf(host, hostMaxLen, "[%s]:%d", addr, port);
		else
			snprintf(host, hostMaxLen, "%s:%d", addr, port);
	}
	else {
		if (mupnp_net_isipv6address(addr) == TRUE)
			snprintf(host, hostMaxLen, "[%s]", addr);
		else
			snprintf(host, hostMaxLen, "%s", addr);
	}
#else	
	if (0 < port && port != CG_HTTP_DEFAULT_PORT) {
		if (mupnp_net_isipv6address(addr) == TRUE)
			sprintf(host, "[%s]:%d", addr, port);
		else
			sprintf(host, "%s:%d", addr, port);
	}
	else {
		if (mupnp_net_isipv6address(addr) == TRUE)
			sprintf(host, "[%s]", addr);
		else
			sprintf(host, "%s", addr);
	}
#endif

	mupnp_http_packet_setheadervalue(httpPkt, CG_HTTP_HOST, host);

	free(host);

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_post
****************************************/

void mupnp_http_packet_post(CgHttpPacket *httpPkt, CgSocket *sock)
{
	CgHttpHeader *header;
	const char *name, *value;
	char *content;
	size_t contentLen;
	
	mupnp_log_debug_l4("Entering...\n");

	/**** send headers ****/
	for (header = mupnp_http_packet_getheaders(httpPkt); header != NULL; header = mupnp_http_header_next(header)) {
		name = mupnp_http_header_getname(header);
		if (name == NULL)
			continue;
		mupnp_socket_write(sock, name, mupnp_strlen(name));
		mupnp_socket_write(sock, CG_HTTP_COLON, sizeof(CG_HTTP_COLON)-1);
		mupnp_socket_write(sock, CG_HTTP_SP, sizeof(CG_HTTP_SP)-1);
		value = mupnp_http_header_getvalue(header);
		if (value != NULL)
			mupnp_socket_write(sock, value, mupnp_strlen(value));
		mupnp_socket_write(sock, CG_HTTP_CRLF, sizeof(CG_HTTP_CRLF)-1);
	}
	mupnp_socket_write(sock, CG_HTTP_CRLF, sizeof(CG_HTTP_CRLF)-1);
	
	/**** send content ****/
	content = mupnp_http_packet_getcontent(httpPkt);
	contentLen = mupnp_http_packet_getcontentlength(httpPkt);
	if (content != NULL && 0 < contentLen)
		mupnp_socket_write(sock, content, contentLen);

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_read_headers
****************************************/

void mupnp_http_packet_read_headers(CgHttpPacket *httpPkt, CgSocket *sock, char *lineBuf, size_t lineBufSize)
{
	CgStringTokenizer *strTok;
	CgHttpHeader *header;
	ssize_t readLen;
	char *name, *value;
	
	mupnp_log_debug_l4("Entering...\n");

	while (1) {
		readLen = mupnp_socket_readline(sock, lineBuf, lineBufSize);
		if (readLen <= 2)
			break;
		name = NULL;
		value = NULL;
		strTok = mupnp_string_tokenizer_new(lineBuf, CG_HTTP_HEADERLINE_DELIM);
		if (mupnp_string_tokenizer_hasmoretoken(strTok) == TRUE)
			name = mupnp_string_tokenizer_nexttoken(strTok);
		if (mupnp_string_tokenizer_hasmoretoken(strTok) == TRUE) {
			value = mupnp_string_tokenizer_nextalltoken(strTok);
			mupnp_strrtrim(value, CG_HTTP_HEADERLINE_DELIM, mupnp_strlen(CG_HTTP_HEADERLINE_DELIM));
		}
		if (0 < mupnp_strlen(name)) {
			if (mupnp_strlen(value) == 0)
				value = "";
			header = mupnp_http_header_new();
			mupnp_http_header_setname(header, name);
			mupnp_http_header_setvalue(header, value);
			mupnp_http_packet_addheader(httpPkt, header);
		}
		mupnp_string_tokenizer_delete(strTok);
	}

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_read_chunk
****************************************/

size_t mupnp_http_packet_read_chunk(CgHttpPacket *httpPkt, CgSocket *sock, char *lineBuf, size_t lineBufSize)
{
	ssize_t readLen = 0;
	ssize_t conLen = 0;
	int tries = 0;
	char *content = NULL;
	
	mupnp_log_debug_l4("Entering...\n");

	/* Read chunk header */
	readLen = mupnp_socket_readline(sock, lineBuf, lineBufSize);
	
	conLen = mupnp_strhex2long(lineBuf);
	if (conLen < 1) return 0;
	
	content = (char *)malloc(conLen+1);

	if (content == NULL)
	{
		mupnp_log_debug_s("Memory allocation problem!\n");
		return 0;
	}
		
	content[conLen] = 0;
	
	readLen = 0;
	/* Read content until conLen is reached, or tired of trying */
	while (readLen < conLen && tries < 20)
	{
		readLen += mupnp_socket_read(sock, (content+readLen), (conLen-readLen));
		tries++;
	}
	
	/* Append content to packet */
	mupnp_http_packet_appendncontent(httpPkt, content, readLen);
	free(content); content = NULL;
	
	if (readLen == conLen)
	{
		/* Read CRLF bytes */
		mupnp_socket_readline(sock, lineBuf, lineBufSize);
	}
	
	mupnp_log_debug_l4("Leaving...\n");

	return readLen;
}

/****************************************
* mupnp_http_packet_read_body
****************************************/

BOOL mupnp_http_packet_read_body(CgHttpPacket *httpPkt, CgSocket *sock, char *lineBuf, size_t lineBufSize)
{
	ssize_t readLen;
	ssize_t conLen;
	char *content;
	char readBuf[READBUF_LENGTH + 1];
	int tries = 0;

	mupnp_log_debug_l4("Entering...\n");

	conLen = mupnp_http_packet_getcontentlength(httpPkt);
	content = NULL;
	if (0 < conLen) {
		content = (char *)malloc(conLen+1);
		if (content == NULL)
		{
			mupnp_log_debug_s("Memory allocation problem!\n");
			return FALSE;
		}
		
		content[0] = '\0';
		readLen = 0;
		
		/* Read content until conLen is reached, or tired of trying */
		while (readLen < conLen && tries < 20)
		{
			readLen += mupnp_socket_read(sock, (content+readLen), (conLen-readLen));
			/* Fixed to increment the counter only when mupnp_socket_read() doesn't read data */
			if (readLen <= 0)
				tries++;
		}
		
		if (readLen <= 0)
			return TRUE;
		content[readLen] = '\0';
		mupnp_http_packet_setcontentpointer(httpPkt, content, readLen);
	}
	else if (mupnp_http_packet_getheadervalue(httpPkt, 
					CG_HTTP_CONTENT_LENGTH) == NULL)
	{
		/* header existance must be checked! otherwise packets which
		   rightly report 0 as content length, will jam the http */
		
		/* Check if we read chunked encoding */
		if (mupnp_http_packet_ischunked(httpPkt) == TRUE)
		{
			conLen = 0;
			do {
				readLen = mupnp_http_packet_read_chunk(httpPkt, sock, lineBuf, lineBufSize);
				conLen += readLen;
			} while (readLen > 0);
			
			mupnp_http_packet_setcontentlength(httpPkt,conLen);
		} else {
			readLen = 0;
			conLen = 0;
			while ((readLen = mupnp_socket_read(sock, readBuf, READBUF_LENGTH)) > 0)
			{
				mupnp_http_packet_appendncontent(httpPkt, readBuf, readLen);
				conLen += readLen;
			}

			mupnp_http_packet_setcontentlength(httpPkt, conLen);
		}
	}

	mupnp_log_debug_l4("Leaving...\n");

	return TRUE;
}

/****************************************
* mupnp_http_packet_read
****************************************/

BOOL mupnp_http_packet_read(CgHttpPacket *httpPkt, CgSocket *sock, BOOL onlyHeader, char *lineBuf, size_t lineBufSize)
{
	mupnp_log_debug_l4("Entering...\n");

	mupnp_http_packet_clear(httpPkt);
	mupnp_http_packet_read_headers(httpPkt, sock, lineBuf, lineBufSize);
	
	mupnp_log_debug_l4("Leaving...\n");

	if (onlyHeader)
		return TRUE;

	return mupnp_http_packet_read_body(httpPkt, sock, lineBuf, lineBufSize);
}

/****************************************
* mupnp_http_packet_getheadersize
****************************************/

size_t mupnp_http_packet_getheadersize(CgHttpPacket *httpPkt)
{
	CgHttpHeader *header;
	size_t headerSize;
	const char *name;
	const char *value;
	
	mupnp_log_debug_l4("Entering...\n");

	headerSize = 0;
	for (header = mupnp_http_packet_getheaders(httpPkt); header != NULL; header = mupnp_http_header_next(header)) {
		name = mupnp_http_header_getname(header);
		value = mupnp_http_header_getvalue(header);
		headerSize += mupnp_strlen(name); 
		headerSize += sizeof(CG_HTTP_COLON)-1; 
		headerSize += sizeof(CG_HTTP_SP)-1; 
		headerSize += mupnp_strlen(value); 
		headerSize += sizeof(CG_HTTP_CRLF)-1; 
	}
	headerSize += sizeof(CG_HTTP_CRLF)-1; 

	mupnp_log_debug_l4("Leaving...\n");

	return headerSize;
}

/****************************************
* mupnp_http_packet_copy
****************************************/

void mupnp_http_packet_copy(CgHttpPacket *destHttpPkt, CgHttpPacket *srcHttpPkt)
{
	CgHttpHeader *srcHeader;
	CgHttpHeader *destHeader;
	
	mupnp_log_debug_l4("Entering...\n");

	mupnp_http_packet_clear(destHttpPkt);
	
	/**** copy headers ****/
	for (srcHeader = mupnp_http_packet_getheaders(srcHttpPkt); srcHeader != NULL; srcHeader = mupnp_http_header_next(srcHeader)) {
		destHeader = mupnp_http_header_new();
		mupnp_http_header_setname(destHeader, mupnp_http_header_getname(srcHeader));
		mupnp_http_header_setvalue(destHeader, mupnp_http_header_getvalue(srcHeader));
		mupnp_http_packet_addheader(destHttpPkt, destHeader);
	}
	
	/**** copy content ****/
	mupnp_http_packet_setcontent(destHttpPkt, mupnp_http_packet_getcontent(srcHttpPkt));

	mupnp_log_debug_l4("Leaving...\n");
}

/****************************************
* mupnp_http_packet_print
****************************************/

void mupnp_http_packet_print(CgHttpPacket *httpPkt)
{
	CgHttpHeader *header;
	char *content;
	long contentLen;

	mupnp_log_debug_l4("Entering...\n");

	/**** print headers ****/
	for (header = mupnp_http_packet_getheaders(httpPkt); header != NULL; header = mupnp_http_header_next(header)) {
	mupnp_log_debug_s("%s: %s\n",
			mupnp_http_header_getname(header),
			mupnp_http_header_getvalue(header));
	}
	mupnp_log_debug_s("\n");
	
	/**** print content ****/
	content = mupnp_http_packet_getcontent(httpPkt);
	contentLen = mupnp_http_packet_getcontentlength(httpPkt);
	
	if (content != NULL && 0 < contentLen)
		mupnp_log_debug_s("%s\n", content);

	mupnp_log_debug_l4("Leaving...\n");
}
