
/*
 * Description: An example of libgrc API do handle simple DIALOGs, without
 *              function callbacks and other complicated stuff.
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Jul 26 11:30:12 2016
 * Project: simple example
 */

#include <stdlib.h>
#include <unistd.h>

#include "libgrc.h"

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

    grc = grc_init_from_file(filename, true);

    if (NULL == grc) {
        fprintf(stderr, "Error: %s\n", grc_strerror(grc_get_last_error()));
        goto end_block;
    }

    grc_prepare_dialog(grc);

    if (grc_do_dialog(grc) < 0)
        fprintf(stderr, "Error: %s\n", grc_strerror(grc_get_last_error()));

end_block:
    if (grc != NULL)
        grc_uninit(grc);

    if (filename != NULL)
        free(filename);

    return 0;
}

