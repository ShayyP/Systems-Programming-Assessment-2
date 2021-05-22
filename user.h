#include "batch.h"

struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int getch(void);
// TODO: Declare your user APIs for your system calls.
int setvideomode(int mode);
int kern_setpixel(int x, int y, int colour);
int kern_drawline(int x0, int y0, int x1, int y1, int colour);
int sendbatch(BatchCall* batchcalls, int count);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int abs(int num);
void begingraphics();
void endgraphics();
void setpixel(int x, int y, int colour);
void drawline(int x0, int y0, int x1, int y1, int colour);
void drawcircle(int xc, int yc, int radius, int colour);
void fillrect(int x, int y, int width, int height, int colour);
void drawpoly(Point* points, int pointcount, bool fill, int colour);