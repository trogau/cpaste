
#include "standard.h"

bool CopyDirectory(char *srcdir, char *destdir)
{
  WIN32_FIND_DATA findData;
  WIN32_FIND_DATA *fd = &findData;
  char createDir[_MAX_PATH];
  char sourceDir[_MAX_PATH];
  HANDLE hFile;
  char *sd; 

  strcpy(sourceDir, srcdir);
  if (strlen(sourceDir) + 4 < _MAX_PATH)
    strcat(sourceDir, "\\*.*");
  else
  {
    printf("ERROR: Source directory path too long!\n");
    return 1;
  }
  sd = &sourceDir[0];
  hFile = FindFirstFile(sd, fd);
  
  if (hFile == INVALID_HANDLE_VALUE)
  {
    printf("ERROR: FindFirstFile returned an invalid file handle!\n");
    return 1;
  }

  // Create the base copy directory
  strcpy(createDir, destdir);

  if (CreateDirectory(createDir, NULL) == 0)
  {
    FormatLastError();
    PrintColour("Error: ", FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("%s", errorMessage);
    FreeLastError();
    printf("     : Couldn't create directory %s!\n", createDir);
    FindClose(hFile);
    return 1;
  }
  else
    printf("\xC9 Creating directory %s...\n", createDir);


/*
  if (fd->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && (strcmp(".", fd->cFileName) != 0) )
  {
    printf("!!<DIR>\t\t%s\\%s\n", srcdir, fd->cFileName);

    strcpy(createDir, destdir);

    if (CreateDirectory(createDir, NULL) == 0)
    {
      printf("ERROR: CreateDirectory(%s) failed!\n", createDir);
      return false;
    }
    else 
    {
      printf("Successfully created %s!\n", createDir);
    }
  }
*/
  FindNextFile(hFile, fd);

  while(FindNextFile(hFile, fd))
  {
    if (fd->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
      if ( strcmp("..", fd->cFileName) != 0 ) 
      {
        //printf("<DIR>\t\t%s\\%s\n", srcdir, fd->cFileName);
        char newDir[_MAX_PATH];
        strcpy(newDir, srcdir);
        if (strlen(newDir) + 1 + strlen(fd->cFileName) < _MAX_PATH)
        {
          strcat(newDir, "\\");
          strcat(newDir, fd->cFileName);
        }
        else
        {
          PrintColour("ERROR: Path too long for subdirectory!\n", FOREGROUND_RED | FOREGROUND_INTENSITY);
          FindClose(hFile);
          return 1;
        }
        //printf("New Dirname = %s\n", newDir);

        strcpy(createDir, destdir);
        if (strlen(createDir) + 1 + strlen(fd->cFileName) < _MAX_PATH)
        {
          strcat(createDir, "\\");
          strcat(createDir, fd->cFileName);
        }
        else
        {
          PrintColour("ERROR: Destination path too long for subdirectory!\n", FOREGROUND_RED | FOREGROUND_INTENSITY);
          FindClose(hFile);
          return 1;
        }      

        if (CopyDirectory(newDir, createDir) == 1)
        {
          PrintColour("ERROR: Recursive CopyDirectory failed!", FOREGROUND_RED | FOREGROUND_INTENSITY);
          printf("newDir:    %s\n", newDir);
          printf("createDir: %s\n", createDir);
        }
      }
      else 
      {
        printf("PANIC! Error: found invalid filename \"..\"!\n");
        return 1;
      }
    }
    else                                          // Copy the file...
    {
      char destFile[_MAX_PATH]; 
      char srcFile[_MAX_PATH];

      strcpy(destFile, destdir);
      if (strlen(destFile) + 1 + strlen(fd->cFileName) < _MAX_PATH)
      {
        strcat(destFile, "\\");
        strcat(destFile, fd->cFileName);
      }
      else
      {
        PrintColour("ERROR: Destination file path too long!\n", FOREGROUND_RED | FOREGROUND_INTENSITY);
        FindClose(hFile);
        return 1;
      }

      strcpy(srcFile, srcdir);
      if (strlen(srcFile) + 1 + strlen(fd->cFileName) < _MAX_PATH)
      {
        strcat(srcFile, "\\");
        strcat(srcFile, fd->cFileName);
      }
      else
      {
        PrintColour("ERROR: Source file path too long!\n", FOREGROUND_RED | FOREGROUND_INTENSITY);
        FindClose(hFile);
        return 1;
      }

      printf("\xBA Copying: %s\n\xBA To     : %s\n", srcFile, destFile);
      
      if (CopyFile(srcFile, destFile, FALSE) == 0) // Overwrite existing
      {
        FormatLastError();
        PrintColour("Error: ", FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("%s\n", errorMessage);
        FreeLastError();
        FindClose(hFile);
        return 1;
      }
      else printf("File copied.\n");
    }
  }
  FindClose(hFile);
  return 0;
}

bool MoveDirectory(char *srcdir, char *destdir)
{
  WIN32_FIND_DATA findData;
  WIN32_FIND_DATA *fd = &findData;
  char createDir[_MAX_PATH];
  char sourceDir[_MAX_PATH];
  HANDLE hFile;
  char *sd; 

  strcpy(sourceDir, srcdir);
  if (strlen(sourceDir) + 4 < _MAX_PATH)
    strcat(sourceDir, "\\*.*");
  else
  {
    printf("ERROR: Source directory path too long!\n");
    return 1;
  }
  sd = &sourceDir[0];
  hFile = FindFirstFile(sd, fd);
  
  if (hFile == INVALID_HANDLE_VALUE)
  {
    printf("ERROR: FindFirstFile returned an invalid file handle!\n");
    return 1;
  }

  // Create the base copy directory
  strcpy(createDir, destdir);

  if (CreateDirectory(createDir, NULL) == 0)
  {
    FormatLastError();
    PrintColour("Error: ", FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("%s", errorMessage);
    FreeLastError();
    printf("     : Couldn't create directory %s!\n", createDir);
    FindClose(hFile);
    return 1;
  }
  else
    printf("\xC9 Creating directory %s...\n", createDir);

  FindNextFile(hFile, fd);                        // shift to ignore ".."

  while(FindNextFile(hFile, fd))
  {
    if (fd->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
      char newDir[_MAX_PATH];
      strcpy(newDir, srcdir);
      if (strlen(newDir) + 1 + strlen(fd->cFileName) < _MAX_PATH)
      {
        strcat(newDir, "\\");
        strcat(newDir, fd->cFileName);
      }
      else
      {
        PrintColour("ERROR: Path too long for subdirectory!\n", FOREGROUND_RED | FOREGROUND_INTENSITY);
        FindClose(hFile);
        return 1;
      }

      strcpy(createDir, destdir);
      if (strlen(createDir) + 1 + strlen(fd->cFileName) < _MAX_PATH)
      {
        strcat(createDir, "\\");
        strcat(createDir, fd->cFileName);
      }
      else
      {
        PrintColour("ERROR: Destination path too long for subdirectory!\n", FOREGROUND_RED | FOREGROUND_INTENSITY);
        FindClose(hFile);
        return 1;
      }      

      if (MoveDirectory(newDir, createDir) == 1)
      {
        PrintColour("ERROR: Recursive MoveDirectory failed!", FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("newDir:    %s\n", newDir);
        printf("createDir: %s\n", createDir);
      }
    }
    else                                          // Copy the file...
    {
      char destFile[_MAX_PATH]; 
      char srcFile[_MAX_PATH];

      strcpy(destFile, destdir);
      if (strlen(destFile) + 1 + strlen(fd->cFileName) < _MAX_PATH)
      {
        strcat(destFile, "\\");
        strcat(destFile, fd->cFileName);
      }
      else
      {
        PrintColour("ERROR: Destination file path too long!\n", FOREGROUND_RED | FOREGROUND_INTENSITY);
        FindClose(hFile);
        return 1;
      }

      strcpy(srcFile, srcdir);
      if (strlen(srcFile) + 1 + strlen(fd->cFileName) < _MAX_PATH)
      {
        strcat(srcFile, "\\");
        strcat(srcFile, fd->cFileName);
      }
      else
      {
        PrintColour("ERROR: Source file path too long!\n", FOREGROUND_RED | FOREGROUND_INTENSITY);
        FindClose(hFile);
        return 1;
      }

      printf("\xBA Moving: %s\n\xBA To     : %s\n", srcFile, destFile);
      
      if (MoveFile(srcFile, destFile) == 0) // Overwrite existing
      {
        FormatLastError();
        PrintColour("Error: ", FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("%s\n", errorMessage);
        FreeLastError();
        FindClose(hFile);
        return 1;
      }
      else printf("File moved.\n");
    }
  }

  FindClose(hFile);
  return 0;
}

bool IsDirectory(char *filename)
{
  WIN32_FIND_DATA findData;
  WIN32_FIND_DATA *fd = &findData;

  HANDLE hFile = FindFirstFile(filename, fd);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    printf("Error!\n");
    return false;
  }

  if (fd->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    return true;

  return false;
}

bool RemoveDirectoryTree(char *dirName)
{
  return false;
}


void PrintColour(char *string, WORD colour)
{
  HANDLE    handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  WORD      wAttributesOld;

  GetConsoleScreenBufferInfo(handle, &csbi);
  wAttributesOld = csbi.wAttributes;

  SetConsoleTextAttribute( handle, colour );

  printf("%s", string);
  SetConsoleTextAttribute( handle, wAttributesOld );
}

void PrintColourBoxTop(WORD colour)
{
  int i; 
  PrintColour("\xC9", colour);
  for (i = 0; i < 77; i++)
    PrintColour("\xCD", colour);
  PrintColour("\xBB", colour);
  printf("\n");
}

void PrintColourBoxBottom(WORD colour)
{
  int i;
  PrintColour("\xC8", colour);
  for (i = 0; i < 77; i++)
    PrintColour("\xCD", colour);
  PrintColour("\xBC", colour);
  printf("\n");
}

void PrintColourBoxText(char *string, WORD colour)
{
  int ct;
  int numSpaces;
  PrintColour("\xBA   ", colour);
  printf("%s", string);

  numSpaces = 74 - (strlen(string));
  for (ct = 0; ct < numSpaces; ct++)
    printf(" ");

  PrintColour("\xBA", colour);
  printf("\n");
}

void FormatLastError(void)
{
  if (FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &errorMessage,
    0,
    NULL) == 0)
  {
    printf("ERROR: Failed to FormatLastError!\n");
  }
}

void FreeLastError(void)
{
  if (errorMessage != NULL)
  {
    LocalFree(errorMessage);
    errorMessage = NULL;
  }
}


/*
void Com_sprintf (char *dest, int size, char *fmt, ...)
{
        int             len;
        va_list         argptr;
        char    bigbuffer[0x10000];

        va_start (argptr,fmt);
        len = vsprintf (bigbuffer,fmt,argptr);
        va_end (argptr);
        if (len >= size)
                Com_Printf ("Com_sprintf: overflow of %i in %i\n", len, size);
        strncpy (dest, bigbuffer, size-1);
}
*/


