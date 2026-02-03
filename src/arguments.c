#include <string.h>
#include <stdlib.h>
#include "arguments.h"

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
    case 's':
        strncpy(arguments->source, arg, MAX_CALLSIGN_LEN);
        break;
    case 'd':
        strncpy(arguments->dest, arg, MAX_CALLSIGN_LEN);
        break;
    case 'i':
        strncpy(arguments->info, arg, MAX_INFO_LEN);
        break;
    case 'p':
        strncpy(arguments->path, arg, MAX_PATH_LEN);
        break;
    case 1000: /* --lat */
        arguments->lat = strtod(arg, NULL);
        break;
    case 1001: /* --lon */
        arguments->lon = strtod(arg, NULL);
        break;
    case 1002: /* --messaging */
        arguments->messaging = true;
        break;
    case 1003: /* --timestamped */
        arguments->timestamped = true;
        break;
    case 1004: /* --symbol-table */
        arguments->symbol_table = arg[0];
        break;
    case 1005: /* --symbol */
        arguments->symbol = arg[0];
        break;
    case ARGP_KEY_END:
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}