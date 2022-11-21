#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_STDIN_BUFFER_SIZE 64

int main(int argc, char** argv) {
  char data[MAX_STDIN_BUFFER_SIZE];

  int total_bytes_read = 0;
  int nbytes_read      = 0;

  while (1) {
    nbytes_read = read(0, data + total_bytes_read, MAX_STDIN_BUFFER_SIZE - total_bytes_read);
    total_bytes_read += nbytes_read;

    if (nbytes_read == 0 || total_bytes_read >= MAX_STDIN_BUFFER_SIZE) {
      data[total_bytes_read] = '\0';

      // flush stdin so it doesn't pollute the terminal
      int c;
      while ((c = getchar()) != '\n' && c != EOF) {
      }
      break;
    }

    if (nbytes_read < 0) {
      fprintf(stderr, "[ERROR] read failed: %s\n", strerror(errno));
      return 1;
    }
  }

  printf("%s\n", data);

  return 0;
}