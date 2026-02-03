#ifndef TEST_POSITION_H
#define TEST_POSITION_H

#include "test.h"
#include "position.h"

void test_position_format_basic()
{
    char buffer[256] = {0};
    position_uncompressed_t pos = {
        .lat = 12.34567,
        .lon = 23.4567,
        .symbol_table = '#',
        .symbol = 'D',
        .messaging_capable = false,
        .with_timestamp = false};

    int ret = position_format_uncompressed(buffer, &pos);
    assert_equal_int(ret, 0, "format basic success");
    assert_string(buffer, "!1220.74N#02327.40ED", "format basic output");
}

void test_position_format_messaging_no_timestamp()
{
    char buffer[256] = {0};
    position_uncompressed_t pos = {
        .lat = 12.34567,
        .lon = 23.4567,
        .symbol_table = '#',
        .symbol = 'D',
        .messaging_capable = true,
        .with_timestamp = false};

    int ret = position_format_uncompressed(buffer, &pos);
    assert_equal_int(ret, 0, "format messaging no timestamp success");
    assert_string(buffer, "=1220.74N#02327.40ED", "format messaging no timestamp output");
}

void test_position_format_timestamp_no_messaging()
{
    char buffer[256] = {0};
    position_uncompressed_t pos = {
        .lat = -12.34567,
        .lon = 23.4567,
        .symbol_table = '/',
        .symbol = '>',
        .messaging_capable = false,
        .with_timestamp = true};

    int ret = position_format_uncompressed(buffer, &pos);
    assert_equal_int(ret, 0, "format timestamp no messaging success");
    assert_string(buffer, "/1220.74S/02327.40E>", "format timestamp no messaging output");
}

void test_position_format_timestamp_with_messaging()
{
    char buffer[256] = {0};
    position_uncompressed_t pos = {
        .lat = 12.34567,
        .lon = -23.4567,
        .symbol_table = '/',
        .symbol = '>',
        .messaging_capable = true,
        .with_timestamp = true};

    int ret = position_format_uncompressed(buffer, &pos);
    assert_equal_int(ret, 0, "format timestamp with messaging success");
    assert_string(buffer, "@1220.74N/02327.40W>", "format timestamp with messaging output");
}

void test_position_format_southern_western_hemisphere()
{
    char buffer[256] = {0};
    position_uncompressed_t pos = {
        .lat = -12.34567,
        .lon = -23.4567,
        .symbol_table = '/',
        .symbol = '>',
        .messaging_capable = false,
        .with_timestamp = false};

    int ret = position_format_uncompressed(buffer, &pos);
    assert_equal_int(ret, 0, "format southern/western hemisphere success");
    assert_string(buffer, "!1220.74S/02327.40W>", "format southern/western hemisphere output");
}

void test_position_format_invalid_latitude()
{
    char buffer[256] = {0};
    position_uncompressed_t pos = {
        .lat = 95.0,
        .lon = 0.0,
        .symbol_table = '/',
        .symbol = '>',
        .messaging_capable = false,
        .with_timestamp = false};

    int ret = position_format_uncompressed(buffer, &pos);
    assert_true(ret < 0, "invalid latitude rejected");
}

void test_position_format_invalid_longitude()
{
    char buffer[256] = {0};
    position_uncompressed_t pos = {
        .lat = 0.0,
        .lon = 200.0,
        .symbol_table = '/',
        .symbol = '>',
        .messaging_capable = false,
        .with_timestamp = false};

    int ret = position_format_uncompressed(buffer, &pos);
    assert_true(ret < 0, "invalid longitude rejected");
}

void test_position_format_null_buffer()
{
    position_uncompressed_t pos = {
        .lat = 12.34567,
        .lon = 23.4567,
        .symbol_table = '#',
        .symbol = 'D',
        .messaging_capable = false,
        .with_timestamp = false};

    int ret = position_format_uncompressed(NULL, &pos);
    assert_true(ret < 0, "null buffer rejected");
}

void test_position_format_null_pos()
{
    char buffer[256] = {0};
    int ret = position_format_uncompressed(buffer, NULL);
    assert_true(ret < 0, "null pos rejected");
}

void test_position_format_append_to_existing()
{
    char buffer[256] = "PREFIX";
    position_uncompressed_t pos = {
        .lat = 12.34567,
        .lon = 23.4567,
        .symbol_table = '#',
        .symbol = 'D',
        .messaging_capable = false,
        .with_timestamp = false};

    int ret = position_format_uncompressed(buffer, &pos);
    assert_equal_int(ret, 0, "append to existing success");
    assert_string(buffer, "PREFIX!1220.74N#02327.40ED", "append to existing output");
}

#endif