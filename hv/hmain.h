#ifndef HV_MAIN_H_
#define HV_MAIN_H_

#include "hplatform.h"
#include "hdef.h"
#include "hstring.h"
#include "hproc.h"

#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib") // for timeSetEvent
#endif

typedef struct main_ctx_s {
    char    run_dir[MAX_PATH];
    char    program_name[MAX_PATH];

    char    confile[MAX_PATH]; // default etc/${program}.conf
    char    pidfile[MAX_PATH]; // default logs/${program}.pid
    char    logfile[MAX_PATH]; // default logs/${program}.log

    pid_t   pid;    // getpid
    pid_t   oldpid; // getpid_from_pidfile

    // arg
    int     argc;
    int     arg_len;
    char**  os_argv;
    char**  save_argv;
    char*   cmdline;
    keyval_t        arg_kv;
    hv::StringList  arg_list;

    // env
    int     envc;
    int     env_len;
    char**  os_envp;
    char**  save_envp;
    keyval_t    env_kv;

    // signals
    procedure_t reload_fn;
    void*       reload_userdata;
    // master workers model
    int             worker_processes;
    int             worker_threads;
    procedure_t     worker_fn;
    void*           worker_userdata;
    proc_ctx_t*     proc_ctxs;
} main_ctx_t;

// arg_type
#define NO_ARGUMENT         0
#define REQUIRED_ARGUMENT   1
#define OPTIONAL_ARGUMENT   2
// option define
#define OPTION_PREFIX   '-'
#define OPTION_DELIM    '='
#define OPTION_ENABLE   "on"
#define OPTION_DISABLE  "off"
typedef struct option_s {
    char        short_opt;
    const char* long_opt;
    int         arg_type;
} option_t;

HV_EXPORT int main_ctx_init(int argc, char** argv);
// ls -a -l
// ls -al
// watch -n 10 ls
// watch -n10 ls
HV_EXPORT int parse_opt(int argc, char** argv, const char* opt);
// gcc -g -Wall -O3 -std=cpp main.c
HV_EXPORT int parse_opt_long(int argc, char** argv, const option_t* long_options, int size);
HV_EXPORT const char* get_arg(const char* key);
HV_EXPORT const char* get_env(const char* key);

#ifdef OS_UNIX
HV_EXPORT void setproctitle(const char* title);
#endif

// pidfile
HV_EXPORT int   create_pidfile();
HV_EXPORT void  delete_pidfile();
HV_EXPORT pid_t getpid_from_pidfile();

// signal=[start,stop,restart,status,reload]
HV_EXPORT int  signal_init(procedure_t reload_fn = NULL, void* reload_userdata = NULL);
HV_EXPORT void signal_handle(const char* signal);
#ifdef OS_UNIX
// we use SIGTERM to quit process, SIGUSR1 to reload confile
#define SIGNAL_TERMINATE    SIGTERM
#define SIGNAL_RELOAD       SIGUSR1
void signal_handler(int signo);
#endif

// global var
#define DEFAULT_WORKER_PROCESSES    4
#define MAXNUM_WORKER_PROCESSES     256
HV_EXPORT extern main_ctx_t   g_main_ctx;

// master-workers processes
HV_EXPORT int master_workers_run(
        procedure_t worker_fn,
        void* worker_userdata = NULL,
        int worker_processes = DEFAULT_WORKER_PROCESSES,
        int worker_threads = 0,
        bool wait = true);

#endif // HV_MAIN_H_
