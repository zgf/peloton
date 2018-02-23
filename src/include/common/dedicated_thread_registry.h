//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// dedicated_thread_registry.h
//
// Identification: src/include/common/dedicated_thread_registry.h
//
// Copyright (c) 2015-2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <thread>
#include "common/macros.h"
#include "common/dedicated_thread_task.h"

namespace peloton {

class DedicatedThreadOwner;
/**
 * Singleton class responsible for maintaining and dispensing long running
 * (dedicated) threads to other system components. The class also serves
 * as a control panel for the brain component to be able to collect information
 * on threads in the system and modify how threads are allocated.
 */
class DedicatedThreadRegistry {
 public:
  DedicatedThreadRegistry() = default;

  ~DedicatedThreadRegistry();

  // TODO(tianyu): Remove when we remove singletons
  static DedicatedThreadRegistry &GetInstance();

  /**
   *
   * Register a thread under requester to run the given task
   *
   * @param requester The owner to assign the new thread to
   * @param args the arguments to pass to constructor of task
   * @return the DedicatedThreadTask running on new thread
   */
  template <typename Task>
  void RegisterDedicatedThread(DedicatedThreadOwner *requester,
                      std::shared_ptr<Task> task);

  // TODO(tianyu): Add code for thread removal

 private:
  // Using raw pointer is okay since we never dereference said pointer,
  // but only use it as a lookup key
  std::unordered_map<DedicatedThreadTask *, std::thread> threads_table_;
  // Using raw pointer here is also fine since the owner's life cycle is
  // not controlled by the registry
  std::unordered_map<DedicatedThreadOwner *,
                     std::vector<std::shared_ptr<DedicatedThreadTask>>>
      thread_owners_table_;
};

}  // namespace peloton
