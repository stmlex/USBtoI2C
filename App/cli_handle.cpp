#include "cli_handle.h"

#include <cstdlib>
#include <cstring>
#include <inttypes.h>

#include "config.h"
#include "main.h"
#include <stdio.h>
#include <inttypes.h>
#include "application.h"

#define LOG_MODULE_NAME cli
#if defined(LOG_LEVEL_CLI)
#define LOG_MODULE_LEVEL LOG_LEVEL_CLI
#endif
#include "log_libs.h"

static void ShellHelpCmd(void);
static void Print_HAL_err(HAL_StatusTypeDef err)
{
    switch (err)
    {
    case HAL_OK:
        LOG_INFO("OK");
        break;
    case HAL_ERROR:
        LOG_INFO("ERROR");
        break;
    case HAL_BUSY:
        LOG_INFO("BUSY");
        break;
    case HAL_TIMEOUT:
        LOG_INFO("TIMEOUT");
        break;
    }
}

static const textToCmd_t textToCmdList[] =
    {
        {"-h", "Print this help", [](const char *text) -> bool
         { ShellHelpCmd(); return true; }},
        {"-led", "[on/off] led ctrl", [](const char *text) -> bool
         {
             bool result = false;
             if (strstr(text, "on"))
             {
                 LED_set(true);
                 result = true;
             }
             else if (strstr(text, "off"))
             {
                 LED_set(false);
                 result = true;
             }
             return result;
         }},
        {"-r", "[addr],[size] read i2c registers", [](const char *text) -> bool
         { 
            uint16_t DevAddress;
            uint8_t data[I2C_MAX_DATA_SIZE];
            uint16_t Size;
            uint32_t Timeout = 1000;
            HAL_StatusTypeDef err;

            err = HAL_I2C_Master_Receive(&I2C_INSTANSE, DevAddress, data, Size, Timeout);
            if (err != HAL_OK) {
                Print_HAL_err(err);
            }
            return true; }},
};

void CliReadTaskFunc(void)
{
    static char buff[32] = {0};
    static uint8_t pos = 0;

    // for (;;)
    // {
    int8_t key = LogLibsGetChar();
    if (key > 0)
    {
        if (((char)key == '\n') && (pos != 0))
        {
            // new string
            buff[pos] = '\0';
            pos = 0;
            if (!CliParse(buff, textToCmdList, sizeof(textToCmdList) / sizeof(*textToCmdList)))
            {
                LOG_WARNING("Wrong cmd! Help: -h");
            }
        }
        else if (pos < (sizeof(buff) - 1))
        {
            buff[pos++] = key;
        }
    }
    // vTaskDelay(100);
    // }
}

/**
 * @brief
 *
 * @param msgP
 * @param table
 * @param tableLen
 * @return true
 * @return false
 */
bool CliParse(const char *msgP, const textToCmd_t *table, size_t tableLen)
{
    for (size_t i = 0; i < tableLen; ++i)
    {
        size_t len_cmd = strlen(table[i].cmdTextP);
        if (!strncmp(msgP, table[i].cmdTextP, len_cmd))
        {
            if ((msgP[len_cmd] == ' ') || (strlen(msgP) == len_cmd))
                /*get command from table*/
                if (table[i].func)
                {
                    return table[i].func(&msgP[strlen(table[i].cmdTextP)]);
                }
        }
    }
    /*unknown command*/
    return false;
}

/**
 * @brief
 *
 */
void ShellHelpCmd(void)
{
    LOG_INFO("Shell commands:");

    for (size_t i = 0; i < sizeof(textToCmdList) / sizeof(*textToCmdList); ++i)
    {
        LOG_RAW_INFO("%s %s\n\r", textToCmdList[i].cmdTextP, textToCmdList[i].cmdDecrP);
    }
}
