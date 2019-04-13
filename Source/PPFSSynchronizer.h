//
// Created by guest_none on 03.04.19.
//
#pragma once

#include "PPFSPrerequisites.h"

typedef enum
{
	ReadOnly,
	WriteOnly,
	ReadWrite
} FileMode;

int createFile(char* path, FileMode mode);

int copyDataFromPath(char* sourcePath, char* destPath, unsigned int fileSizeThreshold);

int copyDataFromFileDesc(int sourceFd, int destFd, unsigned int fileSizeThreshold);

int openFile(char* path, FileMode mode);

int removeFile(char* path);

int closeFileDesc(int fd);
