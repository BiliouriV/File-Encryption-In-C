// HW2 - BILIOURI VICTORIA 2138 , BEKA KLEOPATRA 2246 
// HW2: main program. it creats pipes and connects
// the processes that are created in the other programs

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <utime.h>
#include <sys/time.h>

#define MAX_SIZE 512
#define MAGIC_NUMBER 9


////////////////////////////////my_write//////////////////////////////////
// ulopoihsh write me tous aparaithtous elegxous tis timis epistrofis

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

/////////////////////////////////my_read///////////////////////////////////
// ylopoihsh read me tous aparaititous elegxous tis timis epistrofis 

int my_read(int fd, void *buf, int size) {
    
    ssize_t val, bytes;
    
    bytes = read(fd, buf, size);
    if (bytes == -1) {
        
        fprintf(stderr, "error_read: %s\n", strerror(errno));
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
	char dir_name[MAX_SIZE], key[MAX_SIZE], file_name[MAX_SIZE];
	int fd, return_val, first_pipe[2], second_pipe[2], third_pipe[2];
	int pipefd, dupfd, i; 
	
	pid_t first_pid, second_pid, third_pid, waitpid_val;
    
	if (strcmp(argv[1], "-E") == 0) {                                                          //an to 2o orisma tou xristi einai -E
        
		for (i=0; argv[2][i]; i++) {                                                           //apothikeysi onomatos tou katalogou
			dir_name[i] = argv[2][i];
		}
		dir_name[i] = '\0';
		for (i=0; argv[3][i]; i++) {                                                           //apothikeysi kleidiou
			key[i] = argv[3][i];
		}
		key[i] = '\0';
		for (i=0; argv[4][i]; i++) {                                                           //apothikeusi onomatos arxeiou
			file_name[i] = argv[4][i];
		}
		file_name[i] = '\0';
		fd = open(file_name, O_RDWR|O_CREAT|O_EXCL, S_IRWXU);                                  //anoigma tou arxeiou
		
		if ((fd == -1) && (errno == EEXIST)) {                                                 // an to arxeio yparxei to programma termatizei
			fprintf(stderr, "The file already exists\n");
			exit(1);
		}
		else if (fd == -1) {
			fprintf(stderr, "error_open: %s\n", strerror(errno));
			exit(1);
		}
		return_val = my_write(fd, "P2CRYPTAR", MAGIC_NUMBER);                                  //an den yparxei dimiourgeitai kai periexei to magic number
		
		pipefd = pipe(first_pipe);                                                             //dimiourgia tou 1ou pipe
		
		if (pipefd != 0) {
			
			fprintf(stderr, "error_pipe: %s\n", strerror(errno));
			exit(1);
		}
		first_pid = fork();                                                                    //dimiourgia tou 1ou paidiou
		
		if (first_pid == -1) {
			fprintf(stderr, "error_fork: %s\n", strerror(errno));
			exit(1);
		}
		else if (first_pid == 0) {                                                            //kathorismos akrou anagnwsis kai eggrafis
            dupfd = dup2(first_pipe[1], STDOUT_FILENO);                                       
            
            if (dupfd == -1) {
                fprintf(stderr, "error_dup2: %s\n", strerror(errno)); 
            }
            close(first_pipe[1]);
            
            return_val = execlp("./dirlist", "dirlist", dir_name, NULL);
            
            if (return_val != 0) {
                
                fprintf(stderr, "error_execlp: %s\n", strerror(errno));
                exit(1);
            }
        }
		
		close(first_pipe[1]);
		
		////////////////////////////2nd pid///////////////////////////
	
		pipefd = pipe(second_pipe);                                                           //dimiourgia 2o pipe
		
		if (pipefd != 0) {
			
			fprintf(stderr, "error_pipe: %s\n", strerror(errno));
			exit(1);
		}
		
		second_pid = fork();                                                                  //dimiourgia 2ou paidiou
		
		if (second_pid == -1) {
			
			fprintf(stderr, "error_fork: %s\n", strerror(errno));
			exit(1);
		}
		
		else if (second_pid == 0) {                                                           //kathorismos akrwn anagnwsis kai eggrafis
		
            dupfd = dup2(first_pipe[0], STDIN_FILENO);
                
            if (dupfd == -1) {
                
                fprintf(stderr, "error_dup2: %s\n", strerror(errno)); 
            }
            
            
            dupfd = dup2(second_pipe[1], STDOUT_FILENO);
            
            if (dupfd == -1) {
                
                fprintf(stderr, "error_dup2: %s\n", strerror(errno)); 
            }
            
            return_val = execlp("./p2archive", "p2archive", NULL);
            
            if (return_val != 0) {
                
                fprintf(stderr, "error_execlp: %s\n", strerror(errno));
                exit(1);
            }
        }
        close(first_pipe[0]);
		close(second_pipe[1]);
		
		/////////////////////////////////////////3rd pid////////////////////////////////
		
		
		third_pid = fork();                                                                   //dimiourgia 3ou paidiou
		
		if (third_pid == -1) {
			
			fprintf(stderr, "error_fork: %s\n", strerror(errno));
			exit(1);
		}
		
		else if (third_pid == 0) {                                                            //kathorismos akrwn anagnwsis kai eggrafis
		
            dupfd = dup2(second_pipe[0], STDIN_FILENO);
            
            if (dupfd == -1) {
                
                fprintf(stderr, "error_dup2: %s\n", strerror(errno));
                exit(1);
            }
            

            
            dupfd = dup2(fd, STDOUT_FILENO);
            
            if (dupfd == -1) {
                
                fprintf(stderr, "error_dup2: %s\n", strerror(errno));
                exit(1);
            }
            
            return_val = execlp("./p2crypt", "p2crypt", key, NULL);
            
            if (return_val != 0) {
                
                fprintf(stderr, "error_execl: %s\n", strerror(errno));
                exit(1);
            }
        }
        close(second_pipe[0]);
		
		waitpid_val = waitpid(first_pid, NULL, 0);                                            //to kyriws programma perimenei na oloklirwthoun oi diergasies
                                                                                              //paidia gia na oloklirwthei
		if (waitpid_val == -1) {
			
			fprintf(stderr, "error_waitpid: %s\n", strerror(errno));
			exit(1);
		}
		
		waitpid_val = waitpid(second_pid, NULL, 0);
		
		if (waitpid_val == -1) {
			
			fprintf(stderr, "error_waitpid: %s\n", strerror(errno));
			exit(1);
		}
		
		waitpid_val = waitpid(third_pid, NULL, 0);
		
		if (waitpid_val == -1) {
			
			fprintf(stderr, "error_waitpid: %s\n", strerror(errno));
			exit(1);
		}
		
	}
	else if (strcmp(argv[1], "-D") == 0) {                                                   //an to 2o orisma pou dinei o xristis einai -D
        
		
		for (i=0; argv[2][i]; i++) {
			dir_name[i] = argv[2][i];
		}
		dir_name[i] = '\0';
		
		for (i=0; argv[3][i]; i++) {
			key[i] = argv[3][i];
		}
		key[i] = '\0';
		
		for (i=0; argv[4][i]; i++) {
			file_name[i] = argv[4][i];
		}
		file_name[i] = '\0';
		
		fd = open(file_name, O_RDWR, S_IRWXU);
		
		if ((fd == -1) && (fd == EEXIST)) {
			
			fprintf(stderr, "error_open: The file exists\n");
			exit(1);
		}
		else if (fd == -1) {
            fd = open(file_name, O_RDWR| O_CREAT , S_IRWXU);
            
            return_val = my_write(fd, "P2CRYPTAR", MAGIC_NUMBER);
            
            if (return_val == -1) {
             
                fprintf(stderr, "error_write: %s\n", strerror(errno));
                exit(1);
            }
        }
		
		///////////////////////////////////first_pid/////////////////////////////
		
		pipefd = pipe(third_pipe);
		
		if (pipefd != 0) {
			
			fprintf(stderr, "error_pipe: %s\n", strerror(errno));
			exit(1);
		}
			
		
		first_pid = fork();
		
		if (first_pid == -1) {
			
			fprintf(stderr, "error_fork: %s\n", strerror(errno));
			exit(1);
		}
		else if (first_pid == 0) {
            close(third_pipe[1]);
            dupfd = dup2(fd, STDIN_FILENO);
            
            if (dupfd == -1) {
                fprintf(stderr, "error_dup2: %s\n", strerror(errno));
                exit(1);
            }
                
            dupfd = dup2(third_pipe[1], STDOUT_FILENO);
            
            if (dupfd == -1) {
                
                fprintf(stderr, "error_dup2: %s\n", strerror(errno)); 
                exit(1);
            }
            close(third_pipe[1]);
            
            return_val = execlp("./p2crypt", "p2crypt", key, NULL);
            
            if (return_val != 0) {
                
                fprintf(stderr, "error_execlp: %s\n", strerror(errno));
                exit(1);
            }
            return 0 ;
        }
		
		close(third_pipe[1]);
        
		///////////////////////////////2nd pid////////////////////////////
		
		if (pipefd != 0) {
			
			fprintf(stderr, "error_pipe: %s\n", strerror(errno));
			exit(1);
		}
		second_pid = fork();
		
		if (second_pid == -1) {
			
			fprintf(stderr, "error_fork: %s\n", strerror(errno));
			exit(1);
		}
		else if (second_pid == 0) {
            close(third_pipe[1]);
            dupfd = dup2(third_pipe[0], STDIN_FILENO);
            
            if (dupfd == -1) {
                
                fprintf(stderr, "error_dup2: %s\n", strerror(errno)); 
            }
            close(third_pipe[0]);
            return_val = execlp("./p2unarchive", "p2unarchive", dir_name, NULL);
            
            if (return_val != 0) {
                
                fprintf(stderr, "error_execlp: %s\n", strerror(errno));
                exit(1);
            }
            return 0;
        }
        close(third_pipe[0]);
        close(third_pipe[1]);
		
		waitpid_val = waitpid(first_pid, NULL, 0);
		
		if (waitpid_val == -1) {
			
			fprintf(stderr, "error_waitpid: %s\n", strerror(errno));
			exit(1);
		}
		
		waitpid_val = waitpid(second_pid, NULL, 0);
		
		if (waitpid_val == -1) {
			
			fprintf(stderr, "error_waitpid: %s\n", strerror(errno));
			exit(1);
		}
		
	}
	else {
		fprintf(stderr, "Wrong Wrong!!\n");
	}
	
	return 0;
} 
