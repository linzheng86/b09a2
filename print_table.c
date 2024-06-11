#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "get_information.c"

// this function prints the process table according to whether the pid is given
void print_process_table(fdNode *head, int pid){
    printf(" %-4s %7s %6s \n", " ", " PID", " FD");
    printf("%8s============\n", " ");
    int i = 0;
    if(pid == -1){  // if pid is not specified, write the whole process table
        fdNode *p = head;
        while(p != NULL){
            printf(" %-4d %10d  %-7d\n", i, p->filedes.pid, p->filedes.fd);
            i++;
            p = p->next;
        }
    }
    else{       // if pid is specified, only write the process information about that specified pid
        fdNode *p = head;
        while(p != NULL){
            if(p->filedes.pid == pid){
                printf(" %-4s %10d    %-7d\n", " ", p->filedes.pid, p->filedes.fd);
            }
            p = p->next;
        }
    }
    printf("%8s============\n", " ");
}

// this function prints the systemwise table according to whether the pid is given 
void print_systemwide_table(fdNode *head, int pid){
    printf(" %-4s %7s %6s  %-10s \n", " ", "PID", "FD", "Filename");
    printf("%8s========================================\n", " ");
    int i = 0;
    if(pid == -1){      // if pid is not specified, write the whole systemwide table
        fdNode *p = head;
        while(p != NULL){
            printf(" %-4d %10d  %-5d %-20s\n", i, p->filedes.pid, p->filedes.fd, p->filedes.filename);
            i++;
            p = p->next;
        }
    }
    else{       // if pid is specified, only write the systemwide information about that specified pid
        fdNode *p = head;
        while(p != NULL){
            if(p->filedes.pid == pid){
                printf(" %-4s %10d  %-5d %-20s\n", " ", p->filedes.pid, p->filedes.fd, p->filedes.filename);
            }
            p = p->next;
        }
    }    
    printf("%8s========================================\n", " ");
}

// this function prints the vnode table according to whether the pid is given
void print_vnode_table(fdNode *head, int pid){
    printf(" %-4s %7s %6s  %-10s \n", " ", "FD", " ", "Inode");
    printf("%8s========================================\n", " ");
    int i = 0;
    if(pid == -1){   // if pid is not specified, write the whole vnode table 
        fdNode *p = head;
        while(p != NULL){
            printf(" %-4d %10d  %-5s %-20d\n", i, p->filedes.fd, " ", p->filedes.inode);
            i++;
            p = p->next;
        }
    }
    else{   // if pid is specified, only write the vnode information about that specified pid
        fdNode *p = head;
        while(p != NULL){
            if(p->filedes.pid == pid){
                printf(" %-4s %10d  %-5s %-20d\n", " ", p->filedes.fd, " ", p->filedes.inode);
            }
            p = p->next;
        }
    }    
    printf("%8s========================================\n", " ");
}

// this function print the compsite table according to whether the pid is given
void print_composite_table(fdNode *head, int pid){
    printf(" %-4s %7s %6s  %-10s  %-7s\n", " ", "PID", "FD", "Filename", "Inode");
    printf("%8s========================================\n", " ");
    int i = 0;
    if(pid == -1){   // if pid is not specified, write the whole composite table 
        fdNode *p = head;
        while(p != NULL){
            printf(" %-4d %7d %6d  %-10s  %-7d\n", i, p->filedes.pid, p->filedes.fd, p->filedes.filename, p->filedes.inode);
            i++;
            p = p->next;
        }
    }
    else{   // if pid is specified, only write the composite information about that specified pid
        fdNode *p = head;
        while(p != NULL){
            if(p->filedes.pid == pid){
                printf(" %-4s %7d %6d  %-10s  %-7d\n", " ", p->filedes.pid, p->filedes.fd, p->filedes.filename, p->filedes.inode);
            }
            p = p->next;
        }
    }    
    printf("%8s========================================\n", " ");
}


// this function prints the threshold information depending on the number of threshold
void print_threshold_information(fdNode *head, int threshold){
    printf("## Offending processes:\n");
    fdNode *p = head;
    int pid = p->filedes.pid;
    int count = 0;
    while(p != NULL){
        if(p->filedes.pid == pid){      // if the current node belongs to pid
            count++;    
        }
        // if the next node is not the current node, which means we are done counting the fd, and count > threshold, so print
        if(p->next !=NULL && p->next->filedes.pid != pid && count > threshold){   
            printf("%d (%d), ", pid, count);
            count = 0;      // reset the count
            pid = p->next->filedes.pid;     // reset the pid
            
        }
        else if(p->next == NULL && count > threshold){
            printf("%d (%d), ", pid, count);
        }
        p = p->next;
    }
    printf("\n");
}

// this function save the composite table to a file called compositeTable.txt
void savetxt(fdNode *head, int pid, char *saved_file){
    FILE *file = fopen(saved_file, "w");
    if(file == NULL){
        fprintf(stderr, "Error opening file\n");  // if failed,  print the error
        exit(1);    // exit the program
    }

    fprintf(file, " %-4s %7s %6s  %-10s  %-7s\n", " ", "PID", "FD", "Filename", "Inode");
    fprintf(file, "%8s========================================\n", " ");
    int i = 0;
    if(pid == -1){  // if pid is not specified, write the whole composite table to the file
        fdNode *p = head;
        while(p != NULL){
            fprintf(file, " %-4d %7d %6d  %-10s  %-7d\n", i, p->filedes.pid, p->filedes.fd, p->filedes.filename, p->filedes.inode);
            i++;
            p = p->next;
        }
    }
    else{   // if pid is specified, only write the information about that specified pid
        fdNode *p = head;
        while(p != NULL){
            if(p->filedes.pid == pid){
                fprintf(file, " %-4s %7d %6d  %-10s  %-7d\n", " ", p->filedes.pid, p->filedes.fd, p->filedes.filename, p->filedes.inode);
            }
            p = p->next;
        }
    }    
    fprintf(file, "%8s========================================\n", " ");
    fclose(file);
}

// this function save the composite table to a file called compositeTable.bin
void savebinary(fdNode *head, int pid, char *saved_file){
    FILE *file = fopen(saved_file, "wb");
    if(file == NULL){    //checking if successfully open the file
        fprintf(stderr, "Error opening file\n");  // if failed,  print the error
        exit(1);    // exit the program
    }

    char header[] = "    PID    FD     Filename     Inode\n==========================";
    fwrite(header, sizeof(char), sizeof(header), file);
    int i = 0;
    if(pid == -1){  // if pid is not specified, write the whole composite table to the file
        fdNode *p = head;
        while(p != NULL){
            fwrite(&i, sizeof(int), 1, file);   
            fwrite(&(p->filedes), sizeof(FileDescriptor), 1, file);
            i++;
            p = p->next;
        }
    }
    else{   // if pid is specified, only write the information about that specified pid
        fdNode *p = head;
        while(p != NULL){
            if(p->filedes.pid == pid){
                fwrite(&(p->filedes), sizeof(FileDescriptor), 1, file);
            }
            p = p->next;
        }
    }    
    fclose(file);    // close the file
}