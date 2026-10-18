/*------------------------------------------------------------------
 * test_memory_barrier
 * Files 'mem/memset_s.c' and 'extmem/memzero_s.c'
 *
 * Verifies that memset_s and memzero_s execute MEMORY_BARRIER after
 * writing, and not on constraint violations where nothing is written.
 * With --enable-cache-flush it also verifies the MEMORY_FLUSH of exactly
 * the written range before the barrier, and none without the option.
 *
 * The two sources are compiled into this test with TEST_MEMORY_BARRIER
 * defined (see mem/mem_primitives_lib.h), which counts every executed
 * MEMORY_BARRIER and MEMORY_FLUSH (and still runs the real ones). The
 * entry points are renamed to not clash with the installed library.
 *------------------------------------------------------------------
 */

#define TEST_MEMORY_BARRIER
#define _memset_s_chk tb_memset_s_chk
#define _memset_s_uchk tb_memset_s_uchk
#define _memzero_s_chk tb_memzero_s_chk
#define _memzero_s_uchk tb_memzero_s_uchk

#include "test_private.h"
#include "safeclib_private.h"
#include "mem/mem_primitives_lib.h"


#undef debug_printf
#define debug_printf(...) fprintf(stderr, __VA_ARGS__)
static unsigned long barrier_count;
void test_memory_barrier(void) { barrier_count++; }

static unsigned long flush_count;
static const void *flush_ptr;
static size_t flush_len;
void test_memory_flush(const void *p, size_t len) {
    flush_count++;
    flush_ptr = p;
    flush_len = len;
}

#include "mem/memset_s.c"
#include "extmem/memzero_s.c"

#define LEN 64

#ifdef ENABLE_CACHE_FLUSH
#define FLUSHES(n) (n)
#else
#define FLUSHES(n) 0
#endif

static int errs;

/* barriers and flushes expected after a call; flushes only with the option */
#define EXPECT_BARRIERS(call, rc_exp, n_exp)                                   \
    do {                                                                       \
        unsigned long before = barrier_count, fbefore = flush_count;           \
        errno_t r = (call);                                                    \
        if (r != (rc_exp) || barrier_count - before != (n_exp) ||              \
            flush_count - fbefore != FLUSHES(n_exp)) {                         \
            debug_printf("%s:%d %s rc=%d (want %d) barriers=%lu (want %d) "    \
                         "flushes=%lu (want %d)\n",                            \
                         __FILE__, __LINE__, #call, (int)r, (int)(rc_exp),     \
                         barrier_count - before, (int)(n_exp),                 \
                         flush_count - fbefore, (int)FLUSHES(n_exp));          \
            errs++;                                                            \
        }                                                                      \
    } while (0)

static int all_eq(const uint8_t *p, size_t n, uint8_t v) {
    size_t i;
    for (i = 0; i < n; i++)
        if (p[i] != v)
            return 0;
    return 1;
}

int main(void) {
    uint8_t mem[LEN];
    /* volatile indirection: keep these non-constant to the compiler so
       clang's diagnose_if (BOS_CHK) does not flag the deliberate
       constraint violations below as compile-time errors; they must
       still be caught at runtime by the _chk functions. */
    void *volatile bad_dest = NULL;
    volatile rsize_t big_n = LEN;
    volatile rsize_t zero_dmax = 0;

    /* memset_s: barrier after the write */
    memset(mem, 0, LEN);
    EXPECT_BARRIERS(tb_memset_s_chk(mem, LEN, 0x5A, LEN, BOS_UNKNOWN), EOK, 1);
    if (!all_eq(mem, LEN, 0x5A)) {
        debug_printf("%d memset_s_chk did not set\n", __LINE__);
        errs++;
    }
    memset(mem, 0, LEN);
    EXPECT_BARRIERS(tb_memset_s_uchk(mem, 0xA5, LEN), EOK, 1);
    if (!all_eq(mem, LEN, 0xA5)) {
        debug_printf("%d memset_s_uchk did not set\n", __LINE__);
        errs++;
    }

    /* n > dmax: dmax bytes are still written, so the barrier must run */
    memset(mem, 0, LEN);
    EXPECT_BARRIERS(tb_memset_s_chk(mem, LEN / 2, 0x33, big_n, BOS_UNKNOWN),
                    ESNOSPC, 1);
#ifdef ENABLE_CACHE_FLUSH
    /* the flushed range is exactly the written range */
    if (flush_ptr != mem || flush_len != LEN / 2) {
        debug_printf("%d flushed %p/%zu, want %p/%d\n", __LINE__, flush_ptr,
                     flush_len, (void *)mem, LEN / 2);
        errs++;
    }
#endif

    /* constraint violation before any write: no barrier */
    EXPECT_BARRIERS(tb_memset_s_chk(bad_dest, LEN, 0x5A, LEN, BOS_UNKNOWN), ESNULLP,
                    0);

    /* memzero_s */
    memset(mem, 0x5A, LEN);
    EXPECT_BARRIERS(tb_memzero_s_chk(mem, LEN, BOS_UNKNOWN), EOK, 1);
    if (!all_eq(mem, LEN, 0)) {
        debug_printf("%d memzero_s_chk did not zero\n", __LINE__);
        errs++;
    }
    memset(mem, 0x5A, LEN);
    EXPECT_BARRIERS(tb_memzero_s_uchk(mem, LEN), EOK, 1);
    if (!all_eq(mem, LEN, 0)) {
        debug_printf("%d memzero_s_uchk did not zero\n", __LINE__);
        errs++;
    }
    EXPECT_BARRIERS(tb_memzero_s_chk(bad_dest, LEN, BOS_UNKNOWN), ESNULLP, 0);
    EXPECT_BARRIERS(tb_memzero_s_chk(mem, zero_dmax, BOS_UNKNOWN), ESZEROL, 0);

    return errs;
}
