#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmp_search.h"

int main (void) 
{
    char txt[] = "ABABDABACDABABCABAB";
    char pat[] = "ABABCABAB";
    int m = strlen(pat);
    int n = strlen(txt);
    int *matches = malloc(sizeof(int) * n);
    int c;

    c = KMPSearch(pat, m, txt, n, matches, n);
    if (c == -1) {
       printf("Error: Matches array too small!\n");
       return -1;
    }

    for (int i = 0; i < c; i++) {
        printf("%d\n", matches[i]);
    }

    free(matches);
    return 0;
}
