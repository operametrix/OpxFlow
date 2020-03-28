/**
 * OpxFlow
 * Copyright (c) 2020 OperaMetrix SAS
 * Author: Nicolas Gonzalez <ngonzalez@operametrix.fr>
 *
 * This file is part of OpxFlow.
 *
 * OpxFlow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpxFlow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpxFlow.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>

#include "server/server.h"
#include "config/config.h"
#include "log/log.h"

const char* const short_options = "ha:p:l:c:";

const struct option long_options[] = {
    { "help",            0, NULL, 'h'},
    { "address",         0, NULL, 'a'},
    { "port",            0, NULL, 'p'},
    { "log",             0, NULL, 'l'},
    { "config-file",     0, NULL, 'c'},
    { NULL,              0, NULL, 0}
};

static const char* const usage_template =
    "OpxFlow version %s\n"
    "OpxFlow is a high performance MQTT broker.\n\n"
    "usage: %s [ options ]\n\n"
    "-c, --config-file FILE   Configuration file (by default '/etc/opxflow/opxflow.conf').\n"
    "-a, --address ADDR       Bind to local address (by default, bind to all local addresses).\n"
    "-p, --port PORT          Bind to specified port.\n"
    "-l, --log LEVEL          Log level from 0 (critical) to 5 (none). (by default 4)\n\n"
    "See https://operametrix.fr/ for more information.\n";

const char* program_name;

void print_usage()
{
    printf(usage_template, VERSION, program_name);
}

void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        LOG_INFO("OpxFlow version %s terminating", VERSION);
    }
}

int main(int argc, char* argv[])
{
    struct in_addr local_address;
    uint16_t port;
    int next_option;

    local_address.s_addr = INADDR_ANY;
    port = (uint16_t) htons(1883);

    struct hostent *local_host_name;
    long value;
    char* end;
    bool custom_config = false;

    program_name = argv[0];

    signal(SIGINT, sig_handler);

    do
    {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);

        switch(next_option)
        {
            case 'h':
                print_usage();
                exit(0);
                break;

            case 'c':
                if( config_load_from_file(optarg) != 0 )
                {
                    LOG_CRITICAL("Error: Unable to open config file %s", optarg);
                    exit(-1);
                }
                custom_config = true;
                break;

            case 'a':
                local_host_name = gethostbyname(optarg);
                if(local_host_name == NULL || local_host_name->h_length == 0)
                {
                    LOG_CRITICAL("invalid hostname");
                }
                else
                {
                    local_address.s_addr = *((int*) (local_host_name->h_addr_list[0]));
                }
                
                break;

            case 'p':
                value = strtol(optarg, &end, 10);
                port = (uint16_t) htons(value);
                break;

            case 'l':
                value = strtol(optarg, &end, 10);
                if( (value >= LOG_LEVEL_NONE) && (value <= LOG_LEVEL_DEBUG) )
                {
                    log_run_level = (uint8_t) value;
                }

            case -1:
                break;

            default:
                exit(-1);
        }
    }
    while(next_option != -1);

    LOG_INFO("OpxFlow version %s starting", VERSION);

    if( custom_config == false )
    {
        if( config_load_from_file("/etc/opxflow/opxflow.conf") != 0 )
        {
            LOG_INFO("failed to load configuration from /etc/opxflow/opxflow.conf");
        }
    }

    int listener_fd;
    listener_fd = server_create_listener(local_address, port);

    server_run(listener_fd);

    return EXIT_SUCCESS;
}
