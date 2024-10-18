#include <dirent.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FILES 100
#define MAX_FILENAME_LENGTH 100

typedef struct fileList {
  int length;
  char **fileArray;
} fileList;

static int getFileOwners(const char *file) {
  struct stat fileStat;
  struct passwd *userInfo;
  struct group *groupInfo;

  if (stat(file, &fileStat) == 0) {
    __uid_t ownerId = fileStat.st_uid;
    __gid_t groupId = fileStat.st_gid;

    userInfo = getpwuid(ownerId);
    if (userInfo == NULL) {
      printf("%d", ownerId);
    } else {
      printf("%s", userInfo->pw_name);
    }

    printf(":");

    groupInfo = getgrgid(groupId);
    if (groupInfo == NULL) {
      printf("%d", groupId);
    } else {
      printf("%s", groupInfo->gr_name);
    }

  } else {
    fprintf(stderr, "Failed to get user/group owners.\n");
    return 1;
  }
  return 0;
}

static int getPermissions(const char *file) {
  struct stat fileStat;

  if (stat(file, &fileStat) == 0) {
    __mode_t fileMode = fileStat.st_mode;
    printf("%c%c%c", (fileMode & S_IRUSR) ? 'r' : '-',
           (fileMode & S_IWUSR) ? 'w' : '-', (fileMode & S_IXUSR) ? 'x' : '-');
    printf("%c%c%c", (fileMode & S_IRGRP) ? 'r' : '-',
           (fileMode & S_IWGRP) ? 'w' : '-', (fileMode & S_IXGRP) ? 'x' : '-');
    printf("%c%c%c ", (fileMode & S_IROTH) ? 'r' : '-',
           (fileMode & S_IWOTH) ? 'w' : '-', (fileMode & S_IXOTH) ? 'x' : '-');
  } else {
    fprintf(stderr, "Failed to read file permissions.\n");
    return 1;
  }
  return 0;
}

static const char *baseDir(char *baseDir, char *file) {
  int baseDirLength = strlen(baseDir);
  int filenameLength = strlen(file);
  char *lastCharacter = &baseDir[baseDirLength - 1];
  char *buffer = (char *)malloc(baseDirLength + 1 + filenameLength + 1);

  // int bufferSize = baseDirLength +  filenameLength + 1; // Plus 1 for
  // nullbyte

  if (strcmp(lastCharacter, "/") == 0) {
    sprintf(buffer, "%s%s", baseDir, file);
  } else {
    sprintf(buffer, "%s/%s", baseDir, file);
  }

  return buffer;
}

static struct fileList getFolderObjects(const char *location) {
  DIR *dp;
  struct dirent *ep;
  struct fileList listing;

  dp = opendir(location);

  listing.length = 0;
  listing.fileArray = malloc((MAX_FILES + 1) * sizeof(char *));
  for (int i = 0; i < MAX_FILES; i++) {
    listing.fileArray[i] = malloc(MAX_FILENAME_LENGTH + 1);
  }

  if (dp != NULL) {
    while ((ep = readdir(dp))) {
      if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) {
        continue;
      }
      strncpy(listing.fileArray[listing.length], ep->d_name,
              MAX_FILENAME_LENGTH);
      listing.fileArray[listing.length][MAX_FILENAME_LENGTH - 1] = '\0';
      listing.length++;

      if (listing.length >= MAX_FILES) {
        fprintf(stderr, "Reached max number of files");
        break;
      }
    }
  } else {
    fprintf(stderr, "Couldn't open the directory: %s", location);
    exit(1);
  }
  return listing;
}

static int help(void) {
  printf("The following commands are supported in this bad version of 'ls'\n");
  printf(
      "\t-d:\tSpecify a directory to list (defaults to current working "
      "directory).\n");
  printf("\t-l:\tGives a listing of files and directories with permissions.\n");
  printf("\t-h:\tPrints this help statement.\n");
  return 0;
}

static void printObjects(struct fileList objects) {
  for (int i = 0; i < objects.length; i++) {
    printf("%s\n", objects.fileArray[i]);
  }
}

int main(int argc, char *argv[]) {
  int c;
  int lFlag = 0;
  int dFlag = 0;
  char directory[1024];

  while ((c = getopt(argc, argv, "d:hl")) != -1) {
    switch (c) {
      case 'l':
        lFlag = 1;
        break;
      case 'd':
        dFlag = 1;
        strncpy(directory, optarg, sizeof(directory));
        break;
      case 'h':
        help();
        return 0;
        break;
      case '?':
        fprintf(stderr, "Unknown option. Use -h for help.\n");
        break;
      default:
        abort();
    }
  }

  if (dFlag == 0) {
    strncpy(directory, ".", sizeof(directory));
  }

  struct fileList objects = getFolderObjects(directory);

  if (lFlag == 1) {
    for (int i = 0; i < objects.length; i++) {
      const char *file = baseDir(directory, objects.fileArray[i]);
      getPermissions(file);
      printf(" ");
      getFileOwners(file);
      printf(" ");
      printf("%s\n", file);
    }
    return 0;
  }

  printObjects(objects);

  return 0;
}
