# OSS Scanner build for safeclib.
#
# Two out-of-tree builds from the same checkout in /src:
#   /src/build       gcc, -O2 -g. Reference build; run the upstream suite (make -C /src/build check).
#   /src/build-asan  clang, AddressSanitizer + UndefinedBehaviorSanitizer. Use it to confirm
#                    memory-safety findings with small C reproducers linked against
#                    /src/build-asan/src/.libs/libsafec-*.a / .so.
#
FROM ubuntu:26.04

ENV DEBIAN_FRONTEND=noninteractive \
    LANG=C.UTF-8

RUN apt-get update && apt-get install -y --no-install-recommends \
      ca-certificates git build-essential clang libclang-rt-dev llvm gdb \
      autoconf automake libtool pkg-config perl libtext-diff-perl file \
    && rm -rf /var/lib/apt/lists/*

# scanner contract: the checkout lives inside the image, at /src
COPY . /src
WORKDIR /src

# Regenerate the autotools files (-f: skip the clean-worktree check; the image copy is disposable).
RUN sh build-aux/autogen.sh -f

# 1) Reference build.
RUN mkdir /src/build && cd /src/build \
    && ../configure --enable-debug CFLAGS="-O2 -g -fno-omit-frame-pointer" \
    && make -j"$(nproc)"

# 2) Sanitizer build.
RUN mkdir /src/build-asan && cd /src/build-asan \
    && CC=clang ../configure --enable-debug --disable-hardening \
         CFLAGS="-O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=undefined" \
         LDFLAGS="-fsanitize=address,undefined" \
    && make -j"$(nproc)"

# Sanitizer defaults for anyone running the ASan binaries in this image.
ENV ASAN_OPTIONS=detect_leaks=1:abort_on_error=1:symbolize=1 \
    UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1

# Run the upstream test suite on the reference build. Failures are reported but never abort the build.
RUN cd /src/build && (make -j"$(nproc)" check || echo "WARNING: safeclib tests reported failures")

# Run the upstream test suite on the sanitizer build too. Failures are reported but never abort the build.
RUN cd /src/build-asan && (make -j"$(nproc)" check || echo "WARNING: safeclib ASan tests reported failures")
