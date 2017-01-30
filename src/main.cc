#include "timers.h"
#include "program.h"

int main(int argc, char *argv[]) {
  jayl::init();

  jayl::Program program;
  program.loadFile(argv[1]);

  return 0;
}