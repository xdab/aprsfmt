#include <position.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define LAT_MIN -90.0
#define LAT_MAX 90.0
#define LON_MIN -180.0
#define LON_MAX 180.0

/* Format strings */
#define LAT_FORMAT "%02d%02d.%02d"
#define LON_FORMAT "%03d%02d.%02d"

/* Hemisphere characters */
#define HEMISPHERE_NORTH 'N'
#define HEMISPHERE_SOUTH 'S'
#define HEMISPHERE_EAST 'E'
#define HEMISPHERE_WEST 'W'

/* Data Type Indicator characters */
#define DTI_MESSAGING_WITH_TIMESTAMP '@'
#define DTI_NO_MESSAGING_WITH_TIMESTAMP '/'
#define DTI_MESSAGING_NO_TIMESTAMP '='
#define DTI_NO_MESSAGING_NO_TIMESTAMP '!'

/**
 * Convert decimal degrees (dd.ddd) latitude to APRS format (ddmm.hh)
 *
 * @param lat Latitude in decimal degrees
 * @param buf Output buffer
 *
 * @return 0 on success, negative on error
 */
static int format_latitude(double lat, char *buf)
{
    if (lat < LAT_MIN || lat > LAT_MAX)
    {
        return -1;
    }

    double abs_lat = fabs(lat);
    int degrees = (int)abs_lat;
    double minutes_f = (abs_lat - degrees) * 60.0;
    int minutes = (int)minutes_f;
    int hundredths = (int)((minutes_f - minutes) * 100.0 + 0.5);

    /* Handle overflow from rounding */
    if (hundredths >= 100)
    {
        hundredths -= 100;
        minutes++;
        if (minutes >= 60)
        {
            minutes -= 60;
            degrees++;
        }
    }

    sprintf(buf, LAT_FORMAT, degrees, minutes, hundredths);
    return 0;
}

/**
 * Convert decimal degrees (ddd.ddd) longitude to APRS format (dddmm.hh)
 *
 * @param lon Longitude in decimal degrees
 * @param buf Output buffer
 *
 * @return 0 on success, negative on error
 */
static int format_longitude(double lon, char *buf)
{
    if (lon < LON_MIN || lon > LON_MAX)
    {
        return -1;
    }

    double abs_lon = fabs(lon);
    int degrees = (int)abs_lon;
    double minutes_f = (abs_lon - degrees) * 60.0;
    int minutes = (int)minutes_f;
    int hundredths = (int)((minutes_f - minutes) * 100.0 + 0.5);

    /* Handle overflow from rounding */
    if (hundredths >= 100)
    {
        hundredths -= 100;
        minutes++;
        if (minutes >= 60)
        {
            minutes -= 60;
            degrees++;
        }
    }

    sprintf(buf, LON_FORMAT, degrees, minutes, hundredths);
    return 0;
}

/**
 * Get latitude hemisphere character
 *
 * @param lat Latitude in decimal degrees
 *
 * @return 'N' for north, 'S' for south
 */
static char get_hemisphere_lat(double lat)
{
    return lat >= 0 ? HEMISPHERE_NORTH : HEMISPHERE_SOUTH;
}

/**
 * Get longitude hemisphere character
 *
 * @param lon Longitude in decimal degrees
 *
 * @return 'E' for east, 'W' for west
 */
static char get_hemisphere_lon(double lon)
{
    return lon >= 0 ? HEMISPHERE_EAST : HEMISPHERE_WEST;
}

int position_format_uncompressed(char *buffer, const position_uncompressed_t *pos)
{
    char lat_buf[9];
    char lon_buf[10];
    char hem_lat;
    char hem_lon;
    char dti; /* Data Type Indicator */
    int ret;

    if (buffer == NULL || pos == NULL)
    {
        return -1;
    }

    ret = format_latitude(pos->lat, lat_buf);
    if (ret < 0)
        return ret;

    ret = format_longitude(pos->lon, lon_buf);
    if (ret < 0)
        return ret;

    hem_lat = get_hemisphere_lat(pos->lat);
    hem_lon = get_hemisphere_lon(pos->lon);

    if (pos->with_timestamp)
        dti = pos->messaging_capable ? DTI_MESSAGING_WITH_TIMESTAMP : DTI_NO_MESSAGING_WITH_TIMESTAMP;
    else
        dti = pos->messaging_capable ? DTI_MESSAGING_NO_TIMESTAMP : DTI_NO_MESSAGING_NO_TIMESTAMP;

    /* Build the position string: <DTI><lat><hem><table><lon><hem><symbol> */
    strncat(buffer, &dti, 1);
    strcat(buffer, lat_buf);
    strncat(buffer, &hem_lat, 1);
    strncat(buffer, &pos->symbol_table, 1);
    strcat(buffer, lon_buf);
    strncat(buffer, &hem_lon, 1);
    strncat(buffer, &pos->symbol, 1);

    return 0;
}
