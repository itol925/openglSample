#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *textFileRead(const char* filename);
int textFileWrite(char *filename, char *s);

#endif
