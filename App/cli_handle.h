#ifndef _CLI_HANDLE
#define _CLI_HANDLE

#include "stdbool.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    const char* cmdTextP;
    const char* cmdDecrP;
    bool (*func)(const char*/*text*/);
} textToCmd_t;

void CliReadTaskFunc(void);
bool CliParse(const char *msg, const textToCmd_t *table, size_t tableLen);

#ifdef __cplusplus
}
#endif

#endif /* _CLI_HANDLE */