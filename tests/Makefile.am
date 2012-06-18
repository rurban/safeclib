## Process this file with automake to produce Makefile.in

SAFEC_BUILT_TESTS = \
	t_memcpy32_s          \
	t_memcpy16_s          \
	t_memcpy_s            \
	t_memcmp32_s          \
	t_memcmp16_s          \
	t_memcmp_s            \
	t_memmove32_s         \
	t_memmove16_s         \
	t_memmove_s           \
	t_memset32_s          \
	t_memset16_s          \
	t_memset_s            \
	t_memzero32_s         \
	t_memzero16_s         \
	t_memzero_s           \
	t_strcasecmp_s        \
	t_strcasestr_s        \
	t_strcat_s            \
	t_strcmpfld_s         \
	t_strcmp_s            \
	t_strcpyfldin_s       \
	t_strcpyfldout_s      \
	t_strcpyfld_s         \
	t_strcpy_s            \
	t_strcspn_s           \
	t_strfirstchar_s      \
	t_strfirstdiff_s      \
	t_strfirstsame_s      \
	t_strisalphanumeric_s \
	t_strisascii_s        \
	t_strisdigit_s        \
	t_strishex_s          \
	t_strislowercase_s    \
	t_strismixed_s        \
	t_strispassword_s     \
	t_strisuppercase_s    \
	t_strlastchar_s       \
	t_strlastdiff_s       \
	t_strlastsame_s       \
	t_strljustify_s       \
	t_strncat_s           \
	t_strncpy_s           \
	t_strnlen_s           \
	t_strnterminate_s     \
	t_strpbrk_s           \
	t_strprefix_s         \
	t_strremovews_s       \
	t_strspn_s            \
	t_strstr_s            \
	t_strtok_s            \
	t_strtolowercase_s    \
	t_strtouppercase_s    \
	t_strzero_s


SAFEC_BUILT_PERFS = p_memcpy32_s p_memcpy_s

TESTS = $(SAFEC_BUILT_TESTS)
check_PROGRAMS = $(SAFEC_BUILT_TESTS)
noinst_PROGRAMS = $(SAFEC_BUILT_PERFS)

p_memcpy32_s_SOURCES          = perf_memcpy32_s.c
p_memcpy32_s_LDADD            = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

p_memcpy_s_SOURCES            = perf_memcpy_s.c
p_memcpy_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memcpy32_s_SOURCES          = test_memcpy32_s.c
t_memcpy32_s_LDADD            = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memcpy16_s_SOURCES          = test_memcpy16_s.c
t_memcpy16_s_LDADD            = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memcpy_s_SOURCES            = test_memcpy_s.c
t_memcpy_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memcmp32_s_SOURCES          = test_memcmp32_s.c
t_memcmp32_s_LDADD            = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memcmp16_s_SOURCES          = test_memcmp16_s.c
t_memcmp16_s_LDADD            = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memcmp_s_SOURCES            = test_memcmp_s.c
t_memcmp_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memmove32_s_SOURCES         = test_memmove32_s.c
t_memmove32_s_LDADD           = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memmove16_s_SOURCES         = test_memmove16_s.c
t_memmove16_s_LDADD           = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memmove_s_SOURCES           = test_memmove_s.c
t_memmove_s_LDADD             = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memset32_s_SOURCES          = test_memset32_s.c
t_memset32_s_LDADD            = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memset16_s_SOURCES          = test_memset16_s.c
t_memset16_s_LDADD            = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memset_s_SOURCES            = test_memset_s.c
t_memset_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memzero32_s_SOURCES         = test_memzero32_s.c
t_memzero32_s_LDADD           = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memzero16_s_SOURCES         = test_memzero16_s.c
t_memzero16_s_LDADD           = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_memzero_s_SOURCES           = test_memzero_s.c
t_memzero_s_LDADD             = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcasecmp_s_SOURCES        = test_strcasecmp_s.c
t_strcasecmp_s_LDADD          = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcasestr_s_SOURCES        = test_strcasestr_s.c
t_strcasestr_s_LDADD          = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcat_s_SOURCES            = test_strcat_s.c
t_strcat_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcmpfld_s_SOURCES         = test_strcmpfld_s.c
t_strcmpfld_s_LDADD           = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcmp_s_SOURCES            = test_strcmp_s.c
t_strcmp_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcpyfldin_s_SOURCES       = test_strcpyfldin_s.c
t_strcpyfldin_s_LDADD         = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcpyfldout_s_SOURCES      = test_strcpyfldout_s.c
t_strcpyfldout_s_LDADD        = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcpyfld_s_SOURCES         = test_strcpyfld_s.c
t_strcpyfld_s_LDADD           = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcpy_s_SOURCES            = test_strcpy_s.c
t_strcpy_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strcspn_s_SOURCES           = test_strcspn_s.c
t_strcspn_s_LDADD             = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strfirstchar_s_SOURCES      = test_strfirstchar_s.c
t_strfirstchar_s_LDADD        = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strfirstdiff_s_SOURCES      = test_strfirstdiff_s.c
t_strfirstdiff_s_LDADD        = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strfirstsame_s_SOURCES      = test_strfirstsame_s.c
t_strfirstsame_s_LDADD        = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strisalphanumeric_s_SOURCES = test_strisalphanumeric_s.c
t_strisalphanumeric_s_LDADD   = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strisascii_s_SOURCES        = test_strisascii_s.c
t_strisascii_s_LDADD          = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strisdigit_s_SOURCES        = test_strisdigit_s.c
t_strisdigit_s_LDADD          = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strishex_s_SOURCES          = test_strishex_s.c
t_strishex_s_LDADD            = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strislowercase_s_SOURCES    = test_strislowercase_s.c
t_strislowercase_s_LDADD      = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strismixed_s_SOURCES        = test_strismixed_s.c
t_strismixed_s_LDADD          = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strispassword_s_SOURCES     = test_strispassword_s.c
t_strispassword_s_LDADD       = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strisuppercase_s_SOURCES    = test_strisuppercase_s.c
t_strisuppercase_s_LDADD      = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strlastchar_s_SOURCES       = test_strlastchar_s.c
t_strlastchar_s_LDADD         = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strlastdiff_s_SOURCES       = test_strlastdiff_s.c
t_strlastdiff_s_LDADD         = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strlastsame_s_SOURCES       = test_strlastsame_s.c
t_strlastsame_s_LDADD         = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strljustify_s_SOURCES       = test_strljustify_s.c
t_strljustify_s_LDADD         = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strncat_s_SOURCES           = test_strncat_s.c
t_strncat_s_LDADD             = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strncpy_s_SOURCES           = test_strncpy_s.c
t_strncpy_s_LDADD             = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strnlen_s_SOURCES           = test_strnlen_s.c
t_strnlen_s_LDADD             = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strnterminate_s_SOURCES     = test_strnterminate_s.c
t_strnterminate_s_LDADD       = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strpbrk_s_SOURCES           = test_strpbrk_s.c
t_strpbrk_s_LDADD             = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strprefix_s_SOURCES         = test_strprefix_s.c
t_strprefix_s_LDADD           = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strremovews_s_SOURCES       = test_strremovews_s.c
t_strremovews_s_LDADD         = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strspn_s_SOURCES            = test_strspn_s.c
t_strspn_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strstr_s_SOURCES            = test_strstr_s.c
t_strstr_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strtok_s_SOURCES            = test_strtok_s.c
t_strtok_s_LDADD              = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strtolowercase_s_SOURCES    = test_strtolowercase_s.c
t_strtolowercase_s_LDADD      = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strtouppercase_s_SOURCES    = test_strtouppercase_s.c
t_strtouppercase_s_LDADD      = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

t_strzero_s_SOURCES           = test_strzero_s.c
t_strzero_s_LDADD             = $(top_builddir)/src/libsafec-@SAFEC_API_VERSION@.la

AM_CPPFLAGS = -I$(top_builddir)/src -I$(top_srcdir)/src
CLEANFILES  = *~
