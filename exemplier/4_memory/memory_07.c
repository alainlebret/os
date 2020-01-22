#include <stdio.h>
#include <stdlib.h>

int f() {
  return 1;
}

int main(int argc, char *argv[]) {
  printf("location of main code : %p\n", main);
  printf("location of f() code : %p\n", f);
  printf("location of stack: %p\n", &x);
  printf("location of heap : %p\n", malloc(100e8));
  int x = 10;
  printf("location of stack: %p\n", &x);
  return 0;
}
