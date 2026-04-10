// system calls
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int open(const char*, int);
int exec(char*, char**);
int uptime(void);
int sleep(int);
int getpid(void);
int mknod(const char*, short, short);
int fork(void);
int wait(void);
int exit(void) __attribute__((noreturn));
int kill(int);

// ulib.c
void printf(int, const char*, ...);
char* gets(char*, int);
int strcmp(const char*, const char*);
