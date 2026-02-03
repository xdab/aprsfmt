#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>

typedef struct
{
    double lat;             /* Latitude in decimal degrees */
    double lon;             /* Longitude in decimal degrees */
    char symbol_table;      /* APRS symbol table (e.g., '/', '\', etc.) */
    char symbol;            /* APRS symbol (icon) */
    bool messaging_capable; /* true if the station is messaging capable, false otherwise */
    bool with_timestamp;    /* true if the position report is valid at a specific timestamp, false otherwise */
} position_uncompressed_t;

/**
 * Concatenates an uncompressed position report to the given string.
 *
 * @param str Destination string
 * @param pos Uncompressed position data
 *
 * @return 0 on success, negative on error
 */
int position_format_uncompressed(char *str, const position_uncompressed_t *pos);

#endif