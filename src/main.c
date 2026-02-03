#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ax25.h>
#include <tnc2.h>
#include <common.h>
#include <arguments.h>
#include <position.h>

int main(int argc, char *argv[])
{
    int ret;
    struct arguments args = {0};
    struct argp argp = {options, parse_opt, 0, "aprsfmt", 0, 0, 0};
    argp_parse(&argp, argc, argv, 0, 0, &args);

    ax25_packet_t packet;
    ax25_packet_init(&packet);
    packet.control = 0x03;  // UI frame
    packet.protocol = 0xF0; // No layer 3 protocol

    // Parse source address
    buffer_t src_buf = {
        .data = (unsigned char *)args.source,
        .size = strlen(args.source),
        .capacity = strlen(args.source)};
    ret = tnc2_string_to_addr(&packet.source, &src_buf);
    nonnegative(ret, "tnc2_string_to_addr src");

    // Parse dest address
    buffer_t dst_buf = {
        .data = (unsigned char *)args.dest,
        .size = strlen(args.dest),
        .capacity = strlen(args.dest)};
    ret = tnc2_string_to_addr(&packet.destination, &dst_buf);
    nonnegative(ret, "tnc2_string_to_addr dst");

    // Parse path addresses
    if (args.path[0])
    {
        char *saveptr;
        char *token = strtok_r(args.path, ",", &saveptr);
        while (token != NULL && packet.path_len < AX25_MAX_PATH_LEN)
        {
            buffer_t path_buf = {
                .data = (unsigned char *)token,
                .size = strlen(token),
                .capacity = strlen(token)};
            ret = tnc2_string_to_addr(&packet.path[packet.path_len], &path_buf);
            nonnegative(ret, "tnc2_string_to_addr path");
            packet.path_len++;
            token = strtok_r(NULL, ",", &saveptr);
        }
    }

    // Build info string: if lat/lon provided, prepend position report
    char info_buf[MAX_INFO_LEN + 1] = {0};
    if (args.lat != 0.0 || args.lon != 0.0)
    {
        if (args.compressed)
        {
            position_compressed_t pos = {
                .lat = args.lat,
                .lon = args.lon,
                .symbol_table = args.symbol_table ? args.symbol_table : '/',
                .symbol = args.symbol ? args.symbol : '-',
                .has_course_speed = false,
                .has_radio_range = false,
                .has_altitude = false,
                .gps_fix = GPS_FIX_OLD,
                .nmea_source = NMEA_OTHER,
                .compression_origin = COMPRESS_COMPRESSED};

            ret = position_format_compressed(info_buf, &pos);
            nonnegative(ret, "position_format_compressed");
        }
        else
        {
            position_uncompressed_t pos = {
                .lat = args.lat,
                .lon = args.lon,
                .symbol_table = args.symbol_table ? args.symbol_table : '/',
                .symbol = args.symbol ? args.symbol : '-',
                .messaging_capable = args.messaging,
                .with_timestamp = args.timestamped};

            ret = position_format_uncompressed(info_buf, &pos);
            nonnegative(ret, "position_format_uncompressed");
        }

        // Append original info if provided
        if (args.info[0])
            strncat(info_buf, args.info, MAX_INFO_LEN - strlen(info_buf) - 1);
    }
    else
    {
        // Use info as-is if no position data
        strncpy(info_buf, args.info, MAX_INFO_LEN);
    }

    // Copy info
    if (info_buf[0])
    {
        packet.info_len = strlen(info_buf);
        memcpy(packet.info, info_buf, packet.info_len);
    }

    // Convert to TNC2
    char out_buf_data[512] = {0};
    buffer_t out_buf = {
        .data = (unsigned char *)out_buf_data,
        .size = 0,
        .capacity = sizeof(out_buf_data)};
    ret = tnc2_packet_to_string(&packet, &out_buf);
    nonnegative(ret, "tnc2_packet_to_string");

    printf("%.*s\n", out_buf.size, out_buf.data);
    return 0;
}