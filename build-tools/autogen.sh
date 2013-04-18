#!/bin/sh

# This script is used to automatically recreate the build environment
# in a sane and repeatable way. It is really easy, but just so no one
# forgets...
#
# WARNING! You DO NOT want to run this when you have uncommitted changes!!

require_clean_work_tree () {
	git rev-parse --verify HEAD >/dev/null || exit 1

	# Update the index
	git update-index -q --ignore-submodules --refresh
	err=0

	# Disallow unstaged changes in the working tree
	if ! git diff-files --quiet --ignore-submodules
	then
		echo >&2 "$1: You have unstaged changes."
		err=1
	fi

	# Disallow uncommitted changes in the index
	if ! git diff-index --cached --quiet --ignore-submodules HEAD --
	then
		if test $err = 0
		then
			echo >&2 "$1: Your index contains uncommitted changes."
		else
			echo >&2 "Additionally, your index contains uncommitted changes."
		fi
		err=1
	fi

	if test $err = 1
	then
		test -n "$2" && echo >&2 "$2"
		exit 1
	fi
}

if test -z "$1" || test "$1" != "-f"; then
	require_clean_work_tree "WARNING" "Please commit them or use the '-f' option"
fi

git clean -dx --force --quiet && \
autoreconf -Wall --install
