#include <coroutine>
#include <iostream>
 
struct promise;
 
struct coroutine : std::coroutine_handle<promise>
{
    using promise_type = ::promise;
};
 
struct promise
{
    coroutine get_return_object() { return {coroutine::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
    auto yield_value(int) { return std::suspend_always{}; }
};
 
void example()
{
    coroutine h = []() -> coroutine
    {
        int counter = 0;
        std::cout << "Counter: " << counter << "\n";
        counter++;
        co_yield 0;

        std::cout << "Now here: " << counter << "\n";
    }();

    h.resume();
    
    std::cout << "Now there\n";
    
    h.resume();
    h.destroy();
}

int main()
{
    example();
}