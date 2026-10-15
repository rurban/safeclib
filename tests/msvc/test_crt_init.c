/*------------------------------------------------------------------
 * test_crt_init.c - MSVC debug CRT setup for non-interactive test runs
 *
 * The debug CRT reports failed assertions (heap checks, _VALIDATE_RETURN, ...)
 * and invalid parameters by popping up a modal dialog, which hangs forever on
 * CI. Redirect those reports to stderr instead and make invalid parameters
 * return EINVAL like the release CRT does, so that a failure is visible in the
 * test log, and not a ctest timeout.
 *
 * Linked into every test executable with MSVC. Runs before main() via the
 * CRT initializer table.
 *------------------------------------------------------------------
 */
#ifdef _MSC_VER

#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

static void __cdecl test_invalid_parameter(const wchar_t *expr,
                                           const wchar_t *func,
                                           const wchar_t *file,
                                           unsigned int line,
                                           uintptr_t reserved) {
    (void)expr;
    (void)func;
    (void)file;
    (void)line;
    (void)reserved;
}

static void __cdecl test_crt_init(void) {
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX |
                 SEM_NOOPENFILEERRORBOX);
    _set_invalid_parameter_handler(test_invalid_parameter);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
}

#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) static void(__cdecl *test_crt_init_p)(void) =
    test_crt_init;

#endif /* _MSC_VER */
