#include "test.h"
#include <limits.h>
#include "test_position_uncompressed.h"
#include "test_position_compressed.h"

int main(void)
{
    begin_suite();

    begin_module("Position Uncompressed");
    test_position_format_basic();
    test_position_format_messaging_no_timestamp();
    test_position_format_timestamp_no_messaging();
    test_position_format_timestamp_with_messaging();
    test_position_format_southern_western_hemisphere();
    test_position_format_invalid_latitude();
    test_position_format_invalid_longitude();
    test_position_format_null_buffer();
    test_position_format_null_pos();
    test_position_format_append_to_existing();
    end_module();

    begin_module("Position Compressed");
    test_compressed_format_basic();
    test_compressed_format_course_speed();
    test_compressed_format_course_speed_zero();
    test_compressed_format_radio_range();
    test_compressed_format_altitude();
    test_compressed_format_equator_prime_meridian();
    test_compressed_format_north_pole();
    test_compressed_format_south_pole();
    test_compressed_format_max_longitude();
    test_compressed_format_min_longitude();
    test_compressed_format_append_to_existing();
    test_compressed_format_nmea_sources();
    test_compressed_format_compression_origins();
    test_compressed_format_gps_fix_status();
    test_compressed_format_dti_no_messaging_no_timestamp();
    test_compressed_format_dti_messaging_no_timestamp();
    test_compressed_format_dti_no_messaging_with_timestamp();
    test_compressed_format_dti_messaging_with_timestamp();
    test_compressed_format_invalid_latitude();
    test_compressed_format_invalid_longitude();
    test_compressed_format_null_buffer();
    test_compressed_format_null_pos();
    end_module();

    int failed = end_suite();

    return failed ? 1 : 0;
}
