#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "struct_def.c"

// this function checks if the input is a integer
bool check_valid_integer(char *input) {
    int i = 0;
    while (input[i] != '\0') {      // when the input is nonempty
        if (!isdigit(input[i])) {   //if the input is not integer
            return false;   // return false
        }
        i++;
    }
    return true;    // else, return true
}

// this function checks if it is a pid directory
bool check_pid_dir(const struct dirent *entry){
    const char *p;
    for(p = entry->d_name; *p; p++){
        if(!isdigit(*p)){
            return false;
        }
    }
    return true;
}

// this function checks if the giveen pid belongs to the current user
bool check_exist_pid(int pid, uid_t current_uid){
    char proc_path[1024];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/status", pid);

    // Open the status file
    FILE *file = fopen(proc_path, "r");
    if (file == NULL) {
        return false;
    }

    // Read the status file to get the UID of the process
    char filecontent[1024];
    while (fgets(filecontent, sizeof(filecontent), file) != NULL) {
        // Search for the "Uid:" line in the status file
        if (strstr(filecontent, "Uid:") != NULL) {
            // Parse the UID from the line
            uid_t process_uid;
            sscanf(filecontent, "Uid:\t%u", &process_uid);

            // Check if the process belongs to the current user
            fclose(file);
            if(current_uid == process_uid){
                return true;
            }
            return false;
        }
    }
    fclose(file);
    return false;
}



// this function create the linked list about the disired information
fdNode* get_information(){
    uid_t current_uid = geteuid();  // get the current uid
    fdNode *head = NULL;    // get the head
    char *ptr;      // create the pointer
    DIR *dir = opendir("/proc");     // open the dir
    if(dir == NULL){    // if failed open the directory
        fprintf(stderr, "Error opening directory\n");  // print error
        exit(1);    // exit the program
    }

    struct dirent *entry;     
    while((entry = readdir(dir)) != NULL){     // read the next entry in the directory dir
        // skip anything that is not a PID directory.
        if(!check_pid_dir(entry)){
            continue;
        }
        int pid = strtol(entry->d_name, &ptr, 10);      // convert the pid to integer
        int inode;
        int fd;
        char filename[1024];

        if(check_exist_pid(pid, current_uid) && *ptr == '\0'){  // check if the process belongs to the current user
            // Try to open /proc/<PID>/fd.
            char path[1024];
            snprintf(path, sizeof(path), "/proc/%d/fd", pid);
            DIR *fd_dir = opendir(path);
            if(fd_dir){
                struct dirent *fd_entry;
                while((fd_entry = readdir(fd_dir)) != NULL){
                    if(fd_entry->d_type == DT_LNK){
                        fd = strtol(fd_entry->d_name, &ptr, 10);
                        char path2[1024];
                        snprintf(path2, sizeof(path2), "/proc/%d/fd/%d", pid, fd);
                        char link[1024];
                        ssize_t length = readlink(path2, link, sizeof(link) - 1);
                        if(length == -1){
                            continue;
                        }else{
                            link[length] = '\0';
                            strcpy(filename, link);
                        }
                        if(strstr(filename, "fdinfo") != NULL){
                            continue;
                        }
                        char proc_stat_path[1024];
                        snprintf(proc_stat_path, sizeof(proc_stat_path), "/proc/%d/fdinfo/%s", pid, fd_entry->d_name);

                        FILE *stat_file = fopen(proc_stat_path, "r");
                        if (stat_file != NULL) {
                            char filecontent[1024];
                            while(stat_file){
                                fgets(filecontent, sizeof(filecontent), stat_file);
                                if(sscanf(filecontent, "ino: %d", &inode) == 1){
                                    break;
                                }
                            }
                        } else {
                            continue;
                            // perror("Error opening /proc/PID/stat file");
                        }
                        head = insertfdNode(head, pid, fd, filename, inode);
                    }
                }
                closedir(fd_dir);
            }
        }
    }
    closedir(dir);
    return head;
}