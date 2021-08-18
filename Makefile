LN = cp
#LN = ln -s
EXE_EXT =
#EXE_EXT = .exe
all:
	@make -C src
	@rm -f uu_n$(EXE_EXT) ss_n$(EXE_EXT)
	@$(LN) src/uu_n$(EXE_EXT) uu_n$(EXE_EXT)
	@$(LN) src/ss_n$(EXE_EXT) ss_n$(EXE_EXT)
clean:
	@rm -f *~
	@make -C src clean
	@make -C include clean
	@rm -f uu_n$(EXE_EXT) ss_n$(EXE_EXT)
