#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "print_table.c"

int main(int argc, char **argv){
    fdNode *head = get_information();   // get the information
    bool process_flag = false;  // default bool for process_flag
    bool systemwide_flag = false;   // default bool for systemwide_flag
    bool vnode_flag = false;    // default bool for vnode_flag
    bool composite_flag = false;    // default bool for composite_flag
    bool threshold_flag = false;    // default bool for threshold_flag
    bool txt_flag = false;     // default bool for txt_flag
    bool binary_flag = false;   // default bool for binary_flag
    int threshold = -1;     // default int for threshold
    int doubt_pid = -1;     // default int for doubt_pid
    int pid = -1;       // default int for pid

    if(argc == 1){  // if the user doesn't specify anything
        print_composite_table(head, pid);   // print the whole composite table
    }
    else{
        for(int i = 1; i < argc; i++){       // Start from index 1 to skip the program name (argv[0])
            char *token = strtok(argv[i], "=");      // if the user enters --threshold=X, split the input
            if(strcmp(token, "--threshold") == 0){    // split at =, check if input is correct
                token = strtok(NULL, "");
                if(token != NULL){      // if X is nonempty
                    threshold = atoi(token);      // convert X to integer, and store in threshold
                    threshold_flag = true;      // turn threshold_flag to true
                } 
            }
            else if(strcmp(argv[i], "--per-process") == 0){     // if the user enters --per-percess
                process_flag = true;    // turn process_flag to true
            }
            else if(strcmp(argv[i], "--systemWide") == 0){      // if the user enters --systemWide
                systemwide_flag = true;     // turn systemwide_flag to true
            }
            else if(strcmp(argv[i], "--Vnodes") == 0){      // if the user enters --Vnode
                vnode_flag = true;      // turn vnode_flag to true
            }
            else if(strcmp(argv[i], "--composite") == 0){       // if the user enters --composite
                composite_flag = true;      // turn composite_flag to true
            }
            else if(strcmp(argv[i], "--output_TXT") == 0){  // if the user enter --output_binary
                txt_flag = true;    // turn txt_flag to true
            }
            else if(strcmp(argv[i], "--output_binary") == 0){   // if the user enter --output_binary
                binary_flag = true;     // turn binary_flag to true
            }
            else if(check_valid_integer(argv[i])){  // check if the enter number is an integer
                doubt_pid = strtol(argv[i], NULL, 10);      // convert entered number to integer
                uid_t current_uid = geteuid();      // get the uid
                if(check_exist_pid(pid, current_uid)){      // check if the uid belongs to the current user
                    pid = doubt_pid;      // store the entered number to pid
                    composite_flag = true;      // turn composite_flag to true
                }
                else{   // if does not belong to the current user
                    printf("INVALID PID\n");        // invalid pid
                    return 1;   // exit the program
                }
            }
            else{
                printf("INVALID INPUT\n");      // print invalid input
                return 1;     // exit the program
            }
        }
    }

    if(process_flag){     // if the user enters --per-percess
        print_process_table(head, pid);     
    }
    if(systemwide_flag){    // if the user enters --systemWide
        print_systemwide_table(head, pid);
    }
    if(vnode_flag){     // if the user enters --Vnode
        print_vnode_table(head, pid);
    }
    if(composite_flag || (!process_flag && !systemwide_flag && !vnode_flag && pid != -1)){     // if the user enters --composite or if the user enters the pid
        print_composite_table(head, pid);
    }
    if(threshold_flag){     // if the user enters --threshold=X
        print_composite_table(head, pid);
        print_threshold_information(head, threshold);
    }
    if(txt_flag){   // if the user enters --output_TXT
        savetxt(head, pid, "compositeTable.txt");
    }
    if(binary_flag){    // if the user enter --output_binary
        savebinary(head, pid, "compositeTable.bin");
    }

    deleteList(head);   // free all memory allocated for list
    return 0;
}