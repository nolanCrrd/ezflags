#include "ezflags.h"
#include "test.h"
#include "unistd.h"

void print_flag_test() {
  char found = 0;
  flag_t flag = {.name = "r", .found = &found, .params_count = 1, NULL};

  print_flag(flag);
}

void print_test() { print_flag_test(); }
