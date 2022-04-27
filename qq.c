/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPLv2.
  See the file COPYING.
*/

/** @file
 *
 * minimal example filesystem using high-level API
 *
 * Compile with:
 *
 *     gcc -Wall hello.c `pkg-config fuse3 --cflags --libs` -o hello
 *
 * ## Source code ##
 * \include hello.c
 */

#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include "mlist.h"




struct mlist** root;


static void *qq_init(struct fuse_conn_info *conn,
                     struct fuse_config *cfg)
{
    root = new_list();
    //if(root==NULL)root = new_list();
    (void)conn;
    cfg->kernel_cache = 0;
    return NULL;
}

static int qq_getattr(const char *path, struct stat *stbuf,
                      struct fuse_file_info *fi)
{
    (void)fi;
    int res = 0;


    memset(stbuf, 0, sizeof(struct stat));
    

    if (strcmp(path, "/") == 0){
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }else{
        struct mlist* qq_node = search_list_node(root,path);
        if (qq_node!=NULL)
        {
            if(qq_node->value->isDir==0){
                stbuf->st_mode = S_IFDIR | 0755;
                stbuf->st_nlink = 2;
            }else{
                stbuf->st_mode = S_IFREG | 0770;
		        stbuf->st_nlink = 1;
		        stbuf->st_size = strlen(qq_node->value->data);
            }
        }else{
            return -ENOENT;
        }
        
    }
    return res;
}

char* get_fatherName(const char* path){
    char* fatherName = (char*)malloc(12);
    int index = 1;
    while(path[index]!='/'){
        index++;
    }
    strcpy(fatherName,path+1);
    fatherName[index-1] = '\0';
    return fatherName;
}

static int qq_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                      off_t offset, struct fuse_file_info *fi,
                      enum fuse_readdir_flags flags)
{
    (void)offset;
    (void)fi;
    (void)flags;

    printf("qq_getreadttr");

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);
    // filler(buf, "cnm", NULL, 0, 0);
    // return 0;

    if(strcmp(path,"/")==0){
        struct mlist* now_node = root[1];
        while(now_node!=NULL){
            filler(buf, now_node->value->path+1, NULL, 0, 0);
            now_node = now_node->brother;
        }
        
    }else{
        struct mlist* qq_node = search_list_node(root,path);
        if (qq_node==NULL)
            return -ENOENT;
        if (qq_node->value->isDir==1)
        {
            filler(buf, qq_node->value->fileName, NULL, 0, 0);
            return 0;
        }
            
        struct mlist* now_node = root[2];//todo
        while(now_node!=NULL){
            if(strcmp(qq_node->value->fileName,get_fatherName(now_node->value->path))==0)
            //if(strcmp(qq_node->value->fileName,get_fatherName(now_node->value->path))==0)
            filler(buf, now_node->value->fileName, NULL, 0, 0);
            now_node = now_node->brother;
        }
    }

    return 0;
}



static int qq_read(const char *path, char *buf, size_t size, off_t offset,
                   struct fuse_file_info *fi)
{
    size_t len;
    (void)fi;
    struct qq_fs* qq_node= search_list_node(root,path)->value;
    if (qq_node==NULL)
        return -ENOENT;
    strcpy(buf,qq_node->data);
    return size;
}

static int qq_access(const char *path, int mask)
{
	return 0;
}

static int qq_mkdir(const char *path, mode_t mode)
{
    int res = 0;
    struct mlist *qq_node = search_list_node(root,path);
    
    if (qq_node!=NULL) {
        return -ENOMEM;      
    }
    insert_list_node(root,path,NULL,0,NULL);


    return res;
}
 



static int qq_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res = 0;
   
    struct mlist *qq_node = search_list_node(root,path);
    
    if (qq_node!=NULL) {
        return -ENOMEM;      
    }
    insert_list_node(root,path,NULL,1,NULL);
    return res;
}


static int qq_open(const char *path, struct fuse_file_info *fi)
{
    int res = 0;
    struct mlist* qq_node = search_list_node(root,path);
   if(qq_node==NULL){
       printf("cnm196\n");
       return -ENOMEM;  
   }
    return res;
}


static int qq_write(const char *path,
                       const char *buf, size_t size, off_t offset,
                       struct fuse_file_info *fi)
{
    struct mlist* qq_node = search_list_node(root,path);
    if(qq_node==NULL)return -ENOMEM;
    int index =   strlen(qq_node->value->data);
    memcpy(qq_node->value->data + index, buf, size);
    qq_node->value->data[index+strlen(buf)] = '\0';

    //反转
    //if(qq_node->index==1)return size;
    char* father_name = get_fatherName(path);
    char* son_name = qq_node->value->fileName;
    if(father_name[strlen(father_name)-1]=='1'){
        father_name[strlen(father_name)-1] = '2';
    }else{
        father_name[strlen(father_name)-1] = '1';
    }
    char* new_path = malloc(50);
    new_path[0] = '/';
    strcpy(new_path+1,father_name);
    index = strlen(new_path);
    new_path[index] = '/';
    strcpy(new_path+index+1,son_name);
    struct mlist* qq_node2 = search_list_node(root,new_path);
    if (qq_node2==NULL)
    {
        qq_node2 = insert_list_node(root,new_path,"",1,NULL);
    }
    index =   strlen(qq_node2->value->data);
    memcpy(qq_node2->value->data + index, buf, size);
    qq_node2->value->data[index+strlen(buf)] = '\0';
   // memcpy(qq_node->value->data, new_path, strlen(new_path));
    
    return size;
} 

static int qq_release(const char *path, struct fuse_file_info *fi)
{

	return 0;
}

static int qq_utimens(const char *path, const struct timespec ts[2],
						   struct fuse_file_info *fi)
{
	(void)fi;

	return 0;
}

static const struct fuse_operations qq_oper = {
    .init = qq_init,
    .getattr = qq_getattr,
    .readdir = qq_readdir,
    .open = qq_open,
    .read = qq_read,

//   .access   = qq_access,
    .utimens = qq_utimens,


   .write   = qq_write,
   .release  = qq_release,

   .mknod   = qq_mknod,

   .mkdir   = qq_mkdir

};


int main(int argc, char *argv[])
{
    // // root = new_list();
    // fuse_main(argc, argv, &qq_oper, NULL);
    int ret;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	ret = fuse_main(args.argc, args.argv, &qq_oper, NULL);
	fuse_opt_free_args(&args);
	return ret;
}