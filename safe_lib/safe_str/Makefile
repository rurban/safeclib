#
#
#
SRC =	safe_str_constraint.c \
	strcasecmp_s.c \
	strcasestr_s.c \
	strcat_s.c \
	strcmp_s.c \
	strcmpfld_s.c \
	strcpy_s.c \
	strcpyfld_s.c \
	strcpyfldin_s.c \
	strcpyfldout_s.c \
	strcspn_s.c \
	strfirstchar_s.c \
	strfirstdiff_s.c \
	strfirstsame_s.c \
        strisalphanumeric_s.c \
        strisascii_s.c \
        strisdigit_s.c \
        strishex_s.c \
        strislowercase_s.c \
        strismixedcase_s.c \
        strispassword_s.c \
        strisuppercase_s.c \
	strlastchar_s.c \
	strlastdiff_s.c \
	strlastsame_s.c \
	strljustify_s.c \
	strncat_s.c \
	strncpy_s.c \
	strnlen_s.c \
	strnterminate_s.c \
	strpbrk_s.c \
	strprefix_s.c \
	strremovews_s.c \
	strspn_s.c \
	strstr_s.c \
	strtok_s.c \
        strtolowercase_s.c \
        strtouppercase_s.c \
	strzero_s.c


OBJ = $(SRC:.c=.o)

OUT = ../lib/safe_str.a

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

