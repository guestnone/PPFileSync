//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
#pragma once
/**
 * This function checks the structure of the directory and modifies it if it's different
 * @param source_path Path from which the files should be synchronized.
 * @param destination_path Destination for synchronized files (where they should be written..
 * @param recursive Synchronize both all directories that are in source. (1 if true, 0 if false)
 * @param threshold Threshold (in MegaBytes) where the file should use more performant file copying mechanism.
 */
void performSynchronization(char* source_path, char* destination_path, int recursive, int threshold);

/*
 * Performs the copying operations after checking the folder structure.
 * @param source_path Path from which the files should be synchronized.
 * @param destination_path Destination for synchronized files (where they should be written..
 * @param recursive Synchronize both all directories that are in source. (1 if true, 0 if false)
 * @param threshold Threshold (in MegaBytes) where the file should use more performant file copying mechanism
 */
void copyPasteElements(char *source_path, char *destination_path, int recursive, int threshold);