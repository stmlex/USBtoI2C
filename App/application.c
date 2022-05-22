#include "main.h"
#include "application.h"
#include "log_libs.h"
#include "config.h"
#include "usbd_cdc_if.h"
#include "stdbool.h"
#include "cli_handle.h"

bool host_com_port_open = false;

void LogLibsPrintCustom(char *buff, int n)
{
    if (!host_com_port_open) {
        return;
    }
    uint32_t attempts = 0;
    while (attempts++ < CDC_TRANSMIT_ATTEMPTS)
    {
        if (CDC_Transmit_FS((uint8_t *)buff, n) == USBD_OK)
        {
            return;
        }
    }
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