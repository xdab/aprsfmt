#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include "sum.h"

struct arguments
{
    int a;
    int b;
};

static struct argp_option options[] = {
    {"a", 'a', "INT", 0, "First number (default: 0)", 0},
    {"b", 'b', "INT", 0, "Second number (default: 0)", 0},
    {0, 0, 0, 0, 0, 0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
    case 'a':
        arguments->a = atoi(arg);
        break;
    case 'b':
        arguments->b = atoi(arg);
        break;
    case ARGP_KEY_END:
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    struct arguments args = {0};
    struct argp argp = {options, parse_opt, 0, "aprsfmt", 0, 0, 0};
    argp_parse(&argp, argc, argv, 0, 0, &args);

    int c = sum(args.a, args.b);
    printf("%d\n", c);

    return 0;
}
