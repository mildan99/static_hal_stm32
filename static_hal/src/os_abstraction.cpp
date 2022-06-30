#include "os_abstraction.hpp"

namespace os {
void os_abstraction::wait() {
//TODO: maybe should tie periphereal to it's thread
  this->thread = xTaskGetCurrentTaskHandle();
  ulTaskNotifyTake(true, portMAX_DELAY);
}

void os_abstraction::resumeFromISR() {
  BaseType_t woken;
  vTaskNotifyGiveFromISR(this->thread, &woken);
  this->thread = 0;
  portYIELD_FROM_ISR(woken);
}

} // namespace os