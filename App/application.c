#include "main.h"
#include "application.h"
void application(void)
{
    while (1)
    {
        HAL_Delay(1000);
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
}