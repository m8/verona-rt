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
    void return_void() {}
};
 
void example()
{
    auto h = []() -> coroutine
    {
        std::cout << "L1 > Long running task starting ..... " << "\n";
        int counter = 0;
        
        while(counter < 10)
        {
            counter++;
            std::cout << "L1 > Counter: " << counter << "\n";
            if (counter == 5)
            {
                std::cout << "L1 > Yielding at counter = " << counter << "\n";
                co_yield 0;
            }
        }

        std::cout << "L1 > Long running task finished ..... " << "\n";
        co_return;
    }();

    auto h2 = []() -> coroutine
    {
        std::cout << "L2 > Short running task starting ..... " << "\n";
        std::cout << "L2 > Short running task finished ..... " << "\n";

        co_return;
    }();

 
    // Schedule one by one

    bool finished = false;

    while(!finished)
    {
        if (h.done() && h2.done())
        {
            finished = true;
        }
        else
        {
            if (!h.done())
            {
                h.resume();
            }
            if (!h2.done())
            {
                h2.resume();
            }
        }
    }
}

int main()
{
    example();
}