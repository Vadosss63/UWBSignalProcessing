#include "operationmanager.h"
#include <iostream>
#include <memory>

size_t OperationManager::m_countThread = std::thread::hardware_concurrency();

OperationManager::ThreadPool::ThreadPool(size_t countThread)
    : m_operating(true), m_pool(countThread) {
  try {
    for (auto &thread : m_pool)
      thread = std::thread([&]() { ThreadTask(); });
  } catch (...) {
    m_operating.store(false);
  }
}

OperationManager::ThreadPool::~ThreadPool() {
  m_operating.store(false);
  m_operationQueue.clear();

  for (auto &thread : m_pool)
    if (thread.joinable()) {
      thread.join();
    }
}

void OperationManager::ThreadPool::ThreadTask() {
  while (m_operating) {
    Task *module = nullptr;
    if (m_operationQueue.try_pop(module)) {
      module->Operate();
    } else {
      //            std::this_thread::yield();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
}

void OperationManager::ThreadPool::AddTask(Task *module) {
  m_operationQueue.push(module);
}

void OperationManager::StartManager() {
  m_isStart = true;
  StartEvent();
  m_theadPool = std::make_unique<ThreadPool>(m_countThread);
}

void OperationManager::StopManager() {
  if (m_isStart) {
    m_isStart = false;
    std::cout << __PRETTY_FUNCTION__;
    std::cout << std::endl;
    m_stop = std::async(std::launch::async, [&]() { StopThreads(); });
  }
}

void OperationManager::AddTask(Task *module) {
  if (m_theadPool)
    m_theadPool->AddTask(module);
}

size_t OperationManager::countThread() { return m_countThread; }

void OperationManager::setCountThread(size_t countThread) {
  m_countThread = countThread;
}

void OperationManager::StopThreads() {
  StopEvent();
  m_theadPool.reset();
}

void OperationManager::StartEvent() {
  for (auto *event : m_listEvets)
    event->StartEvent();
}

void OperationManager::StopEvent() {
  for (auto *event : m_listEvets)
    event->StopEvent();
}

void OperationManager::AttachEvent(EventOperationManager *event) {
  m_listEvets.push_front(event);
}

void OperationManager::DetachEvent(EventOperationManager *event) {
  m_listEvets.remove(event);
}

AbstractOperationManager *OperationManager::Clone() {
  return new OperationManager;
}
