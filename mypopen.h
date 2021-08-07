#ifndef mypopen_h
#define mypopen_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h> // waidpid()
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>

FILE *mypopen(const char *command, const char *type);
int *mypclose(FILE *stream);


#endif //MYPOPEN_MYPOPEN_H
