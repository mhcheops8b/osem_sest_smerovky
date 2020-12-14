CC = gcc
CFLAGS = -Wall -O2 --pedantic --ansi -I ../include
#CFLAGS = -Wall -O2 --pedantic --ansi -I ../include -DOSM_DEBUG
#EXE_EXT = 
EXE_EXT = .exe
UU_N_OBJECTS = bmp1dulm.o bmp2dulm.o cesty.o charpath.o chrslovo.o  \
	find.o findsyl.o flosm.o flosmsyl.o lustikrz.o num_path.o obrfile.o   \
	osm_ries.o osm_size.o osmtop.o pismeno.o position.o q_part.o q_sect.o \
	search.o slovo.o smery.o specchar.o str_path.o syltop.o table.o       \
	uu_n.o wrdlist.o
SS_N_OBJECTS = ss_n.o

all: uu_n$(EXE_EXT) ss_n$(EXE_EXT)

$(UU_N_OBJECTS) $(SS_N_OBJECTS): %.o: %.c 
	$(CC) -c $(CFLAGS) $< -o $@ 
uu_n$(EXE_EXT): $(UU_N_OBJECTS)
	$(CC) -o uu_n$(EXE_EXT) $(CFLAGS) $(UU_N_OBJECTS)
ss_n$(EXE_EXT): $(SS_N_OBJECTS)
	$(CC) -o ss_n$(EXE_EXT) $(CFLAGS) $(SS_N_OBJECTS)
clean:
	@rm -f $(UU_N_OBJECTS) uu_n$(EXE_EXT) $(SS_N_OBJECTS) ss_n$(EXE_EXT) *~
