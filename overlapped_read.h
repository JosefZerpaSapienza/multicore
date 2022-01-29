// Read from fd, and write into buffer, handling the ovelapping
// of size overlap between sequential reads.
// index - tells the postion on the file
//         At first call should be 0.
//
// Returns the number of characters written (including overlapping),
// -1 on failure.

int overlapped_read_into_buffer(int fd, char *buffer,
                int buffer_size, int overlap, int *index)
{
  // Tells whether to perform overlapping
  int perform = 0;
  if (*index > buffer_size - 1) { perform = 1; }

  // Perform overlapping only after first read
  if(perform) {
    // Copy final characters into the beginning positions.
    for(int i = 0; i < overlap; i++)
    {
      buffer[i] = buffer[buffer_size - 1 - i];
    }
    // Slide buffer pointer.
    buffer += overlap;
    // Decrement buffer size.
    buffer_size -= overlap;
  }

  // Read from fd and write buffer
  int c = read(fd, buffer, buffer_size);
  
  // Update c and *index
  if (c > 0 )
  {
    *index = *index + c;
    if (perform)
    {
      c += overlap;
    }
  }


  return c;
}
