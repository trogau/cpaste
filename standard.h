
#ifndef __STANDARD_H
#define __STANDARD_H
#define STRICT

#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <windows.h>
#include <winuser.h>
#include <winbase.h>

typedef enum { false, true } bool;

//
// Pointer to a buffer containing the errorMessage text from a GetLastError() call.
//
LPVOID errorMessage;


//
// bool CopyDirectory(char *srcdir, char *destdir);  
//
//    Recursively copy the directory referenced by srcdir to 
//    the directory referenced by destdir. 
//    RETURNS: true on success, false on any error.
//
bool CopyDirectory(char *srcdir, char *destdir);

//
// bool MoveDirectory(char *srcdir, char *destdir);  
//
//    Recursively move the directory referenced by srcdir to 
//    the directory referenced by destdir. 
//    RETURNS: true on success, false on any error.
//    FIXME: needs to delete the sr cdir tree.
//
bool MoveDirectory(char *srcdir, char *destdir);


bool RemoveDirectoryTree(char *dirName);


//
// bool IsDirectory(char *filename); 
//
//    Checks to see if the path specified in filename is a 
//    directory.
//    RETURNS: true if filename is a directory, false otherwise.
//
bool IsDirectory(char *filename); 

//
// void PrintColour(char *string, WORD colour);
//
//    Print a colour line of text.
//
void PrintColour(char *string, WORD colour);

//
// void PrintColourBoxTop(WORD colour);
//
//    Print the top of an ASCII text box (coloured as per parameter)
//
void PrintColourBoxTop(WORD colour);

//
//  PrintColourBoxBottom(WORD colour);
//
//    Print the bottom of an ASCII text box (coloured as per parameter)
//
void PrintColourBoxBottom(WORD colour);

//
// void PrintColourBoxText(char *string, WORD colour);
//
//    Prints a framed line for an ASCII text box
//
void PrintColourBoxText(char *string, WORD colour);

//
// void FormatLastError(void);
//
//    GetLastError and FormatMessage into errorMessage, 
//    ready for printing or displaying in a messagebox.
//
void FormatLastError(void);

#endif // __STANDARD_H