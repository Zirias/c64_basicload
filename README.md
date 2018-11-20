# BASICload -- a C64 machine code loader for BASIC

This is a little tool to store C64 machine code efficiently in BASIC lines
and load it quickly from there.

## Usage

    basicload [lineno] <{mc.prg} >{output.bas}

 * *lineno*: If given, output starts with this line numer, and the initial
             loader code is omitted. This is useful if you need a second
	     machine code program in the same BASIC program
 * *mc.prg*: The machine code program, in normal PRG format (first two bytes
             are the 16bit load address in little-endian).
 * *output.bas*: The file to write the BASIC sourcecode to

The tool will first put some loader code in the *datasette buffer* using the
conventional DATA/READ/POKE method. This loader code then reads your machine
code program encoded as HEX strings and, when finished, directly starts it. It
is assumed the entry point of your program is equal to the load address.

