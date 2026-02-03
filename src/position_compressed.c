#include <position.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define LAT_MIN -90.0
#define LAT_MAX 90.0
#define LON_MIN -180.0
#define LON_MAX 180.0

/* Base-91 encoding/decoding constants */
#define BASE_91_MIN 33  /* ASCII '!' */
#define BASE_91_MAX 120 /* ASCII 'x' */
#define BASE_91_RANGE (BASE_91_MAX - BASE_91_MIN + 1)

/* Compression constants */
#define LAT_COMPRESSION_FACTOR 380926
#define LON_COMPRESSION_FACTOR 190463

/* Data Type Indicator characters */
#define DTI_NO_MESSAGING_NO_TIMESTAMP '!'
#define DTI_MESSAGING_NO_TIMESTAMP '='
#define DTI_NO_MESSAGING_WITH_TIMESTAMP '/'
#define DTI_MESSAGING_WITH_TIMESTAMP '@'

/**
 * Convert a numeric value to base-91 encoded ASCII character
 *
 * @param value Numeric value to encode (0-8190)
 *
 * @return Base-91 encoded ASCII character
 */
static char encode_base_91(int value)
{
    if (value < 0 || value > 8190)
        return '\0';
    return (char)(value + BASE_91_MIN);
}

/**
 * Convert decimal degrees (dd.ddd) latitude to APRS compressed format (YYYY)
 *
 * @param lat Latitude in decimal degrees
 * @param buf Output buffer (4 chars minimum)
 *
 * @return 0 on success, negative on error
 */
static int compress_latitude(double lat, char *buf)
{
    if (lat < LAT_MIN || lat > LAT_MAX)
    {
        return -1;
    }

    /* Formula: YYYY = 380926 * (90 - latitude) */
    double compressed = LAT_COMPRESSION_FACTOR * (90.0 - lat);
    int value = (int)compressed;

    /* Encode to base-91 */
    buf[3] = encode_base_91(value % 91);
    value /= 91;
    buf[2] = encode_base_91(value % 91);
    value /= 91;
    buf[1] = encode_base_91(value % 91);
    value /= 91;
    buf[0] = encode_base_91(value);

    return 0;
}

/**
 * Convert decimal degrees (ddd.ddd) longitude to APRS compressed format (XXXX)
 *
 * @param lon Longitude in decimal degrees
 * @param buf Output buffer (4 chars minimum)
 *
 * @return 0 on success, negative on error
 */
static int compress_longitude(double lon, char *buf)
{
    if (lon < LON_MIN || lon > LON_MAX)
    {
        return -1;
    }

    /* Formula: XXXX = 190463 * (180 + longitude) */
    double compressed = LON_COMPRESSION_FACTOR * (180.0 + lon);
    int value = (int)compressed;

    /* Encode to base-91 */
    buf[3] = encode_base_91(value % 91);
    value /= 91;
    buf[2] = encode_base_91(value % 91);
    value /= 91;
    buf[1] = encode_base_91(value % 91);
    value /= 91;
    buf[0] = encode_base_91(value);

    return 0;
}

/**
 * Build the cs bytes (course/speed, radio range, or altitude)
 *
 * @param pos Compressed position data
 * @param cs1 First cs byte output
 * @param cs2 Second cs byte output
 *
 * @return 0 on success, negative on error
 */
static int build_cs_bytes(const position_compressed_t *pos, char *cs1, char *cs2)
{
    if (pos->has_course_speed)
    {
        /* Course/Speed encoding */
        /* course = c * 4, speed = 1.08^s - 1 */
        int c = (int)(pos->course / 4.0);
        int s = (int)(log(pos->speed + 1.0) / log(1.08) + 0.5);

        if (c < 0)
            c = 0;
        if (c > 89)
            c = 89;
        if (s < 0)
            s = 0;
        if (s > 89)
            s = 89;

        *cs1 = encode_base_91(c);
        *cs2 = encode_base_91(s);
    }
    else if (pos->has_radio_range)
    {
        /* Radio range encoding */
        /* range = 2 * 1.08^s, c must be '{' (ASCII 123) */
        int s = (int)(log(pos->radio_range / 2.0) / log(1.08) + 0.5);

        if (s < 0)
            s = 0;
        if (s > 89)
            s = 89;

        *cs1 = '{'; /* Fixed character for radio range */
        *cs2 = encode_base_91(s);
    }
    else if (pos->has_altitude)
    {
        /* Altitude encoding */
        /* altitude = 1.08^cs feet */
        int cs = (int)(log(pos->altitude) / log(1.08) + 0.5);

        if (cs < 0)
            cs = 0;
        if (cs > 8190)
            cs = 8190;

        *cs1 = encode_base_91(cs);
        *cs2 = '\0'; /* Second byte not used for altitude */
    }
    else
    {
        /* No cs data - use space */
        *cs1 = ' ';
        *cs2 = ' ';
    }

    return 0;
}

/**
 * Build the T byte (compression type indicator)
 *
 * @param pos Compressed position data
 *
 * @return T byte value (0-127)
 */
static int build_t_byte(const position_compressed_t *pos)
{
    /* T byte format:
     * Bits 7-6: Not used (0)
     * Bit 5: GPS fix (0=old, 1=current)
     * Bits 4-3: NMEA source (00=other, 01=GLL, 10=GGA, 11=RMC)
     * Bits 2-0: Compression origin (000=compressed, 001=TNC BText, 010=Software, 011=[tbd],
     *           100=KPC3, 101=Pico, 110=Other tracker, 111=Digipeater)
     */

    int t = 0;

    if (pos->gps_fix == GPS_FIX_CURRENT)
        t |= (1 << 5);
    t |= (pos->nmea_source << 3);
    t |= pos->compression_origin;

    return t;
}

int position_format_compressed(char *buffer, const position_compressed_t *pos)
{
    char lat_buf[5] = {0};
    char lon_buf[5] = {0};
    char cs1, cs2;
    int t;
    int ret;
    size_t len;
    char dti;

    if (buffer == NULL || pos == NULL)
    {
        return -1;
    }

    ret = compress_latitude(pos->lat, lat_buf);
    if (ret < 0)
        return ret;

    ret = compress_longitude(pos->lon, lon_buf);
    if (ret < 0)
        return ret;

    /* Build cs bytes */
    ret = build_cs_bytes(pos, &cs1, &cs2);
    if (ret < 0)
        return ret;

    /* Build T byte */
    t = build_t_byte(pos);
    char t_char = encode_base_91(t);

    /* Build DTI (Data Type Indicator) character */
    if (pos->with_timestamp)
        dti = pos->messaging_capable ? DTI_MESSAGING_WITH_TIMESTAMP : DTI_NO_MESSAGING_WITH_TIMESTAMP;
    else
        dti = pos->messaging_capable ? DTI_MESSAGING_NO_TIMESTAMP : DTI_NO_MESSAGING_NO_TIMESTAMP;

    /* Build the compressed position string: <DTI><symbol_table><YYYY><XXXX><symbol><cs1><cs2><T> */
    len = strlen(buffer);

    buffer[len++] = dti;
    buffer[len++] = pos->symbol_table;
    memcpy(buffer + len, lat_buf, 4);
    len += 4;
    memcpy(buffer + len, lon_buf, 4);
    len += 4;
    buffer[len++] = pos->symbol;
    buffer[len++] = cs1;
    buffer[len++] = cs2;
    buffer[len++] = t_char;
    buffer[len] = '\0';

    return 0;
}
