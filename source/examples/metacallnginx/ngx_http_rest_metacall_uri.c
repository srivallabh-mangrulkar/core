/*
 *	MetaCall NginX Module Wrapper by Parra Studios
 *	Copyright (C) 2016 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	A NginX module example as metacall HTTP (REST API) wrapper.
 *
 */

/* -- Headers -- */

#include "ngx_http_rest_metacall_uri.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <metacall/metacall.h>

/* -- Definitions -- */

#define METACALL_NGINX_URI_ARGS_SIZE 0x10
#define METACALL_NGINX_URI_FUNC_SIZE 0xFF

/* -- Private Method Declarations -- */

/*
int ngx_http_rest_metacall_uri_delimiters(ngx_http_request_t * req, size_t * start, size_t * length);

ngx_str_t * ngx_http_rest_metacall_uri_tokenize_initialize(size_t size);

void ngx_http_rest_metacall_uri_tokenize_clear(ngx_str_t * tokens[], size_t size);

size_t ngx_http_rest_metacall_uri_tokenize_count(ngx_http_request_t * req, size_t start);

int ngx_http_rest_metacall_uri_tokenize_impl(ngx_http_request_t * req, size_t start, ngx_str_t * tokens[], size_t size);

int ngx_http_rest_metacall_uri_tokenize(ngx_http_request_t * req, ngx_str_t * tokens[], size_t * size);

int ngx_http_rest_metacall_uri_parse(ngx_http_request_t * req, char * func_name[], void ** args[]);
*/

/* -- Methods -- */

int ngx_http_rest_metacall_uri_delimiters(ngx_http_request_t * req, size_t * start, size_t * length)
{
	size_t iterator;

	if (start == NULL || length == NULL)
	{
		return 1;
	}

	*start = *length = 0;

	/* Extract position of URI parameters */
	for (iterator = req->uri.len; req->unparsed_uri.data[iterator] != '/' &&
		iterator < req->unparsed_uri.len; ++iterator);

	/* Get URI parameters start */
	if ((iterator + 1) >= req->unparsed_uri.len)
	{
		/* Error */
		*start = req->unparsed_uri.len;

		return 1;
	}
	else
	{
		*start = iterator + 1;
	}

	*length = req->unparsed_uri.len - *start;

	return 0;
}

ngx_str_t * ngx_http_rest_metacall_uri_tokenize_initialize(size_t size)
{
	size_t iterator;

	/* Allocate tokens array */
	ngx_str_t * tokens = malloc(sizeof(ngx_str_t) * size);

	if (tokens == NULL)
	{
		return NULL;
	}

	for (iterator = 0; iterator < size; ++iterator)
	{
		tokens[iterator].data = NULL;
		tokens[iterator].len = 0;
	}

	return tokens;
}

void ngx_http_rest_metacall_uri_tokenize_clear(ngx_str_t * tokens[], size_t size)
{
	if (tokens == NULL)
	{
		return;
	}

	size_t iterator;

	/* Clear tokens array */
	for (iterator = 0; iterator < size; ++iterator)
	{
		if ((*tokens)[iterator].data != NULL)
		{
			free((*tokens)[iterator].data);
		}

		(*tokens)[iterator].len = 0;
	}

	free(*tokens);

	*tokens = NULL;
}

size_t ngx_http_rest_metacall_uri_tokenize_count(ngx_http_request_t * req, size_t start)
{
	size_t iterator, size = 0;

	for (iterator = start; iterator <= req->unparsed_uri.len; ++iterator)
	{
		if (req->unparsed_uri.data[iterator] == '/' || iterator == req->unparsed_uri.len)
		{
			++size;
		}
	}

	return size;
}

int ngx_http_rest_metacall_uri_tokenize_impl(ngx_http_request_t * req, size_t start, ngx_str_t * tokens[], size_t size)
{
	size_t iterator, token_count = 0, prev = start;

	for (iterator = start; iterator <= req->unparsed_uri.len; ++iterator)
	{
		if (req->unparsed_uri.data[iterator] == '/' || iterator == req->unparsed_uri.len)
		{
			size_t length = iterator - prev;

			(*tokens)[token_count].data = malloc(sizeof(u_char) * length);

			if ((*tokens)[token_count].data == NULL)
			{
				return 1;
			}

			(*tokens)[token_count].len = length;

			memcpy((*tokens)[token_count].data, &req->unparsed_uri.data[prev], length);

			(*tokens)[token_count].data[length] = '\0';

			prev = iterator + 1;

			++token_count;
		}
	}

	return 0;
}

int ngx_http_rest_metacall_uri_tokenize(ngx_http_request_t * req, ngx_str_t * tokens[], size_t * size)
{
	size_t start, length;

	*size = 0;

	/* Find URI delimiters */
	if (ngx_http_rest_metacall_uri_delimiters(req, &start, &length) != 0)
	{
		/* Error */
		return 1;
	}

	/* Find number of tokens in URI */
	if (length > 0)
	{
		size_t token_count = ngx_http_rest_metacall_uri_tokenize_count(req, start);

		if (token_count == 0)
		{
			return 1;
		}

		*size = token_count;
	}
	else
	{
		return 1;
	}

	/* Allocate tokens array */
	*tokens = ngx_http_rest_metacall_uri_tokenize_initialize(*size);

	if (*tokens == NULL)
	{
		return 1;
	}

	/* Split URI in tokens */
	if (ngx_http_rest_metacall_uri_tokenize_impl(req, start, tokens, *size) != 0)
	{
		/* Error */
		ngx_http_rest_metacall_uri_tokenize_clear(tokens, *size);

		return 1;
	}

	return 0;
}

int ngx_http_rest_metacall_uri_parse(ngx_http_request_t * req, char * func_name[], void ** args[])
{
	ngx_str_t * tokens = NULL;

	size_t size = 0;

	/* Tokenize URI */
	if (ngx_http_rest_metacall_uri_tokenize(req, (ngx_str_t **)&tokens, &size) == 0)
	{
		/* Copy function name */
		

		/* size_t func_size = 0; */

		/* Parse tokens into arguments */
		/*if (ngx_http_rest_metacall_uri_parse_impl(req, (ngx_str_t **)&tokens, size, func_name, &func_size, args) == 0)
		{
			if (

			return 0;
		}*/

		/* Clear tokens */
		ngx_http_rest_metacall_uri_tokenize_clear((ngx_str_t **)&tokens, size);

		return 0;
	}

	return 1;
}

int ngx_http_rest_metacall_uri(ngx_http_request_t * req, void ** result)
{
	char func_name[METACALL_NGINX_URI_FUNC_SIZE];

	void * args[METACALL_NGINX_URI_ARGS_SIZE];

	/* Parse URI into function name and array of arguments */
	if (ngx_http_rest_metacall_uri_parse(req, (char **)&func_name, (void ***)&args) != 0)
	{
		/* Error */
		*result = NULL;

		return 1;
	}

	/* Execute the call */
	*result = metacallv(func_name, args);

	return 0;
}
