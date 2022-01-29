// Adapted from: 
// https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
//
// C program for implementation of KMP pattern searching
// algorithm
//
   
// Fills lps array for given pattern pat[0..M-1].
void computeLPS(char* pat, int size, int* lps)
{
    // length of the previous longest prefix suffix
    int len = 0;
  
    lps[0] = 0; // lps[0] is always 0
  
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < size) {
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

// Returns -1 when no match is found, alternatively, the
// index at which the pattern is found.
// pat: pattern string; m: its size
// txt: text string; n: its size
// lps: the lps array status: records the status of the search on txt based on lps
void KMPsearch(char* pat, int m, char* txt, int n, int *lps, int *status, int *index)
{
    *index = 0; // index for txt[]
    // int status = 0; // index for pat[]

    while (*index < n) {
        if (pat[*status] == txt[*index]) {
            *status = *status + 1;
            *index = *index + 1;
        }
  
        if (*status == m) {
	    // found match
            *index = *index - *status;
            *status = lps[*status - 1];

	    return;
        }
  
        // mismatch after status matches
        else if (*index < n && pat[*status] != txt[*index]) {
            // Do not match lps[0..lps[status-1]] characters,
            // they will match anyway
            if (*status != 0)
                *status = lps[*status - 1];
            else
                *index = *index + 1;
        }
    }

  *index = -1;
}

