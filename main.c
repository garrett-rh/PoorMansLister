#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int readFileOrDirectory (char *fileOrDirectory)
{
//TODO: Detect if a file is passed in

  DIR *dp;
  struct dirent *ep;

  dp = opendir (fileOrDirectory);

  if (dp != NULL)
    {
      while ((ep = readdir (dp)))
      {
        puts (ep->d_name);
      }
      (void) closedir (dp);
    }
  else {
    fprintf (stderr, "Couldn't open the directory: %s", fileOrDirectory);
    return 1;
  }

  return 0;
}

int noArgs(int argc, char *argv[])
{
  int i;
  int j;
  int equalsLength;


  for (i = 1; i < argc; i++)
  {
    equalsLength = strlen(argv[i]);
    printf("=============%s=============\n", argv[i]);
    readFileOrDirectory(argv[i]);
    printf("=============");
    for (j = 0; j < equalsLength; j++) 
    {
      putchar('=');
    }
    printf("=============\n");
  }

  return 0;
}

void help(void){
  printf("The following commands are supported in this bad version of 'ls'\n");
  printf("\t-l:\tGives a listing of files and directories with permissions.\n");
  printf("\t-s:\tGives a listing of files and directories sorted alphabetically.\n");
  printf("\t-h:\tPrints this help statement.\n");
}
int longList(void)
{

  return 0;
}
void sort(void){}

int main (int argc, char *argv[])
{

  int c;
  int lFlag = 0;
  int sFlag = 0;

  while (( c = getopt ( argc, argv, "hls")) != -1 )
  {
    switch (c) {
      case 'h':
        help();
        break;
      case 'l':
        lFlag = 1;
        break;
      case 's':
        sFlag = 1;
        break;
      case '?':
        fprintf (stderr, "Unknown option. Use -h for help.\n");
      default:
        abort();
    }
  }

  printf("lflag: %d, sflag: %d\n", lFlag, sFlag);


  if ( optind + 1 < argc )
  {
    fprintf (stderr, "Currently only accepts one file or folder.\n");
  } 
  else if ( optind == argc )
  {
    printf("=============.=============\n");
    readFileOrDirectory(".");
    printf("===========================\n");
  } 
  else
  {
    readFileOrDirectory(argv[optind]);
    printf("This is where to action off the args\n");
  }

  return 0;
}