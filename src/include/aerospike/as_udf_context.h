/******************************************************************************
 * Copyright 2008-2013 by Aerospike.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************/
#pragma once

#include <aerospike/as_memtracker.h>
#include <aerospike/as_udf_context.h>
#include <aerospike/as_timer.h>
#include <aerospike/as_aerospike.h>

/******************************************************************************
 * TYPES
 ******************************************************************************/

typedef struct as_udf_context_s as_udf_context;
typedef struct as_udf_context_hook_s as_udf_context_hooks;

struct as_udf_context_s {
	as_aerospike  * as;
	as_memtracker * memtracker;
	as_timer      * timer; 
	const         as_udf_context_hooks * hooks;
};

struct as_udf_context_hook_s {
	as_aerospike  * (* get_aerospike)(const as_udf_context *);
	as_memtracker * (* get_memtracker)(const as_udf_context *);
	as_timer      * (* get_timer)(const as_udf_context *);
};

extern as_aerospike * as_udf_context_get_aerospike(const as_udf_context *ctx); 
extern as_memtracker * as_udf_context_get_memtracker(const as_udf_context *ctx); 
extern as_timer * as_udf_context_get_timer(const as_udf_context *ctx);
