
/*
Copyright (C) 2022 Alexios Zavras
Copyright (C) 2022 Valasiadis Fotios
SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ptrace.h>
#include        <linux/ptrace.h>

/*
 * For each file, we keep its name/path
 * (the argument to the open(2) call that opened it,
 * whether it was opened for reading or for writing
 * (re-using the O_ flags),
 * and a hash of the contents.
 * Not all info is added at the same time.
 */
typedef struct {
    char was_hash_printed;
    char *path;
    char *abspath;
    char *hash;
    char outname[16];
} FILE_INFO;

/*
 * For each (sub-)process we keep its pid,
 * the command line (including all the arguments)
 * the number of files recorded,
 * the actual information on the files,
 * and, while it's running, its current syscall
 * stop info struct.
 */
typedef struct {
    char outname[16];
    pid_t pid;
    char *cmd_line;
    int *finfo;
    int finfo_size;
    struct ptrace_syscall_info state;
    void *entry_info;
    char ignore_one_sigstop;
} PROCESS_INFO;
