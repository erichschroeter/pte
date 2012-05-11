// thanks to http://www.lemoda.net/c/read-whole-file/index.html
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include "util-file.h"

static unsigned get_file_size(const char* file_name)
{
    struct stat sb;
    if (stat(file_name, &sb) != 0)
    {
        fprintf(stderr, "'stat' failed for '%s': %d.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}

static unsigned char* read_whole_file(const char* file_name)
{
    unsigned s;
    unsigned char * contents;
    FILE * f;
    size_t bytes_read;
    int status;

    s = get_file_size(file_name);
    contents = malloc(s + 1);
    if (!contents)
    {
        fprintf(stderr, "Not enough memory.\n");
        exit(EXIT_FAILURE);
    }

    f = fopen(file_name, "r");
    if (!f)
    {
        fprintf(stderr, "Could not open '%s': %d.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    bytes_read = fread(contents, sizeof(unsigned char), s, f);
    if (bytes_read != s)
    {
        fprintf(stderr, "Short read of '%s': expected %d bytes but go %zu: %d.\n", file_name, s, bytes_read, strerror(errno));
        exit(EXIT_FAILURE);
    }

    status = fclose(f);
    if (status != 0)
    {
        fprintf(stderr, "Error closing '%s': %d.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return contents;
}


