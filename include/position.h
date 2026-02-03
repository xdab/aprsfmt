#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>

/* GPS Fix Status */
typedef enum
{
    GPS_FIX_OLD = 0,    /* Last known position */
    GPS_FIX_CURRENT = 1 /* Current position */
} gps_fix_status_t;

/* NMEA Source */
typedef enum
{
    NMEA_OTHER = 0, /* Other source */
    NMEA_GLL = 1,   /* GLL sentence */
    NMEA_GGA = 2,   /* GGA sentence */
    NMEA_RMC = 3    /* RMC sentence */
} nmea_source_t;

/* Compression Origin */
typedef enum
{
    COMPRESS_COMPRESSED = 0,    /* Compressed by APRS software */
    COMPRESS_TNC_BTEXT = 1,     /* TNC BText */
    COMPRESS_SOFTWARE = 2,      /* Software (DOS/Mac/Win/SA) */
    COMPRESS_KPC3 = 4,          /* KPC3 tracker */
    COMPRESS_PICO = 5,          /* Pico tracker */
    COMPRESS_OTHER_TRACKER = 6, /* Other tracker */
    COMPRESS_DIGIPEATER = 7     /* Digipeater conversion */
} compression_origin_t;

typedef struct
{
    double lat;             /* Latitude in decimal degrees */
    double lon;             /* Longitude in decimal degrees */
    char symbol_table;      /* APRS symbol table (e.g., '/', '\', etc.) */
    char symbol;            /* APRS symbol (icon) */
    bool messaging_capable; /* true if the station is messaging capable, false otherwise */
    bool with_timestamp;    /* true if the position report is valid at a specific timestamp, false otherwise */
} position_uncompressed_t;

typedef struct
{
    double lat;        /* Latitude in decimal degrees */
    double lon;        /* Longitude in decimal degrees */
    char symbol_table; /* APRS symbol table (e.g., '/', '\', etc.) */
    char symbol;       /* APRS symbol (icon) */

    /* Optional course/speed data */
    bool has_course_speed; /* true if cs bytes contain course/speed */
    double course;         /* Course in degrees (0-360), only valid if has_course_speed */
    double speed;          /* Speed in knots, only valid if has_course_speed */

    /* Optional radio range data */
    bool has_radio_range; /* true if cs bytes contain radio range */
    double radio_range;   /* Radio range in miles, only valid if has_radio_range */

    /* Optional altitude data */
    bool has_altitude; /* true if cs bytes contain altitude */
    double altitude;   /* Altitude in feet, only valid if has_altitude */

    /* GPS fix status */
    gps_fix_status_t gps_fix; /* GPS fix status */

    /* NMEA source */
    nmea_source_t nmea_source; /* NMEA source of position data */

    /* Compression origin */
    compression_origin_t compression_origin; /* Origin of compression */
} position_compressed_t;

/**
 * Concatenates an uncompressed position report to the given string.
 *
 * @param str Destination string
 * @param pos Uncompressed position data
 *
 * @return 0 on success, negative on error
 */
int position_format_uncompressed(char *str, const position_uncompressed_t *pos);

/**
 * Concatenates a compressed position report to the given string.
 *
 * @param str Destination string
 * @param pos Compressed position data
 *
 * @return 0 on success, negative on error
 */
int position_format_compressed(char *str, const position_compressed_t *pos);

#endif
