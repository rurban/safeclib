#
#
#
SRC =	safe_mem_constraint.c \
	mem_primitives_lib.c \
	memset_s.c \
	memset16_s.c \
	memset32_s.c \
	memcpy_s.c \
	memcpy16_s.c \
	memcpy32_s.c \
	memzero_s.c \
	memzero16_s.c \
	memzero32_s.c \
	memmove_s.c \
	memmove16_s.c \
	memmove32_s.c \
	memcmp_s.c \
	memcmp16_s.c \
	memcmp32_s.c


OBJ = $(SRC:.c=.o)

OUT = ../lib/safe_mem.a


INCLUDE = -I. -I../include/ -I../../safe_base/include

CCFLAGS = -g
CCC = gcc

LIBS = -L../ 

LDFLAGS = -g
.SUFFIXES: .c


.c.o:
	$(CCC) $(INCLUDE) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT) Makefile.bak 

# DO NOT DELETE

