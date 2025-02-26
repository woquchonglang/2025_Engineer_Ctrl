#ifndef UART_H_
#define UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>


void uart_init();
void uart_task(void *param);

#ifdef __cplusplus
}
#endif

#endif
