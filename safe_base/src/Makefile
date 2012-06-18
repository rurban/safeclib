

SRC =	runtime_environment.c

OBJ = $(SRC:.c=.o)

INCLUDE = -I. \
          -I../include/

LIBS = -L../lib

OUT = ../lib/safe_base.a

CCFLAGS = -g
CCC = gcc
LDFLAGS = -g
.SUFFIXES: .c


.c.o:
	$(CCC) $(INCLUDE) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT) 

# DO NOT DELETE

