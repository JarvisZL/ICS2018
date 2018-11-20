#include "fs.h"
#include "unistd.h"
#include "klib.h"
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  off_t open_offset;// pointer while opening  
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}


/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0,  invalid_read, invalid_write},
  {"stdout", 0, 0, 0,  invalid_read, invalid_write},
  {"stderr", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
extern size_t ramdisk_read(void *buf,size_t offset,size_t len);
extern size_t ramdisk_write(const void* buf,size_t offset,size_t len);

int fs_open(const char *pathname,int flags,int mode)
{
     static int index=0;
     for(index=0;index<NR_FILES;++index)
     {
         if(strcmp(pathname,file_table[index].name)==0)
         {
             file_table[index].open_offset=0;
             return index;
         }
     }
     assert(0);
}

int fs_close(int fd)
{
   return 0;
}

size_t fs_filesz(int fd)
{
    return file_table[fd].size;
}

off_t fs_lseek(int fd,off_t offset,int whence)
{
   switch(whence)
   {
       case SEEK_SET: {//SEEK_SET
                   assert(offset<=file_table[fd].size);
                   file_table[fd].open_offset=offset;
                   return file_table[fd].open_offset;
               }
       case SEEK_CUR: {//SEEK_CUR
                   assert(file_table[fd].open_offset+offset<=file_table[fd].size);
                   file_table[fd].open_offset+=offset;
                   return file_table[fd].open_offset;
               }
       case SEEK_END: {//SEEK_END
                  assert(file_table[fd].size+offset<=file_table[fd].size);
                  file_table[fd].open_offset=file_table[fd].size+offset;
                  return file_table[fd].open_offset;
               }
       default: TODO();
   }
}

static ssize_t ret_val; 
ssize_t fs_read(int fd,void *buf,size_t len)
{
   if(file_table[fd].open_offset==file_table[fd].size-1&&len!=0)
         return -1;
   if(len==0)
         return 0;
   if(file_table[fd].open_offset+len>file_table[fd].size)
       len=file_table[fd].size-file_table[fd].open_offset-1;
   Log("len:%d\n",len);
   ret_val=ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len); 
   file_table[fd].open_offset+=ret_val;
   return ret_val;
}

ssize_t fs_write(int fd,const void *buf,size_t len)
{
  assert((file_table[fd].open_offset+len)<=file_table[fd].size);
  ret_val= ramdisk_write(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
  file_table[fd].open_offset+=ret_val;
  return ret_val;
}

