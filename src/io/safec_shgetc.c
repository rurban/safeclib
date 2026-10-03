/*------------------------------------------------------------------
 * safec_shgetc.c
 *
 * Copyright © 2005-2014 Rich Felker, et al. (musl libc, MIT licensed)
 * Copyright (c) 2026 by Reini Urban
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

/* Ported from musl's src/internal/shgetc.c + src/stdio/__toread.c +
 * src/stdio/__uflow.c (MIT licensed), adapted to _SAFEC_FILE: instead
 * of a fixed set of real-descriptor vs. pseudo-string FILE ops, every
 * _SAFEC_FILE source (real stream or in-memory string) refills through
 * its own sf->read() callback (safec_stream_read / safec_string_read),
 * so __toread()/__uflow() need no F_NORD/mode bookkeeping at all. */

#ifdef FOR_DOXYGEN
#include "safe_lib.h"
#else
#include "safeclib_private.h"
#include "io/safec_file.h"
#endif

/* Pull exactly one more string chunk's worth of bytes: src/cookie is
 * the remaining NUL-terminated input, buf is the caller's output
 * destination (len bytes), and as a side effect sf->rpos/rend are
 * populated so that subsequent shgetc() calls can take the fast
 * `*rpos++' path without calling back into read() for every byte. */
size_t safec_string_read(_SAFEC_FILE *sf, unsigned char *buf, size_t len) {
    char *src = (char *)sf->cookie;
    size_t k = len + 256;
    char *end = (char *)memchr((void *)src, 0, k);
    if (end)
        k = end - src;
    if (k < len)
        len = k;
    memcpy(buf, src, len);
    sf->rpos = (unsigned char *)(src + len);
    sf->rend = (unsigned char *)(src + k);
    sf->cookie = src + k;
    return len;
}

#ifndef SAFECLIB_DISABLE_IO
/* Same contract, but for a real stdio stream: fread() a chunk into
 * sf->buf (the caller-owned scratch buffer), return up to len bytes
 * of it via `buf', and leave the rest buffered in rpos/rend for the
 * shgetc() fast path. */
size_t safec_stream_read(_SAFEC_FILE *sf, unsigned char *buf, size_t len) {
    size_t n;

    if (!sf->f || !sf->buf || !sf->buf_size) {
        sf->rpos = sf->rend = sf->buf;
        return 0;
    }
    n = fread(sf->buf, 1, sf->buf_size, sf->f);
    if (!n) {
        sf->rpos = sf->rend = sf->buf;
        return 0;
    }
    if (len > n)
        len = n;
    memcpy(buf, sf->buf, len);
    sf->rpos = sf->buf + len;
    sf->rend = sf->buf + n;
    return len;
}
#endif

void safec_toread(_SAFEC_FILE *sf) {
    sf->rpos = sf->rend = sf->buf + sf->buf_size;
}

static int safec_uflow(_SAFEC_FILE *sf) {
    unsigned char c;
    safec_toread(sf);
    if (sf->read && sf->read(sf, &c, 1) == 1)
        return c;
    return EOF;
}

void safec_shlim(_SAFEC_FILE *sf, safec_off_t lim) {
    sf->shlim = lim;
    sf->shcnt = sf->buf - sf->rpos;
    /* If lim is nonzero, rend must be a valid pointer. */
    if (lim && sf->rend - sf->rpos > lim)
        sf->shend = sf->rpos + lim;
    else
        sf->shend = sf->rend;
}

int safec_shgetc(_SAFEC_FILE *sf) {
    int c;
    safec_off_t cnt = shcnt(sf);
    if ((sf->shlim && cnt >= sf->shlim) || (c = safec_uflow(sf)) < 0) {
        sf->shcnt = sf->buf - sf->rpos + cnt;
        sf->shend = sf->rpos;
        sf->shlim = -1;
        return EOF;
    }
    cnt++;
    if (sf->shlim && sf->rend - sf->rpos > sf->shlim - cnt)
        sf->shend = sf->rpos + (sf->shlim - cnt);
    else
        sf->shend = sf->rend;
    sf->shcnt = sf->buf - sf->rpos + cnt;
    if (sf->rpos <= sf->buf)
        sf->rpos[-1] = (unsigned char)c;
    return c;
}
