#include <coroutine>
#include <iostream>

using namespace std;

struct generator
{
  struct promise_type
  {
    int current_value;
    auto get_return_object()
    {
      return generator{this};
    }
    auto initial_suspend()
    {
      return suspend_always{};
    }
    auto final_suspend() noexcept
    {
      return suspend_always{};
    }
    void unhandled_exception()
    {
      std::terminate();
    }
    void return_void() {}
    auto yield_value(int value)
    {
      current_value = value;
      return suspend_always{};
    }
  };
  bool move_next()
  {
    return coro ? (coro.resume(), !coro.done()) : false;
  }
  int current_value()
  {
    return coro.promise().current_value;
  }
  generator(generator const&) = delete;
  generator(generator&& rhs) : coro(rhs.coro)
  {
    rhs.coro = nullptr;
  }
  ~generator()
  {
    if (coro)
      coro.destroy();
  }

private:
  generator(promise_type* p)
  : coro(coroutine_handle<promise_type>::from_promise(*p))
  {}
  coroutine_handle<promise_type> coro;
};

generator generate_numbers()
{
  for (int i = 0; i < 10; ++i)
  {
    co_yield i;
  }
}

int main()
{
  auto gen = generate_numbers();
  while (gen.move_next())
  {
    cout << gen.current_value() << endl;
  }

  return 0;
}
