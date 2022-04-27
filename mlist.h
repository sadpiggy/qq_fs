#ifndef MLIST_H_
#define MLIST_H_

//#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include<malloc.h>

//root have three cen
struct qq_fs
{
    char *path;//upbounder 50
    char *data;//100
    char *fileName;//upbounder 12
    int isDir;//0 dir 1 file
    struct stat *fileStat; 
};

struct mlist
{
    struct  qq_fs* value;
    struct mlist* next;
    //struct mlist* father;
    struct mlist* brother;
    int index;
};

struct qq_fs* new_qq_fs(){
    struct qq_fs* node = (struct qq_fs*)malloc(sizeof(struct qq_fs));
    node->fileStat = (struct stat*)malloc(sizeof(struct stat));
    node->path = (char*)malloc(50);node->path[0] = '\0';
    node->fileName = (char*)malloc(12);node->fileName[0] = '\0';
    node->data = (char*)malloc(100); node->data[0] = '\0';
    node->isDir = 0;
    return node;
}

struct mlist** new_list(){
    struct mlist** node = (struct mlist**)malloc(sizeof(struct mlist*)*4);
    node[0] = (struct mlist*)malloc(sizeof(struct mlist));
    //node[0]->father = NULL;
    node[0]->index = 0;
    node[0]->next = NULL;
    node[0]->brother = NULL;
    node[0]->value = new_qq_fs();
    node[0]->value->isDir = 0;
   // memcpy((void*)node->value->fileName,(void*)root_name,12);
    node[0]->value->path = "/";
    node[0]->value->fileName = "";
    return node;
}

struct mlist* new_list_node(){
    struct mlist* node = (struct mlist*)malloc(sizeof(struct mlist));
    //node->father = NULL;
    node->index = 0;
    node->next = NULL;
    node->brother = NULL;
    node->value = new_qq_fs();
    return node;
}

struct mlist* insert_list_node(struct mlist** root,const char* path,const char* data, int isDir,struct stat * fileStat){
    int count = 0;
    int i=0;
    while(path[i]!='\0'){
        if(path[i]=='/')count++;
        i++;
    }

    struct mlist* now_node = root[count];


    if(now_node!=NULL){
        while(now_node->brother!=NULL)now_node = now_node->brother;
    }
    struct mlist* node = new_list_node();
    memcpy(node->value->path,path,50);
    int index = 0;
    char* fileName = (char*)malloc(12);
    strcpy(fileName,path);
    while(strchr(fileName,'/')){
        fileName = strchr(fileName,'/')+1;
    }

    memcpy(node->value->fileName,fileName,12);
    if(data!=NULL)
    memcpy(node->value->data,data,100);
    if(fileStat!=NULL)
    memcpy(node->value->fileStat,fileStat,sizeof(struct stat));
    node->value->isDir = isDir;
    //node->father = now_node;
    node->index = count;
    if (now_node!=NULL){
        now_node->brother = node;
    }else{
        root[count] = node;
    }
    
    return node;
}

struct mlist* search_list_node(struct mlist** root,const char* path){
    if(strcmp("/",path)==0)return root[0];
     int count = 0;
    int i=0;
    while(path[i]!='\0'){
        if(path[i]=='/')count++;
        i++;
    }
    struct mlist* now_node = root[count];
    
    while(now_node!=NULL){
        if(strcmp(now_node->value->path,path)==0)return now_node;
        now_node = now_node->brother;
    }
    //if(strcmp(now_node->value->path,path)==0)return now_node;
    
    return NULL;
}



#endif