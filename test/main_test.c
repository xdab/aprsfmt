#include "test.h"
#include <limits.h>
#include "test_position.h"

int main(void)
{
    begin_suite();

    begin_module("Position");
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

    int failed = end_suite();

    return failed ? 1 : 0;
}
