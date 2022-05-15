#include "main.h"
#include "application.h"
#include "log_libs.h"
#include "config.h"

void ReadCommandBuf(void)
{
    static char buff[32];
    static uint8_t pos = 0;
    int32_t key = LogLibsGetChar();
    if (key < 0)
    {
        // continue;
        return;
    }
    if ((char)key == '\n')
    {
        if (pos == 0)
            // continue;
            return;
        buff[pos] = '\0';
        pos = 0;
        // new string
    }
    else if (pos < (sizeof(buff) - 1))
    {
        buff[pos++] = key;
    }
}

void indication(void)
{
    static uint32_t prev_tick = 0;
    if (HAL_GetTick() > (prev_tick + SYSTEM_LED_BLINK_PEROD))
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        prev_tick = HAL_GetTick();
    }
}

void application(void)
{
    while (1)
    {
        ReadCommandBuf();
        indication();
    }
}