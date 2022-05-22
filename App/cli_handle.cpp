#include "cli_handle.h"

#include <cstdlib>
#include <cstring>
#include <inttypes.h>

#include "config.h"
#include "main.h"
#include <stdio.h>
#include <inttypes.h>
#include "application.h"
#include "string.h"

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
        LOG_RAW_INFO("OK");
        break;
    case HAL_ERROR:
        LOG_RAW_INFO("ERROR");
        break;
    case HAL_BUSY:
        LOG_RAW_INFO("BUSY");
        break;
    case HAL_TIMEOUT:
        LOG_RAW_INFO("TIMEOUT");
        break;
    }
    LOG_RAW_INFO("\r\n");
}

static void Print_hex_array(uint8_t *data, size_t size)
{
    for (int i = 0; i < size; ++i)
    {
        LOG_RAW_INFO("%x ", data[i]);
    }
    LOG_RAW_INFO("\r\n");
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
        {"-r", "[DevAddress] [MemAddress] [MemAddSize] [Size] read i2c registers", [](const char *text) -> bool
         { 
            uint16_t DevAddress;
            uint16_t MemAddress;
            uint16_t MemAddSize;
            uint8_t data[I2C_MAX_DATA_SIZE];
            uint16_t Size;
            uint32_t Timeout = 1000;
            HAL_StatusTypeDef err;
            int sscanf_res = sscanf(text, "%hx %hx %hd %hd", &DevAddress, &MemAddress, &MemAddSize, &Size);
            if(sscanf_res < 4) {
                return false;
            }
            DevAddress = DevAddress << 1; // must be shifted to the left before calling the interface
            err = HAL_I2C_Mem_Read(&I2C_INSTANSE, DevAddress, MemAddress, MemAddSize, data, Size, Timeout);
            if (err == HAL_OK) {
                // LOG_HEXDUMP_INFO(data, Size);
                Print_hex_array(data, Size);
            }else {
                Print_HAL_err(err);
            }
            return true; }},
        {"-w", "[DevAddress] [MemAddress] [MemAddSize] [Data] write one i2c register", [](const char *text) -> bool
         { 
            uint16_t DevAddress;
            uint16_t MemAddress;
            uint16_t MemAddSize;
            uint8_t data[I2C_MAX_DATA_SIZE];
            uint16_t Size;
            uint32_t Timeout = 1000;
            HAL_StatusTypeDef err;
            int sscanf_res = sscanf(text, "%hx %hx %hd %hd", &DevAddress, &MemAddress, &MemAddSize, data);
            if(sscanf_res < 4) {
                return false;
            }
            DevAddress = DevAddress << 1; // must be shifted to the left before calling the interface
            err = HAL_I2C_Mem_Write(&I2C_INSTANSE, DevAddress, MemAddress, MemAddSize, data, 1, Timeout);
            Print_HAL_err(err);
            return true; }},
        {"-s", "[Trials] [Timeout] scan i2c bus", [](const char *text) -> bool
         { 
             uint32_t Trials;
             uint32_t Timeout;
             uint8_t data[I2C_MAX_DATA_SIZE];
             int data_index = 0;
             int sscanf_res = sscanf(text, "%d %d", &Trials, &Timeout);
             if(sscanf_res < 2) {
                return false;
            }
            for (uint16_t addr = 0; addr <= 0xff; ++addr) {
                if (HAL_I2C_IsDeviceReady(&I2C_INSTANSE, (addr << 1), Trials, Timeout) == HAL_OK){
                    data[data_index++] = addr;
                }
            }
            if (data_index) {
                // LOG_HEXDUMP_INFO(data, data_index);
                Print_hex_array(data, data_index);
            }
            else {
                LOG_RAW_INFO("not found!\r\n");
            }
            return true; }},
};

void CliReadTaskFunc(void)
{
    static char buff[READ_COMMAND_BUF_LEN] = {0};
    static uint8_t pos = 0;
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
