#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "kmp_search.h"
#include "overlapped_read.h"

#define USAGE "\n USAGE: ./a.out <n_threads> \
	<file.txt> <pattern1> [pattern2] ...\
	\n\n Remember, the program just finds any pattern at any \
place in the text.\n The reported found pattern is not \
exclusively the first pattern in the text.\n"
#define TCP_BUFFER_SIZE 1024

int main (int argc, char** argv) 
{
  char text_buffer[TCP_BUFFER_SIZE];
  int text_buffer_size = TCP_BUFFER_SIZE;
  int fd;
  int index;
  int p_index;
  int l_index;
  int j;
  int c;
  int status;
  int finished = 0;
  int final_i = -1, final_j;

  if (argc < 4) 
  {
    printf(USAGE);
    return 1;
  }

  // Threads to be launched.
  int thread_count = atoi(argv[1]);
  if(thread_count == 0) 
  {
    perror("Invalid thread count.\n");
    return -1;
  }
  // File to be searched.
  char *filename = argv[2];
  // Number of patterns.
  int n_patterns = argc - 3;
  // Array of patterns to be searched.
  char **patterns = malloc(sizeof(char *) * n_patterns);
  // Array of pattern lengths.
  int *lengths = malloc(sizeof(int) * n_patterns);
  // Array of lps[] for each pattern
  int **lps = malloc(sizeof(int *) * n_patterns);
  // Length of the lengthiest pattern
  int max_length = 0;

  // Populate arrays of patterns, lengths, lps, and status.
  for (int i = 0; i < n_patterns; i++) 
  {
    patterns[i] = argv[3 + i];
    lengths[i] = strlen(patterns[i]);
    max_length = (lengths[i] > max_length ? lengths[i] : max_length);
    lps[i] = malloc(sizeof(int) * lengths[i]);
    computeLPS(patterns[i], lengths[i], lps[i]);
  }

  // Open stream.
  fd = open((const char*) filename, O_RDONLY);
  if (fd == -1) 
  {
    perror("\nError opening file.\n");
    return -1;
  }

  // Iteratively read from stream.
  index = 0;
  # pragma omp parallel num_threads(thread_count) \
      private(text_buffer, p_index, l_index, j, c, status)
  while(!finished)
  {
    # pragma omp critical
    do
    {
      c = overlapped_read_into_buffer(
	fd, text_buffer, text_buffer_size, max_length - 1, &index);
      p_index = index;
    } while(0); 
    // Check output of read
    if(c <= 0) 
    { 
      finished = 1; 
      if (c == -1) { perror("\nError reading file.\n"); }
    }
  
    // Apply search for each pattern.
    j = 0;
    l_index = -1;
    status = 0;
    while( (l_index == -1) && (j < n_patterns) ) 
    {
      KMPsearch(patterns[j], lengths[j], text_buffer, c, lps[j], &status, &l_index); 

      if(l_index != -1) 
      { 
        finished = 1; 
        final_i = p_index - c + l_index; 
        final_j = j; 
      } 
      else { j++; }
    }
  }

  // Print results.
  if (final_i == -1) 
  {
    printf("\nPatterns not found.\n");
  } 
  else 
  {
    printf("\nPattern %s found at index: %d.\n", patterns[final_j], final_i);
  }
  
  // Clean.
  close(fd);
  free(patterns);
  free(lengths);
  for(int i = 0; i < n_patterns; i++) 
  {
    free(lps[i]);
  }
  free(lps);

  return 0;
}
