C64SYS?=c64
C64AS?=ca65
C64LD?=ld65

ifeq ($(OS),Windows_NT)
EXEEXT:=.exe
else
EXEEXT:=
endif

mkbin_BIN:=mkbin$(EXEEXT)
basicload_BIN:=basicload$(EXEEXT)

C64ASFLAGS?=-t $(C64SYS) -g
C64LDFLAGS?=-Ln loader.lbl -m loader.map -Csrc/loader.cfg

loader_OBJS:=$(addprefix obj/,main.o)
loader_BIN:=loader.prg

all: $(basicload_BIN)

$(loader_BIN): $(loader_OBJS)
	$(C64LD) -o$@ $(C64LDFLAGS) $^

obj:
	mkdir obj

obj/%.o: src/%.s src/loader.cfg Makefile | obj
	$(C64AS) $(C64ASFLAGS) -o$@ $<

%$(EXEEXT): %.c
	$(CC) -std=c11 -Wall -Wextra -pedantic -g0 -O3 -o$@ $<

$(basicload_BIN): basicload_bin.h

basicload_bin.h: $(loader_BIN) $(mkbin_BIN)
	./$(mkbin_BIN) $@ basicload_bin $<

clean:
	rm -fr obj *.lbl *.map *.prg basicload_bin.h $(mkbin_BIN)

distclean: clean
	rm -f $(basicload_BIN)

.PHONY: all run clean distclean

