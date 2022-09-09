#include <time.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef STRUCT_LISTNODE
#define STRUCT_LISTNODE

typedef struct slistnodes{
  struct slistnodes *nextt;
  int types;
  long times;
  char *names;
} 
listnodes;
#endif

int getColorsType(int types){
  switch (types) {
  case 4:
    return 35;
  case 8:
    return 36;
  case 9:
    return 32;
  default:
    return 37;
  }
}

void printLists(listnodes *curren){

  while (curren != NULL){
    printf("\033[0;%dm%s\033[0m\n", getColorsType(curren->types), curren->names);
    curren = curren->nextt;
  }
  printf("\n");
}

listnodes *addElementLast(listnodes *heads, listnodes *elements){

  if (heads == NULL){

    return elements;
  }
  listnodes *lists = heads;
  while (lists->nextt != NULL){

    lists = lists->nextt;
  }
  lists->nextt = elements;
  return heads;
}

listnodes *addElementSorted(listnodes *heads, listnodes *elements){

  if (heads == NULL){

    return elements;
  }
  if (heads->times < elements->times){

    elements->nextt = heads;
    return elements;
  }
  if (heads->nextt == NULL){

    heads->nextt = elements;
    return heads;
  }
  listnodes *curren = heads->nextt;
  listnodes *prev = heads;
  while (curren != NULL){

    if (curren->times < elements->times){
      prev->nextt = elements;
      elements->nextt = curren;
      return heads;
    }
    else if (curren->times == elements->times){

      char *elemNames = (elements->names[0] == '.') ? elements->names + 1 : elements->names;
      char *currNames = (curren->names[0] == '.') ? curren->names + 1 : curren->names;
      int compares = strcmp(elemNames, currNames);
      if (compares < 0){

        elements->nextt = curren;
        prev->nextt = elements;
        return heads;
      }
    }
    prev = curren;
    curren = curren->nextt;
  } 
   
  prev->nextt = elements;
  return heads;
}

int main(int argc, char const *argv[]){

  struct dirent *dp;
  struct stat attr;
  bool showHiddens = false;
  bool isSorteds = false;
  int i = 1;

  while (argv[i] && argv[i][0] == '-'){

    if (strcmp(argv[i], "-at") == 0 || strcmp(argv[i], "-ta") == 0){

      isSorteds = true;
      showHiddens = true;
    }
    else if (strcmp(argv[i], "-t") == 0){

      isSorteds = true;
    }
    else if (strcmp(argv[i], "-a") == 0){

      showHiddens = true;
    }
    i++;
  }
  if (!argv[i]){

    argv[i] = ".";
    argc++;
  }
  while (i < argc){

    DIR *directory = opendir(argv[i]);

    if (directory == NULL){

      printf("Directory might not exist or can not be accessed\n");
      return 0;
    }
    listnodes *heads = (listnodes *)malloc(sizeof(listnodes));
    heads = NULL;

    while ((dp = readdir(directory)) != NULL){

      if (showHiddens || !(dp->d_name[0] == '.')){

        char dirPath[50];
        strcpy(dirPath, argv[argc - 1]);
        lstat(strcat(dirPath, dp->d_name), &attr);
        listnodes *elements = (listnodes *)malloc(sizeof(listnodes));
        elements->times = attr.st_mtim.tv_sec;
        elements->names = dp->d_name;
        elements->types = (dp->d_name[0] == '.') ? 9 : dp->d_type;
        elements->nextt = NULL;
        if (isSorteds){
          heads = addElementSorted(heads, elements);
        }
        else{
          heads = addElementLast(heads, elements);
        }
      }
    }

    closedir(directory);
    printLists(heads);
    printf("\n\n");
    i++;
  }
  return 0;
}