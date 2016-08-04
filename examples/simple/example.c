
/*
 * Description: An example of libalex API do handle simple DIALOGs, without
 *              function callbacks and other complicated stuff.
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Jul 26 11:30:12 2016
 * Project: simple example
 */

#include <stdlib.h>
#include <unistd.h>

#include "libalex.h"

int main(int argc, char **argv)
{
    const char *opt = "f:\0";
    int option;
    char *filename = NULL;
    struct al_grc *grc = NULL;

    do {
        option = getopt(argc, argv, opt);

        switch (option) {
            case 'f':
                filename = strdup(optarg);
                break;

            case '?':
                return -1;
        }
    } while (option != -1);

    grc = al_grc_init_from_file(filename, true);

    if (NULL == grc) {
        fprintf(stderr, "Error: %s\n", al_strerror(al_get_last_error()));
        goto end_block;
    }

    al_grc_prepare_dialog(grc);

    if (al_grc_do_dialog(grc) < 0)
        fprintf(stderr, "Error: %s\n", al_strerror(al_get_last_error()));

end_block:
    if (grc != NULL)
        al_grc_uninit(grc);

    if (filename != NULL)
        free(filename);

    return 0;
}

