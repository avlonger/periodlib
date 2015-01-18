#include <stdlib.h>
#include <string.h>

void border(const char * text, int * border, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    border[0] = 0;
    int j = 0;
    for (int i = 1; i < n; ++i) {
        j = border[i - 1];
        while (j > 0 && text[i] != text[j]) {
            j = border[j - 1];
        }
        if (text[i] == text[j])
            j++;
        border[i] = j;
    }
}

int kmp(const char * pattern, const char * text, int * output){
    size_t m = strlen(pattern), n = strlen(text);
    int * pattern_border = (int *) calloc(m, sizeof(int));
    border(pattern, pattern_border, n);
    int i = 0, j = 0, k = 0;
    while (j < n) {
        while (i > 0 && pattern[i] != text[j])
            i = pattern_border[i - 1];
        i++;
        j++;
        if (i >= m) {
            output[k++] = j - i;
            i = pattern_border[i];
        }
    }
    free(pattern_border);
    return k;
}
