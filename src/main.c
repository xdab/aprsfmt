#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ax25.h>
#include <tnc2.h>
#include <common.h>
#include <arguments.h>

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

    // Copy info
    packet.info_len = strlen(args.info);
    memcpy(packet.info, args.info, packet.info_len);

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