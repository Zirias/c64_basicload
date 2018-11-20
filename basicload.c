#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifdef _WIN32
# include <io.h>
# include <fcntl.h>
# define SET_BINARY_MODE(stream) setmode(fileno(stream), O_BINARY)
#else
# define SET_BINARY_MODE(stream) ((void)0)
#endif

#define BUFSIZE 64*1024

#include "basicload_bin.h"

unsigned char buf[BUFSIZE];

int main(int argc, char **argv)
{
    SET_BINARY_MODE(stdin);
    SET_BINARY_MODE(stdout);

    size_t nread = fread(buf, 1, BUFSIZE, stdin);
    if (!nread)
    {
        fputs("Error: couldn't read any data\n", stderr);
        return 1;
    }

    size_t rembuf = BUFSIZE;
    unsigned char *bufptr = buf;
    size_t nchunk = nread;
    while (nchunk && nchunk < rembuf)
    {
        bufptr += nchunk;
        rembuf -= nchunk;
        nchunk = fread(bufptr, 1, rembuf, stdin);
        nread += nchunk;
    }

    int lineno = 0;
    if (argc>1)
    {
	lineno = atoi(argv[1]);
    }

    const unsigned char *r = basicload_bin;
    int remaining = 0;

    if (lineno == 0)
    {
	size_t basicload_size = sizeof basicload_bin;
	uint16_t endaddress = basicload_bin_load + basicload_size - 1;

	remaining = 79 - printf("0 fora=%" PRIu16 "to%" PRIu16
		":readb:pokea,b:next:data%u", basicload_bin_load,
		endaddress, (unsigned)*r++);

	--basicload_size;
	while (basicload_size--)
	{
	    int needed=2;
	    if (*r>9)++needed;
	    if (*r>99)++needed;
	    if (remaining - needed < 0)
	    {
		++lineno;
		remaining = 74-needed;
		if (lineno>9) --remaining;
		if (lineno>99) --remaining;
		if (lineno>999) --remaining;
		if (lineno>9999) --remaining;
		printf("\n%d data%u", lineno, (unsigned)*r++);
	    }
	    else
	    {
		remaining -= needed;
		printf(",%u", (unsigned)*r++);
	    }
	}
	if (remaining < 11)
	{
	    ++lineno;
	    remaining = 80 - printf("\n%d sys%" PRIu16 ",\"",
		   lineno, basicload_bin_load);
	}
	else
	{
	    remaining -= printf(":sys%" PRIu16 ",\"", basicload_bin_load);
	}
    }
    else
    {
	remaining = 80 - printf("\n%d sys%" PRIu16 ",\"",
	       lineno, basicload_bin_load);
    }

    r = buf;
    while (nread--)
    {
        uint8_t lnib = *r++;
        uint8_t hnib = lnib >> 4;
        lnib &= 0xf;
        if (remaining < 2)
        {
            ++lineno;
            remaining = 75;
            if (lineno>9) --remaining;
            if (lineno>99) --remaining;
            if (lineno>999) --remaining;
            if (lineno>9999) --remaining;
            printf("\"\n%d \"%x", lineno, (unsigned)hnib);
        }
        else
        {
            --remaining;
            printf("%x", (unsigned)hnib);
        }
        if (remaining < 2)
        {
            ++lineno;
            remaining = 75;
            if (lineno>9) --remaining;
            if (lineno>99) --remaining;
            if (lineno>999) --remaining;
            if (lineno>9999) --remaining;
            printf("\"\n%d \"%x", lineno, (unsigned)lnib);
        }
        else
        {
            --remaining;
            printf("%x", (unsigned)lnib);
        }
    }
    puts("\"");

    return 0;
}
