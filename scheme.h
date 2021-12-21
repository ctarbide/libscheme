/*
  libscheme
  Copyright (c) 1994 Brent Benson
  All rights reserved.

  Permission is hereby granted, without written agreement and without
  license or royalty fees, to use, copy, modify, and distribute this
  software and its documentation for any purpose, provided that the
  above copyright notice and the following two paragraphs appear in
  all copies of this software.

  IN NO EVENT SHALL BRENT BENSON BE LIABLE TO ANY PARTY FOR DIRECT,
  INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF BRENT
  BENSON HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  BRENT BENSON SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT
  NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER
  IS ON AN "AS IS" BASIS, AND BRENT BENSON HAS NO OBLIGATION TO
  PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
  MODIFICATIONS.
*/

#ifndef SCHEME_H
#define SCHEME_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#ifndef _POSIX_C_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <stdio.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include <sys/select.h>

#ifdef _MSC_VER
typedef  __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned  __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
/* assumes msinttypes is available
 */
#include <inttypes.h>
#if (_MSC_VER >= 1100 && _MSC_VER < 1900)
/* MSVC++  5.0, _MSC_VER == 1100, Visual Studio 5.0
 * MSVC++ 14.0, _MSC_VER == 1900, Visual Studio 2015
 * See also: https://github.com/MicrosoftDocs/cpp-docs/issues/1490
 */
#define PRIzu "I"
#else
#define PRIzu "zu"
#endif
#else
#include <inttypes.h>
#define PRIzu "zu"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct Scheme_Bucket {
	char *key;
	void *val;
	struct Scheme_Bucket *next;
};
typedef struct Scheme_Bucket Scheme_Bucket;

struct Scheme_Hash_Table {
	size_t size;
	Scheme_Bucket **buckets;
};
typedef struct Scheme_Hash_Table Scheme_Hash_Table;

struct Scheme_Env {
	int num_bindings;
	struct Scheme_Object **symbols;
	struct Scheme_Object **values;
	Scheme_Hash_Table *globals;
	struct Scheme_Env *next;
};
typedef struct Scheme_Env Scheme_Env;

struct Scheme_Cont {
	int escaped;
	jmp_buf buffer;
	struct Scheme_Object *retval;
};
typedef struct Scheme_Cont Scheme_Cont;

struct Scheme_Object {
	union {
		int char_val;
		int int_val;
		double double_val;
		char *string_val;
		void *ptr_val;
		struct Scheme_Cont *cont_val;
		struct {
			void *ptr1, *ptr2;
		} two_ptr_val;
		struct Scheme_Object *(*prim_val)(int argc, struct Scheme_Object *argv[]);
		struct Scheme_Object *(*syntax_val)(struct Scheme_Object *form, struct Scheme_Env *env);
		struct {
			struct Scheme_Object *car, *cdr;
		} pair_val;
		struct {
			int size;
			struct Scheme_Object **els;
		} vector_val;
		struct {
			struct Scheme_Env *env;
			struct Scheme_Object *code;
		} closure_val;
		struct {
			struct Scheme_Object *def;
			struct Scheme_Method *meths;
		} methods_val;
	} u;
	struct Scheme_Object *type;
};
typedef struct Scheme_Object Scheme_Object;

/* access macros */
#define SCHEME_TYPE(obj)     ((obj)->type)
#define SCHEME_CHAR_VAL(obj) ((obj)->u.char_val)
#define SCHEME_INT_VAL(obj)  ((obj)->u.int_val)
#define SCHEME_DBL_VAL(obj)  ((obj)->u.double_val)
#define SCHEME_STR_VAL(obj)  ((obj)->u.string_val)
#define SCHEME_PTR_VAL(obj)  ((obj)->u.ptr_val)
#define SCHEME_CONT_VAL(obj) ((obj)->u.cont_val)
#define SCHEME_PTR1_VAL(obj) ((obj)->u.two_ptr_val.ptr1)
#define SCHEME_PTR2_VAL(obj) ((obj)->u.two_ptr_val.ptr2)
#define SCHEME_SYNTAX(obj)   ((obj)->u.syntax_val)
#define SCHEME_PRIM(obj)     ((obj)->u.prim_val)
#define SCHEME_CAR(obj)      ((obj)->u.pair_val.car)
#define SCHEME_CDR(obj)      ((obj)->u.pair_val.cdr)
#define SCHEME_VEC_SIZE(obj) ((obj)->u.vector_val.size)
#define SCHEME_VEC_ELS(obj)  ((obj)->u.vector_val.els)
#define SCHEME_CLOS_ENV(obj) ((obj)->u.closure_val.env)
#define SCHEME_CLOS_CODE(obj)((obj)->u.closure_val.code)
#define SCHEME_METH_DEF(obj) ((obj)->u.methods_val.def)
#define SCHEME_METHS(obj)    ((obj)->u.methods_val.meths)

struct Scheme_Method {
	Scheme_Object *type;
	Scheme_Object *fun;
	struct Scheme_Method *next;
};
typedef struct Scheme_Method Scheme_Method;

typedef struct Scheme_Object *
(Scheme_Prim)(int argc, struct Scheme_Object *argv[]);

typedef struct Scheme_Object *
(Scheme_Syntax)(struct Scheme_Object *form, struct Scheme_Env *env);

/* error handling */
extern jmp_buf scheme_error_buf;
#define SCHEME_CATCH_ERROR(try_expr, err_expr) \
  (setjmp(scheme_error_buf) ? (err_expr) : (try_expr))
#define SCHEME_ASSERT(expr,msg) \
  ((expr) ? 0 : (scheme_signal_error(msg), 1))

/* types */
extern Scheme_Object *scheme_type_type;
extern Scheme_Object *scheme_char_type;
extern Scheme_Object *scheme_integer_type, *scheme_double_type;
extern Scheme_Object *scheme_string_type, *scheme_symbol_type;
extern Scheme_Object *scheme_null_type, *scheme_pair_type;
extern Scheme_Object *scheme_vector_type;
extern Scheme_Object *scheme_prim_type, *scheme_closure_type;
extern Scheme_Object *scheme_cont_type;
extern Scheme_Object *scheme_input_port_type, *scheme_output_port_type;
extern Scheme_Object *scheme_eof_type;
extern Scheme_Object *scheme_true_type, *scheme_false_type;
extern Scheme_Object *scheme_syntax_type, *scheme_macro_type;
extern Scheme_Object *scheme_promise_type, *scheme_struct_proc_type;

/* common symbols */
extern Scheme_Object *scheme_quote_symbol;
extern Scheme_Object *scheme_quasiquote_symbol;
extern Scheme_Object *scheme_unquote_symbol;
extern Scheme_Object *scheme_unquote_splicing_symbol;

/* constants */
extern Scheme_Object *scheme_eof;
extern Scheme_Object *scheme_null;
extern Scheme_Object *scheme_true;
extern Scheme_Object *scheme_false;

/* generic port support */

struct Scheme_Input_Port {
	Scheme_Object *sub_type;
	void *port_data;
	int (*getc_fun)(struct Scheme_Input_Port *port);
	void (*ungetc_fun)(int ch, struct Scheme_Input_Port *port);
	int (*char_ready_fun)(struct Scheme_Input_Port *port);
	void (*close_fun)(struct Scheme_Input_Port *port);
};
typedef struct Scheme_Input_Port Scheme_Input_Port;

struct Scheme_Output_Port {
	Scheme_Object *sub_type;
	void *port_data;
	void (*write_string_fun)(char *str, struct Scheme_Output_Port *);
	void (*close_fun)(struct Scheme_Output_Port *);
};
typedef struct Scheme_Output_Port Scheme_Output_Port;

extern Scheme_Object *scheme_stdin_port;
extern Scheme_Object *scheme_stdout_port;
extern Scheme_Object *scheme_stderr_port;

/* environment */
extern Scheme_Env *scheme_env;

/* convenience macros */
#define SCHEME_CHARP(obj)    (SCHEME_TYPE(obj) == scheme_char_type)
#define SCHEME_INTP(obj)     (SCHEME_TYPE(obj) == scheme_integer_type)
#define SCHEME_DBLP(obj)     (SCHEME_TYPE(obj) == scheme_double_type)
#define SCHEME_NUMBERP(obj)  (SCHEME_INTP(obj) || SCHEME_DBLP(obj))
#define SCHEME_STRINGP(obj)  (SCHEME_TYPE(obj) == scheme_string_type)
#define SCHEME_SYMBOLP(obj)  (SCHEME_TYPE(obj) == scheme_symbol_type)
#define SCHEME_BOOLP(obj)    ((obj == scheme_true) || (obj == scheme_false))
#define SCHEME_SYNTAXP(obj)  (SCHEME_TYPE(obj) == scheme_syntax_type)
#define SCHEME_PRIMP(obj)    (SCHEME_TYPE(obj) == scheme_prim_type)
#define SCHEME_CONTP(obj)    (SCHEME_TYPE(obj) == scheme_cont_type)
#define SCHEME_NULLP(obj)    (obj == scheme_null)
#define SCHEME_PAIRP(obj)    (SCHEME_TYPE(obj) == scheme_pair_type)
#define SCHEME_LISTP(obj)    (SCHEME_NULLP(obj) || SCHEME_PAIRP(obj))
#define SCHEME_VECTORP(obj)  (SCHEME_TYPE(obj) == scheme_vector_type)
#define SCHEME_CLOSUREP(obj) (SCHEME_TYPE(obj) == scheme_closure_type)
#define SCHEME_PROCP(obj)    (SCHEME_PRIMP(obj) || SCHEME_CLOSUREP(obj) || SCHEME_CONTP(obj))
#define SCHEME_INPORTP(obj)  (SCHEME_TYPE(obj) == scheme_input_port_type)
#define SCHEME_OUTPORTP(obj) (SCHEME_TYPE(obj) == scheme_output_port_type)
#define SCHEME_EOFP(obj)     (SCHEME_TYPE(obj) == scheme_eof_type)
#define SCHEME_PROMP(obj)    (SCHEME_TYPE(obj) == scheme_promise_type)
/* other */
#define SCHEME_CADR(obj)     (SCHEME_CAR (SCHEME_CDR (obj)))
#define SCHEME_CAAR(obj)     (SCHEME_CAR (SCHEME_CAR (obj)))
#define SCHEME_CDDR(obj)     (SCHEME_CDR (SCHEME_CDR (obj)))

/* constants */
#define SCHEME_MAX_ARGS 256	/* max number of args to function */

#include "scheme_prototypes.h"

#ifdef __cplusplus
}
#endif

#endif /* ! SCHEME_H */
