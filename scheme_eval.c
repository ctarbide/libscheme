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

#include "scheme.h"

/* locals */
static Scheme_Object *eval(int argc, Scheme_Object *argv[]);

void
scheme_init_eval(Scheme_Env *env)
{
	scheme_add_global("eval", scheme_make_prim(eval), env);
}

Scheme_Object *
scheme_eval(Scheme_Object *obj, Scheme_Env *env)
{
	Scheme_Object *type;

	for (;;) {
		type = SCHEME_TYPE(obj);

		if (type == scheme_symbol_type) {
			Scheme_Object *val;
			val = scheme_lookup_value(obj, env);

			if (! val) {
				scheme_signal_error("reference to unbound symbol: %s", SCHEME_STR_VAL(obj));
			}

			return val;
		} else if (type == scheme_pair_type) {
			Scheme_Object *rator, *rands;
			Scheme_Object *evaled_rands[SCHEME_MAX_ARGS];
			Scheme_Object *fun;
			int num_rands, i;
			rator = scheme_eval(SCHEME_CAR(obj), env);
			type = SCHEME_TYPE(rator);

			if (type == scheme_syntax_type) {
				return SCHEME_SYNTAX(rator)(obj, env);
			} else if (type == scheme_macro_type) {
				fun = (Scheme_Object *) SCHEME_PTR_VAL(rator);
				rands = SCHEME_CDR(obj);
				obj = scheme_apply_to_list(fun, rands);
				continue;
			} else {
				rands = SCHEME_CDR(obj);
				num_rands = scheme_list_length(rands);
				i = 0;

				while (rands != scheme_null) {
					evaled_rands[i] = scheme_eval(SCHEME_CAR(rands), env);
					i++;
					rands = SCHEME_CDR(rands);
				}

				return scheme_apply(rator, num_rands, evaled_rands);
			}
		} else {
			return obj;
		}
	}
}

static Scheme_Object *
eval(int argc, Scheme_Object *argv[])
{
	if (argc == 1) {
		scheme_signal_error("eval: wrong number of args");
		return NULL; /* never happens */
	}

	return scheme_eval(argv[0], scheme_env);
}
