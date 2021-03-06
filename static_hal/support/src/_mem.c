
#include "FreeRTOS.h"

// extern void * pvPortMalloc(size_t aNoOfBytes);
// extern void vPortFree(void* aPtr);
#include <stdint.h>
extern void Default_Handler();

//
// Wrapper around newlib's "malloc".
//
// 
void *__wrap_malloc(unsigned int aNoOfBytes) {
  void *ptr = pvPortMalloc(aNoOfBytes);
  // return(pool);
  return ptr;
}

void *_sbrk(unsigned int size) {
  void *ptr = pvPortMalloc(size);

  return ptr;
}
//
// Wrapper around newlib's "free".
//
void __wrap_free(void *aPtr) { vPortFree(aPtr); }



void _exit(int code __attribute__((unused))) {
  // TODO: write on trace
  Default_Handler();
  while (1)
    ;
}

// ----------------------------------------------------------------------------

void __attribute__((weak, noreturn)) abort(void) { _exit(1); }
