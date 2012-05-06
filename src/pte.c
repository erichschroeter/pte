#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "../lib/json-parser/json.h"

static int verbose_flag;

void print_help()
{
    printf("\nprints periodic table of elements information.\n\n");
    printf("Usage:\n"
        "  pte [options] [symbol | atomic number]\n\n"
        "Options:\n"
        "  -h               prints this menu\n"
        "  -v, --version    prints the version number\n"
        "  --verbose        prints verbose information\n");
}

static unsigned get_file_size(const char* file_name)
{
    struct stat sb;
    if (stat(file_name, &sb) != 0)
    {
        fprintf(stderr, "'stat' filaed for '%s': %s.\n", file_name, strerror(errno));
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
        fprintf(stderr, "Could not open '%s': %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    bytes_read = fread(contents, sizeof(unsigned char), s, f);
    if (bytes_read != s)
    {
        fprintf(stderr, "Short read of '%s': expected %d bytes but go %zu: %s.\n", file_name, s, bytes_read, strerror(errno));
        exit(EXIT_FAILURE);
    }

    status = fclose(f);
    if (status != 0)
    {
        fprintf(stderr, "Error closing '%s': %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return contents;
}

json_value* get_element(char* key)
{
    char* data = read_whole_file("../elements.json");
    json_value* elements = json_parse(data);
    int i;
    for (i=0; i < elements->u.object.length; i++)
    {
        json_value* element = elements->u.object.values[i].value;
        char* name;
        char* symbol;
        int atomic_number;
        int j;
        for (j=0; j < element->u.object.length; j++)
        {
            char * subkey = element->u.object.values[j].name;
            json_value* value = element->u.object.values[j].value;
            //printf("checking: '%s'\n", name);
            if (!strcmp(subkey, "name"))
            {
                name = value->u.string.ptr;
            }
            else if (!strcmp(subkey, "symbol"))
            {
                symbol = value->u.string.ptr;
            }
            else if (!strcmp(subkey, "atomicNumber"))
            {
                atomic_number = value->u.integer;
            }
        }
        // name and/or symbol will be NULL if not JSON we support
        if (name == NULL ||
            symbol == NULL)
        {
            continue;
        }

        char atomic_num_str[3];
        sprintf(atomic_num_str, "%d", atomic_number);
        if (!strcmp(name, key))
        {
            return element;
        }
        else if (!strcmp(symbol, key))
        {
            return element;
        }
        else if (!strcmp(atomic_num_str, key))
        {
            return element;
        }
        //json_value* e = elements->u.object.values[i];
        //printf("elements.integer: %s\n", elements.integer);
    }
    return NULL;
}

/**
 * Prints the element information based on the arguments. If no arguments
 * are passed, the help is printed.
 *
 * @param argc the number of arguments
 * @param argv the arguments
 */
void print_element_info(int argc, char** argv) {
    if (argc < 1)
    {
        print_help();
        return;
    }
    printf("printing information for element: %s\n", argv[0]);
    json_value* element = get_element(argv[0]);
    if (element == NULL)
    {
        printf("element is NULL\n");
        return;
    }
    int i;
    for (i=0; i < element->u.object.length; i++)
    {
        char * key = element->u.object.values[i].name;
        json_value* value = element->u.object.values[i].value;
        //printf("checking: '%s'\n", name);
        if (!strcmp(key, "name"))
        {
            printf("name: '%s'\n", value->u.string.ptr);
        }
        else if (!strcmp(key, "symbol"))
        {
            printf("symbol: '%s'\n", value->u.string.ptr);
        }
        else if (!strcmp(key, "atomicNumber"))
        {
            long num = value->u.integer;
            printf("atomic number: '%ld'\n", num);
        }
    }
 }

int main(int argc, char** argv)
{
    static struct option long_options[] =
    {
        {"verbose", no_argument, &verbose_flag, 1},
        {"version", no_argument, NULL, 0}
    };

    int c = getopt_long(argc, argv, "vh", long_options, 0);

    switch(c)
    {
    case 'v':
        printf("1.0.0\n");
        break;
    case 'h':
        print_help();
        break;
    default:
        if (argc < 1)
        {
            print_help();
            return;
        }
        int _argc = argc - 1;
        char* _argv[_argc];
        int i;
        for (i=1; i < argc; i++)
        {
            _argv[i - 1] = argv[i];
        }
        print_element_info(_argc, _argv);
        break;
    }

    return 0;
}
