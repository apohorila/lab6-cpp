#pragma once 
#include <coroutine>
#include <stdexcept>
#include <string>
#include <vector>

class Player {
    public:
    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;
    using result_type = std::pair<int,int>;
    bool in_progress() const { return coro and !coro.done(); }
    bool move_next()
    {
        return in_progress() ? (coro.resume(), !coro.done()) : false;
    }
    result_type current_value_safety() const
    {
        if (coro)
            return coro.promise().result;
        throw std::runtime_error("coroutine is destroyed");
    }
    void set_response(int r)
    {
        if (!coro) throw std::runtime_error("coroutine is destroyed");
        coro.promise().response = r;
    }
    operator bool() const noexcept { return bool(coro); }

    class Awaiter {
        public:
        Awaiter(promise_type& p) : p(p) {}
        bool await_ready() const noexcept { return true; }
        bool await_suspend(std::coroutine_handle<>) noexcept { return false;}
        int await_resume() noexcept { return p.response; }

        private:
        promise_type &p;
    };

    struct promise_type{
        result_type result = {-1,0};
        int response = 0;
        void unhandled_exception() { std::terminate(); }
        auto initial_suspend() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        auto yield_value(int value) 
        {
            
            result.first = value; 
            return std::suspend_always{};
        }
        void return_value(int value) 
        {
            result.first = value;
        }

        auto get_return_object()
        {
            return Player{ handle::from_promise(*this) };
        }

        Awaiter await_transform(int) { return Awaiter{ *this }; };
     
    };
    Player(Player const&) = delete;
    Player& operator=(Player const&) = delete;
    Player(Player&& other) noexcept : coro(other.coro) { other.coro = nullptr; }  
    ~Player()
    {
        if (coro) coro.destroy();
    }
    private :
    Player(handle h) : coro(h) {};
    handle coro;
};

Player play_tic_tac_toe(char player_move);