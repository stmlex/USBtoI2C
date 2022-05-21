#ifndef APPLICATION_H
#define APPLICATION_H

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif
void application(void);
void LED_set(bool state);
#ifdef __cplusplus
}
#endif
#endif // !APPLICATION_H