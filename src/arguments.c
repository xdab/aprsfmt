#include <string.h>
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
    case ARGP_KEY_END:
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}