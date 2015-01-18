#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>

#include "period.h"


// some buffers
char * CHAR_BUFFER;
int * INT_BUFFER;
int (* FUNCTION)();
char ALPHABET = 2;
char MINIMAL_CHAR = 1;
int LENGTH = 2;
bool TRACE = false;
bool NORMALIZE = false;
bool PRINT_TOTAL = false;
int PREFIX = 0;
int PREFIX_LENGTH = 0;


void usage(const char * program_name) {
    printf("Usage: %s [options] ALGORITHM\n", program_name);
    printf("Calculate some value for words range\n");
    printf("Algorithms:\n");
    printf("MAXBORDERLESS_DBF           find longest borderless subword\n");
    printf("                            using dictionary of basic factors\n");
    printf("MAXBORDERLESS_NAIVE         find longest borderless subword naively\n");
    printf("\n");
    printf("Options:\n");
    printf(" -l  Find average value for all words of given length\n");
    printf(" -a  Alphabet size (default: 2)\n");
    printf(" -t  Trace: print results for all generated words\n");
    printf(" -n  Normalize (divide result by total words count)\n");
    printf(" -c  Print count of processed words\n");
    printf(" -f  Fixed prefix length\n");
    printf(" -p  Prefix decimal representation\n");
    printf(" -r  Read from file (\\n separated words)");
    printf(" -m  Measure time");
}


uint64_t do_for_all_words(int position) {
    uint64_t total = 0;
    int value = 0;
    for (char i = MINIMAL_CHAR; i < ALPHABET; ++i) {
        CHAR_BUFFER[position] = i;

        if (position < LENGTH - 1) {
            total += do_for_all_words(position + 1);
        } else {
            value = FUNCTION();

            if (TRACE) {
                printf("%s\t%d\n", CHAR_BUFFER, value);
            }

            total += value;
        }
    }

    return total;
}


int longest_borderless_dbf() {
    return max_borderless_length_dbf(CHAR_BUFFER, LENGTH);
}


int longest_borderless_naive() {
    return max_borderless_length_naive(CHAR_BUFFER, LENGTH);
}


int main(int argc, char** argv) {
    int c = 0;
    opterr = 1;
    while ((c = getopt(argc, argv, "tncl:a:p:f:")) != -1)
        switch (c)
        {
            case 'a':
                ALPHABET = (char) atoi(optarg);
                break;
            case 'l':
                LENGTH = atoi(optarg);
                break;
            case 't':
                TRACE = true;
                break;
            case 'n':
                NORMALIZE = true;
                break;
            case 'c':
                PRINT_TOTAL = true;
                break;
            case 'p':
                PREFIX = atoi(optarg);
                break;
            case 'f':
                PREFIX_LENGTH = atoi(optarg);
                break;
            case '?':
            case ':':
                return -1;
            default:
                usage(argv[0]);
                return -1;
        }

    if (TRACE) {
        MINIMAL_CHAR = 'A';
    }

    if (optind >= argc) {
        usage(argv[0]);
        return -1;
    }

    if (strcmp(argv[optind], "MAXBORDERLESS_DBF") == 0) {
        FUNCTION = longest_borderless_dbf;
    } else if (strcmp(argv[optind], "MAXBORDERLESS_NAIVE") == 0) {
        FUNCTION = longest_borderless_naive;
    } else {
        usage(argv[0]);
        return -1;
    }

    CHAR_BUFFER = (char *) calloc((size_t) LENGTH + 1, sizeof(char));
    INT_BUFFER = (int *) calloc((size_t) LENGTH + 1, sizeof(int));
    char * temp_buffer = (char *) calloc((size_t) PREFIX_LENGTH + 1, sizeof(char));

    int filled_chars = 0;
    while (PREFIX > 0) {
        temp_buffer[filled_chars++] = (char) PREFIX % ALPHABET;
        PREFIX /= ALPHABET;
    }

    for (int i = 0; i < PREFIX_LENGTH; ++i) {
        CHAR_BUFFER[i] = temp_buffer[PREFIX_LENGTH - i - 1] + MINIMAL_CHAR;
    }

    ALPHABET += MINIMAL_CHAR;

    uint64_t answer = do_for_all_words(PREFIX_LENGTH);

    double count = pow((double) (ALPHABET - MINIMAL_CHAR), (double) LENGTH);
    if (NORMALIZE) {
        printf("%.10f\n", (double) answer / count);
    } else {
        printf("%llu\n", answer);
    }

    if (PRINT_TOTAL) {
        printf("%d\n", (int)count);
    }

    free(CHAR_BUFFER);
    free(INT_BUFFER);
    return 0;
}
