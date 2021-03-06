/*
 * Traceshark - a visualizer for visualizing ftrace and perf traces
 * Copyright (C) 2015, 2016  Viktor Rosendahl <viktor.rosendahl@gmail.com>
 *
 * This file is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 *
 *  a) This program is free software; you can redistribute it and/or
 *     modify it under the terms of the GNU General Public License as
 *     published by the Free Software Foundation; either version 2 of the
 *     License, or (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public
 *     License along with this library; if not, write to the Free
 *     Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *     MA 02110-1301 USA
 *
 * Alternatively,
 *
 *  b) Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *     1. Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *     2. Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *     NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *     CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TSTRING_H
#define TSTRING_H

#include <cstring>

class TString {
public:
	char *ptr;
	unsigned int len;
	static __always_inline int cmp(const TString *a, const TString *b);
	static __always_inline int strcmp(const TString *a, const TString *b);
	static __always_inline int strcmp(const TString *a, const TString *b,
					  unsigned short skip,
					  unsigned short *neq);
	__always_inline bool merge(const TString *s, unsigned int maxlen);
	__always_inline bool set(const TString *s, unsigned int maxlen);
};

__always_inline int TString::cmp(const TString *a, const TString *b) {
	unsigned int clen;
	int rval;
	int diff;

	diff = a->len - b->len;
	clen = diff < 0 ? a->len : b->len;
	rval = memcmp(a->ptr, b->ptr, clen);
	if (rval == 0)
		return diff;
	else
		return rval;
}

__always_inline int TString::strcmp(const TString *a, const TString *b,
				    unsigned short skip, unsigned short *eqn)
{
	int rval = (int) a->len - (int)  b->len;
	int cval;
	int imax = rval < 0 ? a->len : b->len;
	int i;

	*eqn = skip;
	for (i = skip; i < imax; i++) {
		cval = a->ptr[i] - b->ptr[i];
		if (cval == 0) {
			(*eqn)++;
			continue;
		} else
			return cval;
	}
	return rval;
}

/* This seems to be the fastest now, at least for stringpool */
__always_inline int TString::strcmp(const TString *a, const TString *b)
{
	int rval = (int) a->len - (int)  b->len;
	int cval;
	int imax = rval < 0 ? a->len : b->len;
	int i;

	for (i = 0; i < imax; i++) {
		cval = a->ptr[i] - b->ptr[i];
		if (cval == 0)
			continue;
		else
			return cval;
	}
	return rval;
}

__always_inline bool TString::merge(const TString *s, unsigned int maxlen)
{
	unsigned int newlen;

	newlen = len + 1 + s->len;
	if (newlen > maxlen)
		return false;
	ptr[len] = ' ';
	strncpy(ptr + len + 1, s->ptr, s->len);
	len = newlen;
	ptr[len] = '\0';
	return true;
}

__always_inline bool TString::set(const TString *s, unsigned int maxlen)
{
	if (s->len > maxlen)
		return false;
	strncpy(ptr, s->ptr, s->len);
	len = s->len;
	ptr[len] = '\0';
	return true;
}

#endif
