#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "kmp_search.h"

#define TCP_BUFFER_SIZE 1024

int main (void) 
{
  char text_buffer[TCP_BUFFER_SIZE];
  int text_buffer_size = TCP_BUFFER_SIZE;
  int fd;
  int index = -1;
  int c, j, iteration;

  // TODO: Get values from command line.
  char *filename = "datastream.txt";
  char *patterns[] = { "Hello", "ip", "name", "address", "password" };
  int p = 5;
  // Array of pattern lengths
  int *lengths = malloc(sizeof(int) * p);
  // Array of lps[] for each pattern
  int **lps = malloc(sizeof(int *) * p);

  // Allocate and populate lengths and lps arrays.
  for (int i = 0; i < p; i++) 
  {
    lengths[i] = strlen(patterns[i]);
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

  // TODO: Consider matches that cross
  //       over different sections of read.

  // Iteratively read from stream.
  for(iteration = 0; \
	(index == -1) && (c = read(fd, text_buffer, text_buffer_size) > 0); \
	iteration++)
  {
    // Apply search for each pattern.
    for (j = 0; (index == -1) && (j < p); j++) 
    {
      index = KMPsearch(patterns[j], lengths[j], \
		      text_buffer, text_buffer_size, lps[j]);
      printf("dbg: it: %d pat: %s index: %d\n", iteration, patterns[j], index);
    }
  }
  iteration--;
  j--;

  // Check error reading file
  if (c == -1) {
    perror("\nError reading file.\n");
  }
  
  // TODO: Print indexes of found matches.
  // Print results.
  if (index == -1) 
  {
    printf("Pattern not found.\n");
  } 
  else 
  {
    index = index + text_buffer_size * iteration;
    printf("Pattern %s found at index: %d.\n", patterns[j], index);
  }
  
  // Clean.
  close(fd);
  free(lengths);
  for(int i = 0; i < p; i++) 
  {
    free(lps[i]);
  }
  free(lps);

  return 0;
}
