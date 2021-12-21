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
#include <string.h>

Scheme_Hash_Table *
scheme_hash_table(size_t size)
{
	Scheme_Hash_Table *table;
	table = (Scheme_Hash_Table *) scheme_malloc(sizeof(Scheme_Hash_Table));
	table->size = size;
	table->buckets = (Scheme_Bucket **) scheme_calloc((size_t)size, sizeof(Scheme_Bucket *));
	return (table);
}

void
scheme_add_to_table(Scheme_Hash_Table *table, char *key, void *val)
{
	size_t h, i;
	Scheme_Bucket *bucket;
	h = i = 0;

	while (key[i]) {
		h += (h << 5) + h + (size_t)key[i++];
	}

	h = h % table->size;
	bucket = (Scheme_Bucket *) scheme_malloc(sizeof(Scheme_Bucket));
	bucket->key = scheme_strdup(key);
	bucket->val = val;
	bucket->next = table->buckets[h];
	table->buckets[h] = bucket;
}

void *
scheme_lookup_in_table(Scheme_Hash_Table *table, char *key)
{
	size_t h;
	char *str;
	Scheme_Bucket *bucket;
	h = 0;
	str = key;

	while (*str) {
		h += (h << 5) + h + (size_t) * str++;
	}

	h = h % table->size;
	bucket = table->buckets[h];

	while (bucket) {
		if (strcmp(key, bucket->key) == 0) {
			return (bucket->val);
		} else {
			bucket = bucket->next;
		}
	}

	return (NULL);
}

void
scheme_change_in_table(Scheme_Hash_Table *table, char *key, void *new)
{
	size_t h, i;
	Scheme_Bucket *bucket;
	h = i = 0;

	while (key[i]) {
		h += (h << 5) + h + (size_t)key[i++];
	}

	h = h % table->size;
	bucket = table->buckets[h];

	while (bucket) {
		if (strcmp(key, bucket->key) == 0) {
			bucket->val = new;
			return;
		}

		bucket = bucket->next;
	}
}

#if 0
static unsigned int
scheme_hash(char *key)
{
	size_t h;
	h = 0;

	while (*key) {
		h += (h << 5) + h + (size_t) * key++;
	}

	return (unsigned int)(h & UINT_MAX);
}
#endif
