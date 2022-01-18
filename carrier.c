#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
// #include "linear_kmp_search.h"
#include "linear_ac_search.h"

#define USAGE "\n USAGE: ./a.out [file.txt] [pattern1] [pattern2] ...\n\n"
#define TCP_BUFFER_SIZE 1024

int main (int argc, char** argv) 
{
  char text_buffer[TCP_BUFFER_SIZE];
  int text_buffer_size = TCP_BUFFER_SIZE;
  int fd;
  int index = -1;
  int c, j, iteration;

  if (argc < 3) 
  {
    printf(USAGE);
    return 1;
  }

  // File to be searched.
  char *filename = argv[1];
  // Number of patterns.
  int n_patterns = argc - 2;
  // Array of patterns to be searched.
  char **patterns = malloc(sizeof(char *) * n_patterns);
  /*
  // Array of pattern lengths.
  int *lengths = malloc(sizeof(int) * n_patterns);
  // Array of lps[] for each pattern
  int **lps = malloc(sizeof(int *) * n_patterns);
  // Array of 'status' indexes storing progression of the kmp search.
  int *status = malloc(sizeof(int) * n_patterns);
  */

  int *searchState;
  int zero = 0;
  searchState = &zero;
  char **word;

  // Populate arrays of patterns, lengths, lps, and status.
  for (int i = 0; i < n_patterns; i++) 
  {
    patterns[i] = argv[2 + i];
    /*
    lengths[i] = strlen(patterns[i]);
    lps[i] = malloc(sizeof(int) * lengths[i]);
    computeLPS(patterns[i], lengths[i], lps[i]);
    status[i] = 0;
    */
  }

  // Matching Automaton for A-C search
  struct MatchingAutomaton *ma;
  ma = getMatchingAutomaton(patterns, n_patterns);

  // Open stream.
  fd = open((const char*) filename, O_RDONLY);
  if (fd == -1) 
  {
    perror("\nError opening file.\n");
    return -1;
  }

  // Iteratively read from stream.
  for(iteration = 0; \
	(index == -1) && (c = read(fd, text_buffer, text_buffer_size) > 0); \
	iteration++)
  {
    // Apply search for each pattern.
    for (j = 0; (index == -1) && (j < n_patterns); j++) 
    {
      index = ACsearch(patterns, n_patterns, text_buffer, text_buffer_size, \
		      ma, searchState, word);
    }
  }
  iteration--;
  j--;

  // Check error reading file
  if (c == -1) {
    perror("\nError reading file.\n");
  }
  
  // Print results.
  if (index == -1) 
  {
    printf("\nPatterns not found.\n");
  } 
  else 
  {
    index = index + text_buffer_size * iteration;
    printf("\nPattern %s found at index: %d.\n", *word, index);
  }
  
  // Clean.
  close(fd);
  free(patterns);
  /*
  free(lengths);
  for(int i = 0; i < n_patterns; i++) 
  {
    free(lps[i]);
  }
  free(lps);
  free(status);
  */
  free(ma);
  return 0;
}
