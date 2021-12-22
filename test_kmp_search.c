#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmp_search.h"

int main (void) 
{
    char txt[] = "CABABDABACDABABCABABCABABBABAABBABABABCABAB";
    char pat[] = "ABABCABAB";
    int m = strlen(pat);
    int n = strlen(txt);
    int i;
    int *lps = malloc(sizeof(int) * m);

    computeLPS(pat, m, lps);

    i = KMPSearch(pat, m, txt, n, lps);
    if (i == -1) {
       printf("No match found.\n");
    } else {
      printf("Pattern found at index: %d.\n", i);
    }

    free(lps);
    return 0;
}
