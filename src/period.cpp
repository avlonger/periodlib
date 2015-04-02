#include "period.h"

#include <stdlib.h>
#include <string.h>

#include "kmp.h"
#include "dbf.h"
#include "etc.h"


// check if x is element of arithmetic progression
bool is_element(int x, int first, int diff, int last) {
    if (x < first || x > last)
        return false;
    return x == first + ((x - first) / diff) * diff;
}

bool is_borderless(const char * text, DBF & dbf, int start, int end) {

    // This implementation uses LargePS queries only
    // and does not find actual period length
    int n = end - start;

    // check 1-length basic factors
    if (text[start] == text[end - 1]) {
        return false;
    }

    // check basic factors
    for (int k = 2; k < n; k <<= 1) {
        int half = k >> 1;
        int left_half_id = dbf.id(start, half);
        int right_half_id = dbf.id(end - half, half);

        // first occurrence of right half in left factor
        int first_in_left = dbf.succ_short(start, half, right_half_id);
        if (first_in_left < 0 || first_in_left > start + half)
            // no occurrences
            continue;

        // first occurrence of left half in right factor
        int first_in_right = dbf.succ_short(end - k, half, left_half_id);
        if (first_in_right < 0 || first_in_right > end - half)
            // no occurrences
            continue;

        // second occurrence of right half in left factor
        int second_in_left = dbf.succ_short(first_in_left + 1, half, right_half_id);

        // last occurrence of right half in left factor
        int last_in_left = first_in_left;
        if (second_in_left < 0 || second_in_left > start + half) {
            second_in_left = -1;
        } else {
            last_in_left = dbf.pred_short(start + half, half, right_half_id);
        }

        // second occurrence of left half in right factor
        int second_in_right = dbf.succ_short(first_in_right + 1, half, left_half_id);

        // last occurrence of left half in right factor
        int last_in_right = first_in_right;
        if (second_in_right < 0 || second_in_right > end - half) {
            second_in_right = -1;
        } else {
            last_in_right = dbf.pred_short(end - half, half, left_half_id);
        }

        int first1 = half + first_in_left - start;
        int diff1 = second_in_left - first_in_left;
        int last1 = half + last_in_left - start;

        int first2 = end - last_in_right;
        int diff2 = second_in_right - first_in_right;
        int last2 = end - first_in_right;

        // check different cases
        if (second_in_left == -1 && second_in_right == -1) {
            if (first1 == first2)
                return false;
        } else if (second_in_left == -1) {
            if (is_element(first1, first2, diff2, last2))
                return false;
        } else if (second_in_right == -1) {
            if (is_element(first2, first1, diff1, last1))
                return false;
        } else {
            if (is_element(first2, first1, diff1, last1) || is_element(last2, first1, diff1, last1))
                return false;
        }
    }

    return true;
}

int maximal_border(const char * text, DBF & dbf, int start, int end) {

    int n = end - start;

    int highest_power = 1 << highest_bit(n - 1);

    // at first try to find BordersLarger
    int second_from_start = dbf.succ(start + 1, highest_power, dbf.id(start, highest_power));
    if (second_from_start > -1 && second_from_start <= end - highest_power) {
        int d = second_from_start - start;
        int second_from_end = dbf.pred(end - highest_power - 1, highest_power, dbf.id(end - highest_power, highest_power));
        if (second_from_end >= start && end - highest_power - second_from_end == d) {
            return n - d;
        }
    }

    // check basic factors
    for (int k = highest_power; k > 1; k >>= 1) {
        int half = k >> 1;
        int left_half_id = dbf.id(start, half);
        int right_half_id = dbf.id(end - half, half);

        // first occurrence of right half in left factor
        int first_in_left = dbf.succ_short(start, half, right_half_id);
        if (first_in_left < 0 || first_in_left > start + half)
            // no occurrences
            continue;

        // first occurrence of left half in right factor
        int first_in_right = dbf.succ_short(end - k, half, left_half_id);
        if (first_in_right < 0 || first_in_right > end - half)
            // no occurrences
            continue;

        // second occurrence of right half in left factor
        int second_in_left = dbf.succ_short(first_in_left + 1, half, right_half_id);

        // last occurrence of right half in left factor
        int last_in_left = first_in_left;
        if (second_in_left < 0 || second_in_left > start + half) {
            second_in_left = -1;
        } else {
            last_in_left = dbf.pred_short(start + half, half, right_half_id);
        }

        // second occurrence of left half in right factor
        int second_in_right = dbf.succ_short(first_in_right + 1, half, left_half_id);

        // last occurrence of left half in right factor
        int last_in_right = first_in_right;
        if (second_in_right < 0 || second_in_right > end - half) {
            second_in_right = -1;
        } else {
            last_in_right = dbf.pred_short(end - half, half, left_half_id);
        }

        int first1 = half + first_in_left - start;
        int diff1 = second_in_left - first_in_left;
        int last1 = half + last_in_left - start;

        int first2 = end - last_in_right;
        int diff2 = second_in_right - first_in_right;
        int last2 = end - first_in_right;

        // check different cases
        if (second_in_left == -1 && second_in_right == -1) {
            if (first1 == first2)
                return first1;
        } else if (second_in_left == -1) {
            if (is_element(first1, first2, diff2, last2))
                return first1;
        } else if (second_in_right == -1) {
            if (is_element(first2, first1, diff1, last1))
                return first2;
        } else {
            if (is_element(first2, first1, diff1, last1)) {
                return first2;
            } else if (is_element(first1, first2, diff2, last2)) {
                return first1;
            }
        }
    }

    if (text[start] == text[end - 1])
        return 1;

    return 0;
}

int max_borderless_length_dbf(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }

    // at first check if word itself is borderless
    int * border_buffer = (int *) calloc(n, sizeof(int));
    border(text, border_buffer, n);
    if (border_buffer[n - 1] == 0) {
        return n;
    }

    DBF dbf(text, n);

    for (int i = border_buffer[n - 1]; i < n - 1; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            if (is_borderless(text, dbf, j, j + n - i)) {
                free(border_buffer);
                return n - i;
            }
        }
    }
    free(border_buffer);
    return 1;
}


int max_borderless_length_dbf_hashtable(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }

    // at first check if word itself is borderless
    int * border_buffer = (int *) calloc(n, sizeof(int));
    border(text, border_buffer, n);
    if (border_buffer[n - 1] == 0) {
        return n;
    }

    DBFHashTable dbf(text, n);

    for (int i = border_buffer[n - 1]; i < n - 1; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            if (is_borderless(text, dbf, j, j + n - i)) {
                free(border_buffer);
                return n - i;
            }
        }
    }
    free(border_buffer);
    return 1;
}


int max_borderless_length_dbf_long_steps(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }

    // at first check if word itself is borderless
    int * border_buffer = (int *) calloc(n, sizeof(int));
    border(text, border_buffer, n);
    if (border_buffer[n - 1] == 0) {
        return n;
    }
    free(border_buffer);

    DBF dbf(text, n);

    int max_len = 1;

    for (int i = 0; i < n >> 1 && n - i > max_len; ++i) {
        int border_length = 0;
        for (int j = n; max_len < j - i; j -= border_length) {
            border_length = maximal_border(text, dbf, i, j);
            if (border_length == 0)
                max_len = j - i;
        }
    }

    return max_len;
}

int max_borderless_length_border(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    int * border_buffer = (int *) calloc(n, sizeof(int));

    int max_len = 1;

    // for each word of length n
    // there is at least one maximal borderless subword
    // which starting at i <= n / 2, so i < n >> 1
    for (int i = 0; i < n >> 1 && n - i > max_len; ++i) {

        border(text + i, border_buffer, n - i);

        for (int j = n - i - border_buffer[n - i - 1]; j > max_len; j -= border_buffer[j - 1])
            if (border_buffer[j - 1] == 0)
                max_len = j;

    }
    free(border_buffer);
    return max_len;
}


int max_borderless_length_naive(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    int * border_buffer = (int *) calloc(n, sizeof(int));

    int max_len = 1;

    for (int i = 0; i < n - 1 && n - i > max_len; ++i) {

        border(text + i, border_buffer, n - i);

        for (int j = n - i; j > max_len; --j)
            if (border_buffer[j - 1] == 0)
                max_len = j;

    }
    free(border_buffer);
    return max_len;
}


int max_borderless_length_super_naive(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    int * border_buffer = (int *) calloc(n, sizeof(int));

    int max_len = 1;

    for (int i = 0; i < n - 1; ++i) {

        border(text + i, border_buffer, n - i);

        for (int j = n - i; j > i; --j)
            if (border_buffer[j - 1] == 0)
                max_len = j;

    }
    free(border_buffer);
    return max_len;
}


int max_borderless_length_border_fast(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    int * border_buffer = (int *) calloc(n, sizeof(int));

    int max_len = 1;

    // for each word of length n
    // there is at least one maximal borderless subword
    // which starting at k <= n / 2, so k < n >> 1
    for (int k = 0; k < n >> 1 && n - k > max_len; ++k) {

        const char * suffix = text + k;

        border_buffer[0] = 0;
        int j = 0;
        for (int i = 1; i < n - k; ++i) {
            j = border_buffer[i - 1];
            while (j > 0 && suffix[i] != suffix[j]) {
                j = border_buffer[j - 1];
            }
            if (suffix[i] == suffix[j])
                j++;
            border_buffer[i] = j;

            if (j == 0 && i + 1 > max_len) {
                max_len = i + 1;
            }
        }
    }
    free(border_buffer);
    return max_len;
}


int max_borderless_prefix(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    int * border_buffer = (int *) calloc(n, sizeof(int));

    int max_len = 1;

    border_buffer[0] = 0;
    int j = 0;
    for (int i = 1; i < n; ++i) {

        j = border_buffer[i - 1];

        while (j > 0 && text[i] != text[j])
            j = border_buffer[j - 1];

        if (text[i] == text[j])
            j++;

        border_buffer[i] = j;

        if (j == 0)
            max_len = i + 1;
    }
    free(border_buffer);
    return max_len;
}
