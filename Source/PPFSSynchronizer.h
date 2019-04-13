//
// Created by guest_none on 03.04.19.
//
#pragma once

enum
{
	ReadOnly,
	WriteOnly,
	ReadWrite
} FileMode;

int createFile(char* path, FileMode mode);

inline int removeFile(char* path);

int copyDataFromPath(char* sourcePath, char* destPath);

int copyDataFromFileDesc(int sourceFd, int destFd);

inline int closeFileDesc(int fd);

int openFile(char* path, FileMode mode);


