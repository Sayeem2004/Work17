#ifndef HEADER

// Duplicate Prevention
#define HEADER 0

// Library Imports
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>

// Constants
#define SHM_KEY 63021
#define SEM_KEY 63022
#define BUFF_SIZE 2048
#define FILE_NAME "story.txt"

#endif
