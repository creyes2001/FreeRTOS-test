#ifndef OS_PORT_H
#define OS_PORT_H

#ifdef USE_FREERTOS

	#include "FreeRTOS.h"
    #include "task.h"

    /* Task-context critical section */
    #define OS_ENTER_CRITICAL()        taskENTER_CRITICAL()
    #define OS_EXIT_CRITICAL()         taskEXIT_CRITICAL()

    /* ISR-context critical section — needs a saved-status variable */
    #define OS_ENTER_CRITICAL_ISR(s)   do { (s) = taskENTER_CRITICAL_FROM_ISR(); } while(0)
    #define OS_EXIT_CRITICAL_ISR(s)    taskEXIT_CRITICAL_FROM_ISR(s)
    #define OS_ISR_STATUS_T            UBaseType_ti

#else  /* bare-metal */
    #include "stm32f446xx.h"

    #define OS_ENTER_CRITICAL()        __disable_irq()
    #define OS_EXIT_CRITICAL()         __enable_irq()

    /* Bare-metal ISR critical section: save PRIMASK, disable, restore */
    #define OS_ENTER_CRITICAL_ISR(s)   do { (s) = __get_PRIMASK(); __disable_irq(); } while(0)
    #define OS_EXIT_CRITICAL_ISR(s)    __set_PRIMASK(s)
    #define OS_ISR_STATUS_T            uint32_t

#endif

#endif //OS_PORT_H
