#pragma once
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <type_traits>

namespace os {

struct os_abstraction {
public:
  void wait();
  void resumeFromISR();
  using queue_handle = QueueHandle_t;

  template <size_t N, typename T = uint8_t> struct queue {
    QueueHandle_t xQueue;
    StaticQueue_t xStaticQueue;
    uint8_t ucQueueStorageArea[N * sizeof(T)]{};
    std::enable_if_t<(N > 0)> create() {
      xQueue = xQueueCreateStatic(N, sizeof(T), ucQueueStorageArea, &xStaticQueue);
    }
    void push(T x) {
      if (xQueue != nullptr) { xQueueSendToBack(xQueue, x, portMAX_DELAY); }
    }
    void push_fromISR(T x) {
      if (xQueue != nullptr) { xQueueSendToBackFromISR(xQueue, x, portMAX_DELAY); }
    }
    T pop() {
      T t;
      xQueueReceive(xQueue, &t, portMAX_DELAY);
      return t;
    }
  };

private:
  TaskHandle_t thread;
};

} // namespace os
