#include "main.h"
#include "application.h"
#include "log_libs.h"
#include "config.h"
#include "usbd_cdc_if.h"
#include "stdbool.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
void LogLibsPrintCustom(char *buff, int n)
{
    CDC_Transmit_FS((uint8_t*)buff, n);
}

bool ParseCommand(char *buf, size_t len) {
    LOG_RAW_INFO(buf);
    return true;
}

void ReadCommandBuf(void)
{
    static char buff[READ_COMMAND_BUF_LEN];
    static uint8_t pos = 0;
    int32_t key = LogLibsGetChar();
    if (key < 0)
    {
        return;
    }
    if ((char)key == '\n')
    {
        if (pos == 0)
            return;
        buff[pos] = '\0';
        ParseCommand(buff, pos);
        pos = 0; // new string
    }
    else if (pos < (sizeof(buff) - 1))
    {
        buff[pos++] = key;
    }
}

void Indication(void)
{
    static uint32_t prev_tick = 0;
    uint32_t current_tick = HAL_GetTick();
    if (current_tick > (prev_tick + SYSTEM_LED_BLINK_PEROD))
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        prev_tick = current_tick;
    }
}

void application(void)
{
    while (1)
    {
        ReadCommandBuf();
        Indication();
    }
}