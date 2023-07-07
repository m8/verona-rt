// Copyright Microsoft and Project Verona Contributors.
// SPDX-License-Identifier: MIT
#pragma once

#include <coroutine>

namespace verona::rt
{
  struct vpromise;

  struct vcoroutine : std::coroutine_handle<vpromise>
  {
      using promise_type = verona::rt::vpromise;
  };
  
  struct vpromise
  {
      vcoroutine get_return_object() { return {vcoroutine::from_promise(*this)}; }
      std::suspend_always initial_suspend() noexcept { return {}; }
      std::suspend_always final_suspend() noexcept { return {}; }
      void unhandled_exception() {}
      auto yield_value(int) { return std::suspend_always{}; }
      void return_void() {}
  };
}