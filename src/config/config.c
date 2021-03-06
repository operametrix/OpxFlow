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

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "log/log.h"

opxflow_config_t config;

int config_load_from_file(char *file)
{
    FILE *f = NULL;
    uint32_t buffer_len = 100;
    char buffer[buffer_len];
    char *line = NULL;
    char *token = NULL;
    char *saveptr = NULL;

    f = fopen(file, "r");
    if( f == NULL )
    {
        return -1;
    }

    while( fgets(buffer, buffer_len, f) )
    {
        line = buffer;
        /* Remove spaces at beginning of the line */
        while( line[0] == ' ' )
        {
            line++;
        }

        if( line[0] != '#' && line[0] != '\r' && line[0] != '\n' )
        {
            /* Remove terminating \r, \n and ' ' */
            while( line[strlen(line) - 1] == '\r' || line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == ' ' )
            {
                line[strlen(line) - 1] = 0;
            }

            token = strtok_r(line, " ", &saveptr);

            if( token )
            {
                if( !strcmp(token, "test") )
                {
                    
                }
            }
        }
    }

    fclose(f);
    return 0;
}
