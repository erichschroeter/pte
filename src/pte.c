#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "pte.h"
#include "elements.h"
#include "printers/printer-default.h"
#include "printers/printer-json.h"

static int verbose_flag;

void print_help()
{
    printf("\nprints periodic table of elements information.\n\n");
    printf("Usage:\n"
        "  pte [options] name | symbol | atomic number ...\n\n"
        "Options:\n"
        "  -h               prints this menu\n"
        "  -v, --version    prints the version number\n"
        "  --verbose        prints verbose information\n\n"
        "Examples:\n"
        "  pte Carbon\n"
        "  pte C\n"
        "  pte 6\n"
        "\n");
}

json_value* get_element(char* key)
{
    json_value* elements = json_parse(ELEMENTS_JSON);
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
        if (!strcasecmp(name, key))
        {
            return element;
        }
        else if (!strcasecmp(symbol, key))
        {
            return element;
        }
        else if (!strcmp(atomic_num_str, key))
        {
            return element;
        }
    }
    return NULL;
}

#define PRINTER_DEFAULT 0
#define PRINTER_JSON 1

/**
 * Prints the element information based on the arguments. If no arguments
 * are passed, the help is printed.
 *
 * @param argc the number of arguments
 * @param argv the arguments
 */
void print_element_info(int argc, char** argv) {
    static int printer;
    static struct option printer_options[] =
    {
        {"json", no_argument, &printer, PRINTER_JSON},
        {"default", no_argument, &printer, PRINTER_DEFAULT}
    };

    int c = getopt_long(argc, argv, "", printer_options, 0);

    switch(printer)
    {
    case PRINTER_JSON:
        print_json(argc, argv);
        break;
    case PRINTER_DEFAULT:
    default:
        print_default(argc, argv);
        break;
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
        // if we don't have an argument we can't do anything
        if (argc < 2)
        {
            break;
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
