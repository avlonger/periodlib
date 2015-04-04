#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>

#include "period.h"


// some buffers
char * CHAR_BUFFER;
int * INT_BUFFER;


void usage(const char * program_name) {
    printf("Usage: %s [options] ALGORITHM\n", program_name);
    printf("Calculate some value for words range\n");
    printf("Algorithms:\n");
    printf("MAXBORDERLESS_SUBWORD        find longest borderless subword\n");
    printf("MAXBORDERLESS_PREFIX         find longest borderless prefix\n");

    printf("\n");
    printf("Options:\n");
    printf(" -b  Word length start (default: 10)\n");
    printf(" -f  Word length finish (default: 11)\n");
    printf(" -a  Alphabet size (default: 2)\n");
    printf(" -s  Check difference after given count of words\n");
    printf(" -d  Stop calculations when difference after step become less than given number\n");
    printf(" -t  Trace estimation process\n");
    printf(" -r  Random seed\n");
}


void fill_rand_chars(char *str, int n, int alphabet_size) {
    for (int i = 0; i < n; ++i)
        str[i] = (char) (rand() % alphabet_size);
}


int main(int argc, char** argv) {
    char alphabet = 2;
    int step = 100000;
    int start = 10;
    int finish = 11;
    double target_diff = 0.00000001;
    bool trace = false;
    
    int c = 0;
    opterr = 1;
    while ((c = getopt(argc, argv, "ta:b:f:s:d:r:")) != -1)
        switch (c)
        {
            case 'a':
                alphabet = (char) atoi(optarg);
                break;
            case 'b':
                start = atoi(optarg);
                break;
            case 'f':
                finish = atoi(optarg);
                break;
            case 's':
                step = atoi(optarg);
                break;
            case 'd':
                target_diff = atof(optarg);
                break;
            case 'r':
                srand((unsigned int) atoi(optarg));
                break;
            case 't':
                trace = true;
                break;
            case '?':
            case ':':
                return -1;
            default:
                usage(argv[0]);
                return -1;
        }

    if (optind >= argc) {
        usage(argv[0]);
        return -1;
    }

    int (* target_function)(const char *, int);

    if (strcmp(argv[optind], "MAXBORDERLESS_SUBWORD") == 0) {
        target_function = max_borderless_length;
    } else if (strcmp(argv[optind], "MAXBORDERLESS_PREFIX") == 0) {
        target_function = max_borderless_prefix;
    } else {
        usage(argv[0]);
        return -1;
    }

    CHAR_BUFFER = (char *) calloc((size_t) finish + 1, sizeof(char));

    for (int length = start; length < finish; ++length) {
        CHAR_BUFFER[length] = 0;

        double current_diff = target_diff + 1;
        uint64_t total = 0;
        uint64_t count = 0;
        double old_value = 0.0;
        double new_value = 0.0;

        while (current_diff > target_diff) {

            for (int i = 0; i < step; ++i) {
                fill_rand_chars(CHAR_BUFFER, length, alphabet);
                total += target_function(CHAR_BUFFER, length);
            }

            count += step;
            new_value = (double) total / (double) count;
            current_diff = fabs(new_value - old_value);
            old_value = new_value;

            if (trace) {
                printf("%d %d %llu %llu %.10f\n", alphabet, length, count, total, new_value);
            }

        }

        printf("%d %d %llu %llu %.10f\n", alphabet, length, count, total, new_value);
        fflush(stdout);
    }


    free(CHAR_BUFFER);
    return 0;
}
