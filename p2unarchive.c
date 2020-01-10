// HW2 - BILIOURI VICTORIA 2138 , BEKA KLEOPATRA 2246 
// P2UNARCHIVE: This program takes as argument a file that 
// contains the data and the content of other files, 
// and it exports them in a new directory

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <utime.h>
#include <sys/time.h>

#define MAX_SIZE 512


//////////////////////////////////my_read/////////////////////////////
// yliopoihsh read me tous aparaititous elegxous tis timis epistrofhs

int my_read(int fd, void *buf, int size) {
    
    ssize_t  bytes;
    int val;
    
    
   bytes = read(fd, buf, size);
        
    if (bytes == -1) {
        
        fprintf(stderr, "error_read: %s\n", strerror(errno));
        exit(1);
    }
    
    
    while (bytes < size) {
        
        val = read(fd, buf+bytes , size-bytes);
        
        if (val == 0) {
        
            break;
        }
        bytes = bytes + val;
    }
    
    return(bytes);
}

//////////////////////////////////////my_write///////////////////////////////
// ylopoihsh write me tous aparaititous elegxous tis timis epistrofis 

int my_write(int fd,const void *buf, int size) {
    
    size_t  bytes;
    int val;
    
    bytes = write (fd, buf, size);
    if (bytes == -1) {
        
		fprintf(stderr, "error_write: %s line %d", strerror(errno), __LINE__);
        exit(1);
    }
    while (bytes < size) {
        
        fprintf(stderr, "!!!\n");
        val = write(fd, buf+bytes , size-bytes);
        
        if (val == 0) {
        
            break;
        }
        bytes = bytes + val;
    }
    
    return(bytes);
}

int main (int argc, char *argv[]) {
	
	int return_value,  mod, div, fd, j, val, i;
	char name[MAX_SIZE], buf[MAX_SIZE], *slash_ptr, pathname[MAX_SIZE], magic_number[9];
	mode_t mode;
	time_t atime, mtime;
	size_t name_size, file_size;
    
    
    
	struct timeval temp;
	struct timeval times[2];
	
	if (opendir(argv[1]) != NULL) {                                                                //elegxos an to onoma tou katalogou yparxei idi
                                                                                                   //an uparxei to programma termatizei 
		fprintf(stderr, "error_opendir: The directory already exists!\n");
		exit(1);
	}
	return_value = mkdir(argv[1], S_IRWXU);                                                        //o katalogos dimiourgeitai

	if (return_value != 0) {
		fprintf(stderr, "error_mkdir: %s", strerror(errno));
		exit(1);
	}
	
    
	while ((val = my_read(0, &name_size, sizeof(size_t))) >= sizeof(size_t)) {                   // oso yparxoun akomi arxeia sto megalo arxeio kanei to loop
        
		my_read(0, &name, name_size);                                                            // diavazei to onoma tou arxeiou
		name[name_size] = '\0';
		slash_ptr = strchr(name, '/');
		slash_ptr++;
        
        for (i=0; name[i] != '\0'; i++) {
            name[i] = *slash_ptr;
            slash_ptr++;
        }
        name[i] = '\0';
		pathname[0] = '\0';
		strcat(pathname, argv[1]);
		strcat(pathname, "/");
		strcat(pathname, name);                                                                   // dimiourgei to kainoyrgio onoma me to kenourgio path
		
		my_read(0, &atime, sizeof(time_t));                                                       //diavazei ton teleytaio xrono prosvasis 
		temp.tv_sec = atime;                                                                      //kai ton apothikeyei se ena struct typou timeval
		temp.tv_usec = 0;
		times[0] = temp;                                                                          //epeita krataei to struct ayto se enan pinaka apo structs
		
		my_read(0, &mtime, sizeof(time_t));                                                       // omoiws kai gia ton teleytaio xrono tropopoihshs tou arxeiou
		temp.tv_sec = mtime;
		temp.tv_usec = 0;
		times[1] = temp;
		
        fd = open(pathname, O_CREAT|O_RDWR, S_IRWXU);                                             //anoigei to arxeio kai an den yparxei to dimiourgei
		if (fd == -1) {
			fprintf(stderr, "error_open: %s\n", strerror(errno));
			exit(1);
		}
		
		my_read(0, &mode, sizeof(mode_t));                                                        //diavazei to eidos tou arxeiou
        
		return_value = chmod(pathname, mode);
		if (return_value == -1) {
			fprintf(stderr, "error_chmod: %s\n", strerror(errno));
			exit(1);
		}
		
		my_read(0, &file_size, sizeof(size_t));                                                  // divazei to megethos tou arxeiou
        
		
		div = file_size/MAX_SIZE;                                                                 //akeraia diairesi tou megethous tou arxeiou me to 512 
		mod = file_size%MAX_SIZE;
		
		if (div>0) {                                                                              //pernaei ta periexomena tou arxeiou ana 512 bytes
			for(j=0; j<div; j++) {
				
				val = my_read(0, buf, MAX_SIZE);
				my_write(fd, buf, MAX_SIZE);
			}
		}
		if (mod>0) {
			my_read(0, buf, mod);
			my_write(fd, buf, mod);
		}
		
		return_value = utimes(pathname, times);                                                   //allazei tous xronous kai vazei tous palious
		if (return_value != 0) {
			fprintf(stderr, "error_utimes: %s\n", strerror(errno));
			exit(1);
		}
	}
	
	return 0;
}
