#include <cstdio>
#include <cstdlib>
#include <string>
#include "Loader.hpp"

void readFile(const char* file, char** buffer) {
  *buffer = nullptr;
  size_t size = 0;

  FILE *fp = fopen(file, "r");
  if (fp == nullptr) {
    throw "Can't open file. Maybe the path is wrong?";
  }
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);
  *buffer = (char *) malloc((size + 1) * sizeof(**buffer));
  fread(*buffer, size, 1, fp);
  (*buffer)[size] = '\0';
  fclose(fp);
}
