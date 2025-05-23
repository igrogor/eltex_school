#ifndef HEADER_H_
#define HEADER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


uint32_t moveip(const char *ip);
int is_in_subnet(uint32_t ip, uint32_t mask, uint32_t net);
#endif