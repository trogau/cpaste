
#include "standard.h"

#define VERSION "0.9"

bool main(int argc, char *argv[])
{
  UINT retval = 0;
  UINT numitems = 0;
  UINT inval = 0;

  bool moveflag = false;
  bool overwriteExisting = false;

  char sourcefile[_MAX_PATH];
  char string[_MAX_PATH];
  char filename[_MAX_PATH];
  char dest[_MAX_PATH];
  char destdir[_MAX_PATH];
  char temp[_MAX_PATH];

  HDROP cliphdrop;

  int result = 1;

  char buffer[_MAX_PATH];  
   
  if ( argc == 2 )
  {
    if (strcmp(argv[1], "-v") == 0)
    {
      printf("\n");
      printf(" | Paste | version %s\t\t\t", VERSION);
      printf("(C)\t David Harrison, 1998.\n");
      printf("\t\t\t\t\t ");
      PrintColour("trog@uq.net.au", FOREGROUND_BLUE| FOREGROUND_INTENSITY);
      printf("\n");
      return 0;
    }
    else if (strcmp(argv[1], "-h") == 0)
    {
      PrintColourBoxTop(FOREGROUND_RED);
      PrintColourBoxText("Usage:", FOREGROUND_RED);
      PrintColourBoxText("paste [switch]", FOREGROUND_RED);
      PrintColourBoxText(" ", FOREGROUND_RED);
      PrintColourBoxText("Default: Copies file(s) copied to clipboard to the current", FOREGROUND_RED);
      PrintColourBoxText("working directory.", FOREGROUND_RED);
      PrintColourBoxText(" ", FOREGROUND_RED);
      PrintColourBoxText("Switches:", FOREGROUND_RED);
      PrintColourBoxText(" ", FOREGROUND_RED);
      PrintColourBoxText("-o     Overwrite existing file", FOREGROUND_RED);      
      PrintColourBoxText("-m     Move", FOREGROUND_RED);
      PrintColourBoxText("-v     Version", FOREGROUND_RED);
      PrintColourBoxText("-h     Help", FOREGROUND_RED);
      PrintColourBoxBottom(FOREGROUND_RED);
      return 0;
    }
    else if (strcmp(argv[1], "-m") == 0)
    {
      moveflag = true;
    }
    else if ( strcmp(argv[1], "-o") == 0 )
    {
      overwriteExisting = true;
    }
  }
#ifdef __DEBUG
  else if ( argc == 3)
  {
    if (strcmp(argv[1], "-t") == 0)
    {
      CopyDirectory(argv[2], "Blah");
      return 0;
    }
  }
#endif //__DEBUG
  else if (argc != 1)
  {
    printf("Usage: paste -h for help\n");
    return 0;
  }
  
  PrintColour("Pasting...", FOREGROUND_RED |FOREGROUND_GREEN| FOREGROUND_INTENSITY);
  printf("\n");
  
  if( _getcwd( buffer, _MAX_PATH ) == NULL )
    perror( "_getcwd error" );
  else
  {
    strcpy(destdir, buffer);
    strcat(destdir, "\\");
  }
  
  if ( !OpenClipboard(NULL) )  
	  printf("ERROR: Cannot open clipboard.\n");

  if (IsClipboardFormatAvailable(CF_HDROP))
  {
#ifdef __DEBUG    
	  printf("Clipboard format CF_HDROP Available.\n");
#endif
  }
  else 
  {
    printf("FAILURE: Invalid clipboard format.\n");
    return 1;
  }

  if ( (cliphdrop = (HDROP) GetClipboardData(CF_HDROP)) == NULL)
  {
    MessageBox(NULL, "Call to GetClipboardData failed - Wrong data type on clipboard.", "Failure", MB_ICONSTOP );
    return 1;
  }  

  numitems = DragQueryFile(cliphdrop, 0xFFFFFFFF, NULL, 0 );

  while (inval < numitems)
  {
    retval = DragQueryFile(cliphdrop, inval, string, MAX_PATH );
    
    strcpy(sourcefile, string);
    
    if (GetFileTitle(string, filename, MAX_PATH ) != 0)
    {
      printf("Error: GetFileTitle(%s, %s, %d)\n", string, filename, MAX_PATH);
      return 1;
    }

#ifdef __DEBUG
    printf("String is %s\n", string);
    printf("filename is %s\n", filename);
#endif

    strcpy(dest, destdir);
    strcat(dest, filename);
    printf("Copying \t%s\nto \t\t%s...\n", sourcefile, dest);

    if (IsDirectory(sourcefile))
    {
      if (moveflag)
        MoveDirectory(sourcefile, dest);
      else
        CopyDirectory(sourcefile, dest);
    }
    else                                          // We're only dealing with a file
    {
      if (!moveflag)
      {
        if (overwriteExisting)
        {
          if (CopyFile(sourcefile, dest, FALSE) == 0) // Overwrite existing
          {
            FormatLastError();
            PrintColour("Error: ", FOREGROUND_INTENSITY | FOREGROUND_RED);
            printf("%s\n", errorMessage);
            return 1;
          }
          else printf("File copied.\n");
        }
        else
        {
          if (CopyFile(sourcefile, dest, TRUE) == 0) // Overwrite existing
          {
            FormatLastError();
            PrintColour("Error: ", FOREGROUND_INTENSITY | FOREGROUND_RED);
            printf("%s\n", errorMessage);
            return 1;
          }
          else printf("File copied.\n");
        }
      }
      else 
      {
        if (MoveFile(sourcefile, dest) == 0)
        {
          FormatLastError();
          PrintColour("Error: ", FOREGROUND_INTENSITY | FOREGROUND_RED);
          printf("%s\n", errorMessage);
          return 1;
        }
        else printf("File moved.\n");
  	  }
    }

    inval++;  
  }

  if (CloseClipboard() == 0)
    printf("ERROR: Cannot close clipboard\n");

  //printf("%d item(s) copied.", numitems);

  sprintf(temp, "%d item(s) copied.", numitems);
  printf("%s\n", temp);
  //PrintColour(temp, FOREGROUND_INTENSITY | FOREGROUND_RED);
  printf("\n");
  return 0;
}

