/******************************************************************************
 *	Copyright 2008-2013 by Aerospike.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy 
 *	of this software and associated documentation files (the "Software"), to 
 *	deal in the Software without restriction, including without limitation the 
 *	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 *	sell copies of the Software, and to permit persons to whom the Software is 
 *	furnished to do so, subject to the following conditions:
 * 
 *	The above copyright notice and this permission notice shall be included in 
 *	all copies or substantial portions of the Software.
 * 
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *	IN THE SOFTWARE.
 *****************************************************************************/

#pragma once

#include <stdlib.h>

#include <aerospike/as_val.h>
#include <aerospike/as_util.h>

/******************************************************************************
 *	MACROS
 *****************************************************************************/

#define AS_STREAM_END ((void *) 0)

/******************************************************************************
 *	TYPES
 *****************************************************************************/

struct as_stream_hooks_s;

/**
 *	Stream Status Codes
 */
typedef enum as_stream_status_e {
	AS_STREAM_OK	= 0,
	AS_STREAM_ERR	= 1
} as_stream_status;

/**
 *	Stream Interface
 *
 *	To use the stream interface, you will need to create an instance 
 *	via one of the implementations.
 *
 *	@ingroup aerospike_t
 */
typedef struct as_stream_s {

	/**
	 *	Specifies whether the free() can be used 
	 *	on this stream.
	 */
	bool free;

	/**
	 *	Context data for the stream.
	 */
	void * data;

	/**
	 *	Hooks for the stream
	 */
	const struct as_stream_hooks_s * hooks;

} as_stream;

/**
 *	Stream Hooks
 *
 *	An implementation of `as_rec` should provide implementations for each
 *	of the hooks.
 */
typedef struct as_stream_hooks_s {

	/**
	 *	Destroy the stream.
	 */
	int (* destroy)(as_stream * stream);

	/**
	 *	Read the next value from the stream.
	 */
	as_val * (* read)(const as_stream * stream);

	/**
	 *	Write a value to the stream.
	 */
	as_stream_status (* write)(const as_stream * stream, as_val * value);
	
} as_stream_hooks;

/******************************************************************************
 *	INSTANCE FUNCTIONS
 *****************************************************************************/

/**
 *	Initializes a stack allocated as_stream for a given source and hooks.
 *
 *	@param stream	The stream to initialize.
 *	@param data		The source feeding the stream
 *	@param hooks	The hooks that interface with the source
 *
 *	@return On success, the initialized stream. Otherwise NULL.
 *
 *	@relatesalso as_stream
 */
inline as_stream * as_stream_init(as_stream * stream, void * data, const as_stream_hooks * hooks) 
{
	if ( !stream ) return stream;

	stream->free = false;
	stream->data = data;
	stream->hooks = hooks;
	return stream;
}

/**
 *	Creates a new heap allocated as_stream for a given source and hooks.
 *
 *	@param data		The source feeding the stream
 *	@param hooks	The hooks that interface with the source
 *
 *	@return On success, a new stream. Otherwise NULL.
 *
 *	@relatesalso as_stream
 */
inline as_stream * as_stream_new(void * data, const as_stream_hooks * hooks)
{
	as_stream * stream = (as_stream *) malloc(sizeof(as_stream));
	if ( !stream ) return stream;

	stream->free = true;
	stream->data = data;
	stream->hooks = hooks;
	return stream;
}

/**
 *	Destroy the as_stream and associated resources.
 *
 *	@param stream 	The stream to destroy.
 *
 *	@return 0 on success, otherwise 1.
 *
 *	@relatesalso as_stream
 */
inline void as_stream_destroy(as_stream * stream)
{
	as_util_hook(destroy, 1, stream);
	if ( stream && stream->free ) {
		free(stream);
	}
}

/******************************************************************************
 *	VALUE FUNCTIONS
 *****************************************************************************/

/**
 *	Get the source for the stream
 *
 *	@param stream 	The stream to get the source from
 *
 *	@return pointer to the source of the stream
 *
 *	@relatesalso as_stream
 */
inline void * as_stream_source(const as_stream * stream)
{
	return (stream ? stream->data : NULL);
}

/**
 *	Reads a value from the stream
 *
 *	@param stream 	The stream to be read.
 *
 *	@return the element read from the stream or STREAM_END
 *
 *	@relatesalso as_stream
 */
inline as_val * as_stream_read(const as_stream * stream)
{
	return as_util_hook(read, NULL, stream);
}

/**
 *	Is the stream readable? Tests whether the stream has a read function.
 *
 *	@param stream 	The stream to test.
 *
 *	@return true if the stream can be read from
 *
 *	@relatesalso as_stream
 */
inline bool as_stream_readable(const as_stream * stream)
{
	return stream != NULL && stream->hooks != NULL && stream->hooks->read;
}

/**
 *	Write a value to the stream
 *
 *	@param stream	The stream to write to.
 *	@param value	The element to write to the stream.
 *
 *	@return AS_STREAM_OK on success, otherwise is failure.
 *
 *	@relatesalso as_stream
 */
inline as_stream_status as_stream_write(const as_stream * stream, as_val * value)
{
	return as_util_hook(write, AS_STREAM_ERR, stream, value);
}


/**
 *	Is the stream writable? Tests whether the stream has a write function.
 *
 *	@param stream 	The stream to test.
 *
 *	@return true if the stream can be written to.
 *
 *	@relatesalso as_stream
 */
inline bool as_stream_writable(const as_stream * stream)
{
	return stream != NULL && stream->hooks != NULL && stream->hooks->write;
}
