#pragma once

#include <future>
#include <tuple>
#include <type_traits>

// Utility for testing concurrent code
// It takes any number of callables and starts each on a separate thread, managed by std::async
// Each threads executes the given function only after it receives a signal that all threads
// are ready. Note that the threads will run truly concurrently only if the hardware allows
// that many concurrent threads.

// utility to easily fill a tuple with N default constructed instances of type T
template <typename T, std::size_t = 0>
constexpr auto default_construct() noexcept(std::is_nothrow_default_constructible_v<T>)
{
    return T{};
}

// utility to construct N instances of type T initialized with arguments Args...
template<typename T, std::size_t = 0>
struct construct {
    template<typename... Args>
    auto operator()(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        return T{std::forward<Args>(args)...};
    }
};

template <std::size_t... Is, typename... Fs>
auto even_start_impl(std::index_sequence<Is...>, Fs&&... fs)
{
    std::promise<void> go{};
    auto ready_flags{std::make_tuple(default_construct<std::promise<void>, Is>()...)};
    std::shared_future<void> go_future{go.get_future()};
    auto futures{std::make_tuple(std::async(
        std::launch::async,
        [go_flag{go_future}, &ready = std::get<Is>(ready_flags), f{std::forward<Fs>(fs)}]() {
            ready.set_value();
            go_flag.wait();
            return f();
        })...)};
    (..., std::get<Is>(ready_flags).get_future().wait());
    go.set_value();
    return futures;
    ;
}

template <typename... Fs>
auto even_start(Fs&&... fs)
{
    using Indices = std::make_index_sequence<sizeof...(Fs)>;
    return even_start_impl(Indices{}, std::forward<Fs>(fs)...);
}