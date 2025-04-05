#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <string.h>
#include <stdint.h>


uint32_t moveip(const char *ip)
{
    uint32_t val = 0;
    uint8_t octet;
    char *ip_copy =  strdup(ip);
    char *token = strtok(ip_copy, ".");
    for (size_t i = 0; i < 4; i++)
    {
        if (token != NULL)
        {
            octet=(__uint8_t)atoi(token);
            val=(val<<8)|octet;
            token=strtok(NULL, ".");
        }
    }
    
    free(ip_copy);
    return val;
}

int is_in_subnet(uint32_t ip, uint32_t mask, uint32_t net) {
    if((ip&mask) == (net&mask))return 1;
    return 0;
}