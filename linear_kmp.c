#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "kmp_search.h"

#define USAGE "\n USAGE: ./a.out [file.txt] [pattern1] [pattern2] ...\
	\n\n Remember, the program just finds any pattern at any \
place in the text.\n The reported found pattern is not \
exclusively the first pattern in the text.\n"
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
  // Array of pattern lengths.
  int *lengths = malloc(sizeof(int) * n_patterns);
  // Array of lps[] for each pattern
  int **lps = malloc(sizeof(int *) * n_patterns);
  // Array of 'status' indexes storing progression of the kmp search.
  int *status = malloc(sizeof(int) * n_patterns);

  // Populate arrays of patterns, lengths, lps, and status.
  for (int i = 0; i < n_patterns; i++) 
  {
    patterns[i] = argv[2 + i];
    lengths[i] = strlen(patterns[i]);
    lps[i] = malloc(sizeof(int) * lengths[i]);
    computeLPS(patterns[i], lengths[i], lps[i]);
    status[i] = 0;
  }

  // Open stream.
  fd = open((const char*) filename, O_RDONLY);
  if (fd == -1) 
  {
    perror("\nError opening file.\n");
    return -1;
  }
  
  // Start search.
  struct timespec begin, end;
  clock_gettime(CLOCK_REALTIME, &begin);


  // Iteratively read from stream.
  for(iteration = 0; \
	(index == -1) && (c = read(fd, text_buffer, text_buffer_size) > 0); \
	iteration++)
  {
    // Apply search for each pattern.
    for (j = 0; (index == -1) && (j < n_patterns); j++) 
    {
	    int zero = 0;
      KMPsearch(patterns[j], lengths[j], \
	text_buffer, text_buffer_size, lps[j], &(status[j]), &index);
    }
  }
  iteration--;
  j--;

  // End search.
  clock_gettime(CLOCK_REALTIME, &end);
  long seconds = end.tv_sec - begin.tv_sec;
  long nanoseconds = end.tv_nsec - begin.tv_nsec;
  double elapsed = seconds + nanoseconds*1e-9;

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
    printf("\nPattern %s found at index: %d.\n", patterns[j], index);
  }
  printf("\nTime spent searching: %f sec.\n", elapsed);
  
  // Clean.
  close(fd);
  free(patterns);
  free(lengths);
  for(int i = 0; i < n_patterns; i++) 
  {
    free(lps[i]);
  }
  free(lps);
  free(status);

  return 0;
}
