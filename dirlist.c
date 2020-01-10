// HW2 - BILIOURI VICTORIA 2138 , BEKA KLEOPATRA 2246 
// DIRLIST:This programme takes as input a catalog
// and set as outputs
// the names of the files that exist in the catalog

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define SIZE 100

int main (int argc, char *argv[]) {
    
    char  *dir_ptr, dir_name[SIZE];
    int i;
    DIR *val;
    struct dirent *dir;
    
    dir_ptr = argv[1];
    
    val = opendir(dir_ptr);                                                     //anoigma tou katalogou pou dothike
    
    for (i=0; *dir_ptr != '\0'; i++) {                                          //apothikeysi tou onomatos tou katalogou sto dir_name
        dir_name[i] = *dir_ptr;
        dir_ptr++;
    }
    dir_name[i] = '\0';
    
    if (val != NULL) {
        
        while ((dir = readdir(val)) != NULL) {                                  //gia oso yparxoun arxeia emfanizei ta onomata tous
            
            if (dir->d_type != DT_DIR) {
                printf("%s/%s\n",dir_name, dir->d_name);
            }
        }
        closedir(val);
        return 0;
    }
    else if ((val == NULL) && (errno == ENOENT)) {                              //an einai NULL o katalogos den uparxei
        fprintf(stderr, "The directory does not exist\n");
        exit (1);
    }
     else  {
        fprintf(stderr, "error_opendir: %s", strerror(errno));
        exit (1);
    }
    
   
  
    
    
    return 0;
    
    
}
