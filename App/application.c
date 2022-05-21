#include "main.h"
#include "application.h"
#include "log_libs.h"
#include "config.h"
#include "usbd_cdc_if.h"
#include "stdbool.h"
#include "cli_handle.h"

void LogLibsPrintCustom(char *buff, int n)
{
    CDC_Transmit_FS((uint8_t*)buff, n);
}

void LED_set(bool state){
    state ? HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
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
        CliReadTaskFunc();
        // Indication();
    }
}