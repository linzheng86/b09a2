#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// def the struct FileDescroptor, which has four fields
typedef struct FileDescriptor{
    int pid;
    int fd;
    char filename[1024];
    int inode;
}FileDescriptor; 

// det the struct FileDescriptor_node, which has two fields, one id FileDesctiptor, one is a pointer
typedef struct FileDescriptor_node{
    FileDescriptor filedes;
    struct FileDescriptor_node *next;
}fdNode;

// this function create a new node, and initalize the node to the default information
fdNode *newfdNode(){
    fdNode *new = NULL;
    new = (fdNode*)calloc(1, sizeof(fdNode));
    new->filedes.pid = -1;
    new->filedes.fd = -1;
    strcpy(new->filedes.filename, "");
    new->filedes.inode = -1;
    new->next = NULL;
    return new;
}

// this function create a new node depends on the given information, and insert the node at the end of the linked list
fdNode *insertfdNode(fdNode *head, int pid, int fd, char filename[1024], int inode){
    fdNode *new = newfdNode();
    new->filedes.pid = pid;
    new->filedes.fd = fd;
    strcpy(new->filedes.filename, filename);
    new->filedes.inode = inode;
    if(head == NULL){
        return new;
    }
    fdNode *p = head;
    while(p->next != NULL){
        p = p->next;    // loop to the end
    }
    p->next = new;     // add the new to the end
    return head;
}

fdNode *deleteList(fdNode *head)
{
    fdNode *p = NULL;
    fdNode *q = NULL;
    p = head;
    while (p != NULL)
    {
        q = p -> next;  // keep track of p -> next before freeing p
        free(p);    
        p = q;
    }
    return p;
}