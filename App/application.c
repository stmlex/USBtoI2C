#include "main.h"
#include "application.h"
#include "log_libs.h"
#include "config.h"
#include "usbd_cdc_if.h"
#include "stdbool.h"
#include "cli_handle.h"

void LogLibsPrintCustom(char *buff, int n)
{
    CDC_Transmit_FS((uint8_t *)buff, n);
}

void LED_set(bool state)
{
    state ? HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

void application(void)
{
    while (1)
    {
        CliReadTaskFunc();
    }
}