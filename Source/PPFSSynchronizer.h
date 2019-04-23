//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
#pragma once

#include "PPFSPrerequisites.h"

/**
 * Creates the copy of the source file to destination based on the path of the file.
 * @param sourcePath File to copy from
 * @param destPath Destination, where the file is copied
 * @param fileSizeThreshold Threshold (in MegaBytes) where the file should use more performant file copying mechanism.
 *
 * @note Every file open or copy error are treated as fatal and will crash the program.
 */
int copyDataFromPath(char* sourcePath, char* destPath, unsigned int fileSizeThreshold);

/**
 * Creates the copy of the source file to destination based on the file descriptors.
 * @param sourceFd File descriptor to copy from
 * @param destFd Destination descriptor, where the file is copied
 * @param fileSizeThreshold Threshold (in MegaBytes) where the file should use more performant file copying mechanism.
 *
 * @note Every file copy error are treated as fatal and will crash the program.
 */
void copyDataFromFileDesc(int sourceFd, int destFd, unsigned int fileSizeThreshold);

/**
 * Removes the file from the given path. Wrapper around unlink.
 * @param path File to delete.
 * @return 0 if OK. Other values if something gone wrong. Please check unlink()'s man-page for more info.
 */
int removeFile(char* path);

/**
 * Closes the file descriptor. Wrapper around the close() command.
 * @param fd File descriptor to close
 * @return  if OK. Other values if something gone wrong. Please check close()'s man-page for more info.
 */
int closeFileDesc(int fd);
