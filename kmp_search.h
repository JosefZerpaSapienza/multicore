// Adapted from: 
// https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
//
// C program for implementation of KMP pattern searching
// algorithm
//
  
// Fills a given array of int, with indexes where matches
// of pat[] are found in txt[].
// Returns -1 on failure, the count of matches on success.
// pat: pattern string; m: its size
// txt: text string; n: its size
int KMPSearch(char* pat, int m, char* txt, int n, int *lps)
{
    int i = 0; // index for txt[]
    int j = 0; // index for pat[]
    while (i < n) {
        if (pat[j] == txt[i]) {
            j++;
            i++;
        }
  
        if (j == m) {
            return i - j;

            j = lps[j - 1];
        }
  
        // mismatch after j matches
        else if (i < n && pat[j] != txt[i]) {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }

    free(lps);
    return -1;
}
  
// Fills lps[] for given patttern pat[0..M-1]
void computeLPS(char* pat, int M, int* lps)
{
    // length of the previous longest prefix suffix
    int len = 0;
  
    lps[0] = 0; // lps[0] is always 0
  
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar
            // to search step.
            if (len != 0) {
                len = lps[len - 1];
  
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}
