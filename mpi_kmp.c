#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <mpi.h>
#include "kmp_search.h"
#include "overlapped_read.h"

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
  int g_index = 0;
  int c = 1;
  int j; 
  int send_rank, recv_rank;
  int comm_sz, my_rank;

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
  // Lenght of the lenghiest pattern (for search-buffer overlapping)
  int max_length = 0;
  int zero = 0;

  // Populate arrays of patterns, lengths, lps, and status.
  for (int i = 0; i < n_patterns; i++) 
  {
    patterns[i] = argv[2 + i];
    lengths[i] = strlen(patterns[i]);
    max_length = (lengths[i] > max_length ? lengths[i] : max_length);
    lps[i] = malloc(sizeof(int) * lengths[i]);
    computeLPS(patterns[i], lengths[i], lps[i]);
  }

  // Start MPI World
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  // Master: read stream, send buffers, and print results.
  if(my_rank == 0) {

    // Open stream.
    fd = open((const char*) filename, O_RDONLY);
    if (fd == -1) 
    {
      perror("\nError opening file.\n");
      return -1;
    }
 
    // Iteratively read from stream.
    while(index == -1 && c > 0) 
    {
      // Read and send data
      for (send_rank = 1; send_rank < comm_sz &&  
  	(c = overlapped_read_into_buffer(
 		fd, text_buffer, text_buffer_size, max_length - 1, &g_index)) > 0; 
	send_rank++)
      {
	// Send number of chars read.
	MPI_Send(&c, 1, MPI_INT, send_rank, 0, MPI_COMM_WORLD);

	// Send buffer.
        MPI_Send(text_buffer, c, MPI_CHAR, send_rank, 0, MPI_COMM_WORLD);
      }

      // Receive result
      for(recv_rank = 1; recv_rank < send_rank && index == -1; ) 
      {
        MPI_Recv(&index, 1, MPI_INT, recv_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if (index != -1) 
	{
          MPI_Recv(&j, 1, MPI_INT, recv_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	} 
	else { recv_rank++; }
      }
    }

    //Search finished.

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
      g_index = g_index - c 
	      - ((text_buffer_size - (max_length - 1)) * ((send_rank - 1) - recv_rank));
      printf("\nPattern %s found at index: %d.\n", patterns[j], g_index + index);
    }
 
    close(fd);
  }

  // Slave: Receive data, perform search, send result.
  else {
    while(1) 
    {
      // Receive buffer size.
      MPI_Recv(&c, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // Receive buffer.
      MPI_Recv(text_buffer, c, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // Apply search for each pattern.
      for(j = 0; (index == -1) && (j < n_patterns); j++)
      {
        KMPsearch(patterns[j], lengths[j], text_buffer, c, lps[j], &zero, &index);
      }
      j--;

      // Send Results.
      MPI_Send(&index, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      if (index != -1) 
      {
        MPI_Send(&j, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
    }
  }

  MPI_Abort(MPI_COMM_WORLD, 0);

  // Clean.
  free(patterns);
  free(lengths);
  for(int i = 0; i < n_patterns; i++) 
  {
    free(lps[i]);
  }
  free(lps);

  return 0;
}

