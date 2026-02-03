#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <argp.h>
#include <stdbool.h>

#define MAX_CALLSIGN_LEN 16
#define MAX_INFO_LEN 256
#define MAX_PATH_LEN 512

struct arguments
{
    char source[MAX_CALLSIGN_LEN + 1];
    char dest[MAX_CALLSIGN_LEN + 1];
    char info[MAX_INFO_LEN + 1];
    char path[MAX_PATH_LEN + 1];
    double lat;
    double lon;
    bool messaging;
    bool timestamped;
    char symbol_table;
    char symbol;
    bool compressed;
};

static struct argp_option options[] = {
    {"source", 's', "CALL", 0, "Source callsign/address in TNC2 format", 0},
    {"dest", 'd', "CALL", 0, "Destination callsign/address in TNC2 format", 0},
    {"path", 'p', "ADDR,ADDR,...", 0, "Path addresses in TNC2 format", 0},
    {"info", 'i', "STR", 0, "Info string", 0},
    {"lat", 1000, "DD.DDDDD", 0, "Latitude in decimal degrees", 0},
    {"lon", 1001, "DDD.DDDDD", 0, "Longitude in decimal degrees", 0},
    {"messaging", 1002, 0, 0, "Station is messaging capable", 0},
    {"timestamped", 1003, 0, 0, "Position report is timestamped", 0},
    {"symbol-table", 1004, "CHAR", 0, "APRS symbol table (default /)", 0},
    {"symbol", 1005, "CHAR", 0, "APRS symbol (default - for home)", 0},
    {"compressed", 1006, 0, 0, "Use compressed position format", 0},
    {0, 0, 0, 0, 0, 0}};

error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif