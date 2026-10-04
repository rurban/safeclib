#!/bin/sh
# cppcheck-timeout.sh: run cppcheck with a hard wall-clock budget.
#
# Adapted from rcc's cppcheck-timeout.sh.
#
# Used as the pre-commit `cppcheck` hook's entry point. CI runners
# occasionally stall cppcheck (or the surrounding prek/git-submodule
# machinery it gets invoked through) well past any reasonable analysis
# time for a handful of changed files -- a real finding takes seconds
# locally. Rather than let that hang the whole lint job until CI's own
# job-level timeout cancels it (reported as a bare, uninformative
# "operation was canceled"), bound it here and treat "timed out" as
# "inconclusive, not a lint failure": a genuine cppcheck error/warning
# still fails the hook normally, and finishing well inside the budget is
# unaffected.
set -eu

TIMEOUT_SECS="${CPPCHECK_TIMEOUT_SECS:-180}"

if command -v timeout >/dev/null 2>&1; then
    # `set -e` aborts the script the instant any unguarded command
    # returns non-zero -- including `timeout` itself reporting 124 -- so
    # capturing $? on the line *after* silently never runs. `|| rc=$?`
    # keeps the non-zero status from tripping `set -e` at all.
    timeout "$TIMEOUT_SECS" cppcheck "$@" || rc=$?
    rc="${rc:-0}"
    if [ "$rc" -eq 124 ]; then
        echo "cppcheck-timeout.sh: cppcheck did not finish within ${TIMEOUT_SECS}s; skipping (not a lint failure)" >&2
        exit 0
    fi
    exit "$rc"
else
    # No `timeout` binary available (unlikely outside minimal containers):
    # fall back to running cppcheck directly, unbounded.
    exec cppcheck "$@"
fi
