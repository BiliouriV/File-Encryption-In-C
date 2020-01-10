// HW2 - BILIOURI VICTORIA 2138 , BEKA KLEOPATRA 2246
// P2CRYPT: This program takes the content of a file 
// and encrypts or decrypts it with an xor between 
// the characters and a key word that the user sets

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define MAX_SIZE 512

//////////////////////////////////my_read//////////////////////////////////
//ulopoihsh read me ton apaitoumeno elegxo

int my_read(int fd, void *buf, int size) {
    
    ssize_t val;
    
    val = read(fd, buf, size);
    
    if (val == -1) {
        
        fprintf(stderr, "error_read: %s\n", strerror(errno));
        return -1;
    }
    
    return(val);
}

//////////////////////////////////my_read//////////////////////////////////
//ulopoihsh read me ton apaitoumeno elegxo

int my_write(int fd,const void *buf, int size) {
    
    ssize_t val;
    
    val = write (fd, buf, size);
    if (val == -1) {
        
		fprintf(stderr, "error_write: %s\n", strerror(errno));
        return -1;
    }
    else if(val < size) {
        
        fprintf(stderr, "error_write:  written correctly!\n");
        return 0;
    }
    
    return (val);
}

int main (int argc, char *argv[]) {
	
	char buf[MAX_SIZE], crypt_buf[MAX_SIZE];
	int i , k=0, key_size = 0, return_value;

	key_size = strlen(argv[1]);
	
	while (1) {	

		return_value = my_read(0, buf, MAX_SIZE);
		
		if(return_value == 0) {
			exit(1);
		}
		
		for (i=0; i < return_value; i++) {                                                    //kryptografisi tou periexomenou. XOR anamesa stous xaraktires
			crypt_buf[i] = buf[i]^argv[1][k];                                                 //tou periexomenou kai tou kleidiou
			my_write(1, &crypt_buf[i], sizeof(char));
			k++;
			if (k == key_size) {
				k=0;
			}
		}
	}

	return 0;
}
