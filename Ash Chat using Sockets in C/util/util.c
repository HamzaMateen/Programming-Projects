#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void checkError(int return_value, char *error_msg) {
  if (return_value < 0) {
    perror(error_msg);
    exit(EXIT_FAILURE);
  }
}
