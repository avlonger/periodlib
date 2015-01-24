#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <ctime>
#include <iostream>

#include <map>
#include <string>

#include "period.h"


typedef int (* algorithm)(const char *, int);


void usage(const char * program_name) {
    printf("Usage: %s [options] ALGORITHM\n", program_name);
    printf("Calculate some value for words range\n");
    printf("Algorithms:\n");
    printf("MAXBORDERLESS_DBF           find longest borderless subword\n");
    printf("                            using dictionary of basic factors\n");
    printf("MAXBORDERLESS_DBF_HASH      find longest borderless subword\n");
    printf("                            using dictionary of basic factors and hashtable\n");
    printf("MAXBORDERLESS_NAIVE         find longest borderless subword naively\n");
    printf("MAXBORDERLESS_SUPER_NAIVE   find longest borderless subword naively\n");
    printf("MAXBORDERLESS_BORDER        find longest borderless subword\n");
    printf("                            using border-related heuristics\n");
    printf("MAXBORDERLESS_BORDER_FAST   find longest borderless word when building border array\n");
    printf("MAXBORDERLESS_DBF_LONG_STEPS find longest borderless word using dbf and long cuts\n");
    printf("\n");
    printf("Options:\n");
    printf(" -b  Measure time for random words of minimal length\n");
    printf(" -e  Measure time for random words of maximal length\n");
    printf(" -s  Word length step\n");
    printf(" -a  Alphabet size (default: 2)\n");
    printf(" -r  Random words count\n");
    printf(" -t  Trace: print results for all generated words\n");
}


void fill_rand_chars(char *str, int n, int alphabet_size, char minimal_char) {
    for (int i = 0; i < n; ++i)
        str[i] = (char) (rand() % alphabet_size) + minimal_char;
}


void measure(std::map<std::string, algorithm> algorithms, char alphabet, char minimal_char, int length, int words_count, bool trace) {

    // fill words_count strings with random letters
    char * strings = (char *) calloc(length * words_count, sizeof(char));
    fill_rand_chars(strings, length * words_count, alphabet, minimal_char);

    std::cout.precision(8);

    for (std::map<std::string, algorithm>::iterator it = algorithms.begin(); it != algorithms.end(); ++it) {
        clock_t begin = clock();

        for (int i = 0; i < words_count; ++i) {
            int result = it->second(strings + i * length, length);
            if (trace) {
                std::cout.write(strings + i * length, length);
                std::cout << ' ' << result << std::endl;
            }
        }

        std::cout << it->first << ' ' << length << ' ' << std::fixed << double(clock() - begin) / CLOCKS_PER_SEC / words_count << std::endl;
    }

    free(strings);
}


int main(int argc, char** argv) {
    int c = 0;
    opterr = 1;
    char alphabet = 2;
    int start = 2;
    int finish = 10;
    int step = 1;
    bool trace = false;
    int words_count = 1;
    char minimal_char = 1;
    while ((c = getopt(argc, argv, "tb:e:s:a:r:")) != -1)
        switch (c)
        {
            case 'a':
                alphabet = (char) atoi(optarg);
                break;
            case 'b':
                start = atoi(optarg);
                break;
            case 'e':
                finish = atoi(optarg);
                break;
            case 's':
                step = atoi(optarg);
                break;
            case 't':
                trace = true;
                minimal_char = 'A';
                break;
            case 'r':
                words_count = atoi(optarg);
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

    std::map<std::string, algorithm> algorithms;
    for (int i = optind; i < argc; ++i) {
        if (strcmp(argv[i], "MAXBORDERLESS_DBF") == 0) {
            algorithms[std::string(argv[i])] = max_borderless_length_dbf;
        } else if (strcmp(argv[i], "MAXBORDERLESS_DBF_HASH") == 0) {
            algorithms[std::string(argv[i])] = max_borderless_length_dbf_hashtable;
        } else if (strcmp(argv[i], "MAXBORDERLESS_NAIVE") == 0) {
            algorithms[std::string(argv[i])] = max_borderless_length_naive;
        } else if (strcmp(argv[i], "MAXBORDERLESS_SUPER_NAIVE") == 0) {
            algorithms[std::string(argv[i])] = max_borderless_length_super_naive;
        } else if (strcmp(argv[i], "MAXBORDERLESS_BORDER_FAST") == 0) {
            algorithms[std::string(argv[i])] = max_borderless_length_border_fast;
        } else if (strcmp(argv[i], "MAXBORDERLESS_BORDER") == 0) {
            algorithms[std::string(argv[i])] = max_borderless_length_border;
        } else if (strcmp(argv[optind], "MAXBORDERLESS_DBF_LONG_STEPS") == 0) {
            algorithms[std::string(argv[i])] = max_borderless_length_dbf_long_steps;
        } else {
            usage(argv[0]);
            return -1;
        }
    }

    if (algorithms.size() == 0) {
        usage(argv[0]);
        return -1;
    }
    for (int length = start; length < finish + 1; length += step) {
        measure(algorithms, alphabet, minimal_char, length, words_count, trace);
    }

    return 0;
}
