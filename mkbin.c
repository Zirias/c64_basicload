#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BUFSIZE 1024

char buf[BUFSIZE];

int main(int argc, char **argv)
{
    int ret = -1;
    FILE *bin = 0;
    FILE *out = 0;
    char *name = 0;

    if (argc != 4) goto cleanup;

    out = fopen(argv[1], "w");
    if (!out) goto cleanup;
    name = argv[2];
    bin = fopen(argv[3], "rb");
    if (!bin) goto cleanup;

    uint16_t load;
    int c;
    if ((c = fgetc(bin)) < 0) goto cleanup;
    load = (unsigned char) c;
    if ((c = fgetc(bin)) < 0) goto cleanup;
    load |= ((unsigned char)c << 8);

    fprintf(out, "#include <stdint.h>\n\n"
            "static const uint16_t %s_load = 0x%04x;\n"
            "static const uint8_t %s[] = {", name, (unsigned)load, name);

    int first = 1;
    size_t b;
    while ((b = fread(buf, 1, BUFSIZE, bin)))
    {
        for (size_t i = 0; i < b; ++i)
        {
            if (!first) fputc(',', out);
            first = 0;
            fprintf(out, "0x%02x", (uint8_t)buf[i]);
        }
    }
    fputs("};\n", out);
    ret = 0;

cleanup:
    if (bin) fclose(bin);
    if (out) fclose(out);
    return ret;
}

