// HW2 - BILIOURI VICTORIA 2138 , BEKA KLEOPATRA 2246
// P2ARCHIVE: This program takes the files of a catalog
// and stores their attributes and their content 
// in an other file. 

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
#define MAX_SIZE 512


///////////////////////////////////////my_open//////////////////////////////
// ulopoihsh synartisis open me tous aparaititous elegxous tis timis epistrofis

int my_open(char *file_ptr) {
    
    int fd, i;
    char file[SIZE];
    
	for (i=0; *file_ptr != '\0'; i++) {
        file[i] = *file_ptr;
		
        file_ptr++;
		
    }
    file[i] = '\0';
    
    fd = open(file, O_RDWR, S_IRWXU);
    
    if (fd == -1) {
        
		
		fprintf(stderr, "error_open: %s\n", strerror(errno));
		
        exit(1); 
        
    }
    return (fd);
}

///////////////////////////////////////my_write/////////////////////////////////
// ulopoihsh sunartisis write me tous aparaithtous elegxous tis timis epistrofis

int my_write(int fd,const void *buf, int size) {
    
    ssize_t val;
    
    val = write (fd, buf, size);
    if (val == -1) {
        
		fprintf(stderr, "error_write: %s line %d", strerror(errno), __LINE__);
        return -1;
    }
    else if(val < size) {
        
        printf("Not written correctly!\n");
        return 0;
    }
    
    return 1;
}

//////////////////////////////////////my_read////////////////////////////////////
// ulopoihsh ths synarthshs read me tous aparaithtous elegxous tis timis epistrofhs

int my_read(int fd, void *buf, int size) {
    
    ssize_t val, bytes;
    
    bytes = read(fd, buf, size);
    
    if (bytes == -1) {
        
        fprintf(stderr, "error_read1: %s\n", strerror(errno));
        return -1;
    }
    if (bytes == 0) {
        
        fprintf(stderr, "error_read: %s\n", strerror(errno));
        return -1;
    }
    
    while (bytes < size) {
        
        val = read(fd, buf+bytes , size-bytes);
        
        if (val == 0) {
            return -1;
        }
        
        
        bytes = bytes + val;
    }
    
    return(bytes);
}


int main (int argc, char *argv[]) {
    
    int  j,  fd, div, mod, return_value;
    char  *check_num, buf[MAX_SIZE], name[SIZE];
    size_t name_size, file_size;
   
   
    struct stat st;
	
	
	while (1) {
		
		check_num = fgets(name, SIZE, stdin);                             //pairnoume to onoma tou arxeiou
		if (check_num == NULL) {                                          //elegxos timis epistrofis tis fgets
			return 0;
		}
		name[strlen(name)-1] = '\0';
        
        name_size = strlen(name)+1;
        
        fd = my_open(&name[0]);
        
		my_write(1, &name_size, sizeof(size_t));                         //grafoume sto arxeio to megethos tou onomatos
		
		my_write(1, &name[0], name_size);                                //grafoume sto arxeio to onom
		
		my_write(1, &st.st_atime, sizeof(size_t));                       //grafoume sto arxeio ton xrono teleytaias prosvasis sto arxeio
		
		my_write(1, &st.st_mtime, sizeof(size_t));                       //grafoume sto arxeio ton xrono teleytaias tropopoihshs tou arxeiou
		
		my_write(1, &st.st_mode, sizeof(size_t));                        //grafoume sto arxeio to eidos tou arxeiou
		
		return_value = fstat(fd, &st);
		if (return_value != 0) {
			fprintf(stderr, "error_fstat: %s\n", strerror(errno));
			exit(1);
		}
		
        file_size = st.st_size;
		div = file_size/MAX_SIZE;                                        //akeraia diairesi tou megethous tou arxeiou me to 512 
		mod = file_size%MAX_SIZE;                                        
		
		my_write(1, &file_size, sizeof(size_t));                         //grafoume sto arxeio to megethos tou arxeiou 
		
		if (div>0) {  
			for(j=0; j<div; j++) {
				
				my_read(fd, buf, MAX_SIZE);                              //grafoume sto arxeio ta periexomena ana 512 bytes
				my_write(1, buf, MAX_SIZE);
				
			}
			
		}
		if (mod>0) {
			my_read(fd, buf, mod);
			my_write(1, buf, mod);
			
		}
	}
	
    return 0;
}
