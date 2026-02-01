#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <argp.h>

#define MAX_CALLSIGN_LEN 16
#define MAX_INFO_LEN 256

struct arguments
{
    char source[MAX_CALLSIGN_LEN + 1];
    char dest[MAX_CALLSIGN_LEN + 1];
    char info[MAX_INFO_LEN + 1];
};

static struct argp_option options[] = {
    {"source", 's', "CALL", 0, "Source callsign/address in TNC2 format", 0},
    {"dest", 'd', "CALL", 0, "Destination callsign/address in TNC2 format", 0},
    {"info", 'i', "STR", 0, "Info string", 0},
    {0, 0, 0, 0, 0, 0}};

error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif