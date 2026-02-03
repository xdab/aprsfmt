#ifndef TEST_POSITION_COMPRESSED_H
#define TEST_POSITION_COMPRESSED_H

#include "test.h"
#include "position.h"
#include <math.h>
#include <string.h>

/* Compressed format buffer indices: <DTI><symbol_table><YYYY><XXXX><symbol><cs1><cs2><T> */
#define COMPRESSED_DTI_IDX 0
#define COMPRESSED_SYMBOL_TABLE_IDX 1
#define COMPRESSED_LAT_START_IDX 2
#define COMPRESSED_LON_START_IDX 6
#define COMPRESSED_SYMBOL_IDX 10
#define COMPRESSED_CS1_IDX 11
#define COMPRESSED_CS2_IDX 12
#define COMPRESSED_T_BYTE_IDX 13

static position_compressed_t default_pos(void)
{
    return (position_compressed_t){
        .lat = 0.0,
        .lon = 0.0,
        .symbol_table = '/',
        .symbol = '>',
        .messaging_capable = false,
        .with_timestamp = false,
        .has_course_speed = false,
        .has_radio_range = false,
        .has_altitude = false,
        .gps_fix = GPS_FIX_OLD,
        .nmea_source = NMEA_OTHER,
        .compression_origin = COMPRESS_COMPRESSED};
}

void test_compressed_format_basic()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lat = 49.5;
    pos.lon = -72.75;
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format basic success");
    assert_string(buffer, "!/5L!!<*e7>  [", "format basic output");
}

void test_compressed_format_course_speed()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lat = 49.5;
    pos.lon = -72.75;
    pos.has_course_speed = true;
    pos.course = 90.0;
    pos.speed = 30.0;
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format course/speed success");
    assert_true(strncmp(buffer, "!/5L!!<*e7>", 11) == 0, "format course/speed prefix matches");
    assert_true(buffer[COMPRESSED_CS1_IDX] != ' ', "course byte is not space");
    assert_true(buffer[COMPRESSED_CS2_IDX] != ' ', "speed byte is not space");
    assert_true(buffer[COMPRESSED_T_BYTE_IDX] == '[', "T byte is correct for GPS_FIX_CURRENT/NMEA_RMC/COMPRESS_SOFTWARE");
}

void test_compressed_format_course_speed_zero()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.has_course_speed = true;
    pos.course = 0.0; /* Should encode to 0 */
    pos.speed = 0.0;  /* Should encode to 0 */

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format zero course/speed success");
    /* DTI='!' (no messaging, no timestamp), course=0 -> '!' (33), speed=0 -> '!' (33), T=0 -> '!' */
    assert_string(buffer, "!/NN!!NN!!>!!!", "format zero course/speed output");
}

void test_compressed_format_radio_range()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lat = 49.5;
    pos.lon = -72.75;
    pos.has_radio_range = true;
    pos.radio_range = 10.0; /* Range in miles */
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format radio range success");
    assert_true(strncmp(buffer, "!/5L!!<*e7>", 11) == 0, "format radio range prefix matches");
    /* cs1 should be '{' for radio range */
    assert_true(buffer[COMPRESSED_CS1_IDX] == '{', "cs1 is '{' for radio range");
    assert_true(buffer[COMPRESSED_CS2_IDX] != ' ', "cs2 is not space for valid range");
}

void test_compressed_format_altitude()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lat = 49.5;
    pos.lon = -72.75;
    pos.has_altitude = true;
    pos.altitude = 1000.0; /* 1000 feet */
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format altitude success");
    assert_true(strncmp(buffer, "!/5L!!<*e7>", 11) == 0, "format altitude prefix matches");
    assert_true(buffer[COMPRESSED_CS1_IDX] != ' ', "cs1 is not space for valid altitude");
    assert_true(buffer[COMPRESSED_CS2_IDX] != ' ', "cs2 has value even though altitude only uses cs1 - this is how the code works");
}

void test_compressed_format_nmea_sources()
{
    /* Test all NMEA source values */
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();

    /* NMEA_GLL = 1 */
    pos.nmea_source = NMEA_GLL;
    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_T_BYTE_IDX] == ')', "NMEA_GLL T byte is correct");

    /* NMEA_GGA = 2 */
    memset(buffer, 0, sizeof(buffer));
    pos.nmea_source = NMEA_GGA;
    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_T_BYTE_IDX] == '1', "NMEA_GGA T byte is correct");

    /* NMEA_RMC = 3 */
    memset(buffer, 0, sizeof(buffer));
    pos.nmea_source = NMEA_RMC;
    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_T_BYTE_IDX] == '9', "NMEA_RMC T byte is correct");
}

void test_compressed_format_compression_origins()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();

    /* COMPRESS_TNC_BTEXT = 1 */
    pos.compression_origin = COMPRESS_TNC_BTEXT;
    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_T_BYTE_IDX] == '\"', "COMPRESS_TNC_BTEXT T byte is correct");

    /* COMPRESS_KPC3 = 4 */
    memset(buffer, 0, sizeof(buffer));
    pos.compression_origin = COMPRESS_KPC3;
    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_T_BYTE_IDX] == '%', "COMPRESS_KPC3 T byte is correct");

    /* COMPRESS_DIGIPEATER = 7 */
    memset(buffer, 0, sizeof(buffer));
    pos.compression_origin = COMPRESS_DIGIPEATER;
    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_T_BYTE_IDX] == '(', "COMPRESS_DIGIPEATER T byte is correct");
}

void test_compressed_format_gps_fix_status()
{
    char buffer[256] = {0};
    /* GPS_FIX_CURRENT sets bit 5 (value 32) */
    position_compressed_t pos = default_pos();
    pos.gps_fix = GPS_FIX_CURRENT;

    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_CS2_IDX] == ' ', "No extra data bytes are space");
    assert_true((buffer[COMPRESSED_T_BYTE_IDX] - 33) & 0x20, "GPS fix bit is set for CURRENT");
}

void test_compressed_format_equator_prime_meridian()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format equator/prime meridian success");
    /* At 0,0: lat compressed = 380926 * 90 = 34283340 -> 'NN!!'
     * lon compressed = 190463 * 180 = 34283340 -> 'NN!!' */
    assert_string(buffer, "!/NN!!NN!!>  [", "format equator/prime meridian output");
}

void test_compressed_format_north_pole()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lat = 90.0;
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format north pole success");
    /* At 90 deg lat: compressed = 380926 * 0 = 0 -> all bytes '!'
     * At 0 deg lon: compressed = 190463 * 180 = 34283340 -> 'NN!!' */
    assert_string(buffer, "!/!!!!NN!!>  [", "format north pole output");
}

void test_compressed_format_south_pole()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lat = -90.0;
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format south pole success");
    /* At -90 deg lat: compressed = 380926 * 180 = 68566680 -> '{{!!'
     * At 0 deg lon: compressed = 190463 * 180 = 34283340 -> 'NN!!' */
    assert_string(buffer, "!/{{!!NN!!>  [", "format south pole output");
}

void test_compressed_format_max_longitude()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lon = 180.0;
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format max longitude success");
    /* At 0 deg lat: compressed = 380926 * 90 = 34283340 -> 'NN!!'
     * At 180 deg lon: compressed = 190463 * 360 = 68566680 -> '{{!!' */
    assert_string(buffer, "!/NN!!{{!!>  [", "format max longitude output");
}

void test_compressed_format_min_longitude()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lon = -180.0;
    pos.gps_fix = GPS_FIX_CURRENT;
    pos.nmea_source = NMEA_RMC;
    pos.compression_origin = COMPRESS_SOFTWARE;

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "format min longitude success");
    /* At 0 deg lat: compressed = 380926 * 90 = 34283340 -> 'NN!!'
     * At -180 deg lon: compressed = 190463 * 0 = 0 -> '!!!!' */
    assert_string(buffer, "!/NN!!!!!!>  [", "format min longitude output");
}

void test_compressed_format_append_to_existing()
{
    char buffer[256] = "PREFIX";
    position_compressed_t pos = default_pos();

    int ret = position_format_compressed(buffer, &pos);
    assert_equal_int(ret, 0, "append to existing success");
    assert_string(buffer, "PREFIX!/NN!!NN!!>  !", "append to existing output");
}

void test_compressed_format_invalid_latitude()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lat = 95.0;
    pos.symbol = '-';

    int ret = position_format_compressed(buffer, &pos);
    assert_true(ret < 0, "invalid latitude rejected");
}

void test_compressed_format_invalid_longitude()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.lon = 200.0;
    pos.symbol = '-';

    int ret = position_format_compressed(buffer, &pos);
    assert_true(ret < 0, "invalid longitude rejected");
}

void test_compressed_format_null_buffer()
{
    position_compressed_t pos = default_pos();
    pos.lat = 49.5;
    pos.lon = -72.75;
    pos.symbol = '-';

    int ret = position_format_compressed(NULL, &pos);
    assert_true(ret < 0, "null buffer rejected");
}

void test_compressed_format_null_pos()
{
    char buffer[256] = {0};
    int ret = position_format_compressed(buffer, NULL);
    assert_true(ret < 0, "null pos rejected");
}

void test_compressed_format_dti_no_messaging_no_timestamp()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.messaging_capable = false;
    pos.with_timestamp = false;

    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_DTI_IDX] == '!', "DTI '!' for no messaging, no timestamp");
}

void test_compressed_format_dti_messaging_no_timestamp()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.messaging_capable = true;
    pos.with_timestamp = false;

    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_DTI_IDX] == '=', "DTI '=' for messaging capable, no timestamp");
}

void test_compressed_format_dti_no_messaging_with_timestamp()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.messaging_capable = false;
    pos.with_timestamp = true;

    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_DTI_IDX] == '/', "DTI '/' for no messaging, with timestamp");
}

void test_compressed_format_dti_messaging_with_timestamp()
{
    char buffer[256] = {0};
    position_compressed_t pos = default_pos();
    pos.messaging_capable = true;
    pos.with_timestamp = true;

    position_format_compressed(buffer, &pos);
    assert_true(buffer[COMPRESSED_DTI_IDX] == '@', "DTI '@' for messaging capable, with timestamp");
}

#endif
