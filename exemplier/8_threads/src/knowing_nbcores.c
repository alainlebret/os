/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2023 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include<mach/mach_host.h>
#include<mach/vm_map.h>
#include <mach/mach.h>
#endif

/**
 * @file knowing_cores.c
 *
 * A simple program used to know how much cores on your Linux system.
 */

static int get_num_cpus(void) {
    processor_cpu_load_info_t newCPUInfo;
    kern_return_t kr;
    unsigned int processor_count;
    mach_msg_type_number_t load_count;

    kr = host_processor_info(mach_host_self(),
                             PROCESSOR_CPU_LOAD_INFO,
                             &processor_count,
                             (processor_info_array_t * ) & newCPUInfo,
                             &load_count);
    if (kr != KERN_SUCCESS) {
        return 0;
    } else {
        vm_deallocate(mach_task_self(),
                      (vm_address_t) newCPUInfo,
                      (vm_size_t)(load_count * sizeof(*newCPUInfo)));

        return (int) processor_count;
    }
}

int main(void) {
    int cores;

#ifdef __APPLE__
    FILE *f = popen("grep processor /proc/cpuinfo | wc -l", "r");
    if (!f) {
        perror("popen");
        exit(EXIT_FAILURE);
    };
    fscanf(f, "%d", &cores);
    pclose(f);
#else
    cores = get_num_cpus();
#endif

    printf("Number of cores: %d\n", cores);

    return EXIT_SUCCESS;
}
