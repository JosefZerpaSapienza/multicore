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
  int index;
  int fd;
  int c, i;

  // TODO: Get patterns from command line.
  char *filename = "datastream.txt";
  // char *patterns[] = { "Hello", "ip", "name", "address", "password" };
  char *pattern = "Hello!";
  int m = strlen(pattern);
  int *lps = malloc(sizeof(int) * m);

  // Open stream.
  fd = open((const char*) filename, O_RDONLY);
  if (fd == -1) 
  {
    perror("\nError opening file.\n");
    return -1;
  }

  // TODO: Iteratively read from stream and 
  //      apply search for each pattern.
  //      !Remember: Consider matches that cross
  //      over different sections of read.

  // Build lps arrays for patterns.
  computeLPS(pattern, m, lps);

  // Iteratively read from stream.
  for(i = 0; (c = read(fd, text_buffer, text_buffer_size)) > 0; i++)
  {
    // Apply search for each pattern.
    index = KMPsearch(pattern, m, text_buffer, c, lps);
  }

  // Check error reading file
  if (c == -1) {
    perror("\nError reading file.\n");
  }
  
  // TODO: Print indexes of found matches.
  // Print results.
  if (index == -1) 
  {
    printf("Pattern not found.\n");
  } else {
    printf("Pattern found at index: %d.\n", index);
  }
  
  // Clean.
  close(fd);
  free(lps);

  return 0;
}
