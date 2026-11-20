// coro.hpp -- the coroutine the translated programs' runtimes (csrt.hpp, py.hpp) use for the forms of a language that stop in the middle of a
// body: an iterator or generator (`yield`), an async method (`await`). Stackful and portable C++17 (no co_await): see the comment on the class.
#ifndef CORO_HPP
#define CORO_HPP
#include <condition_variable>
#include <exception>
#include <functional>
#include <mutex>
#include <thread>

namespace corort {

// A coroutine: a function that runs on a thread of its own and hands control back and forth with its caller, one side at a time, so it
// is suspended exactly where it called suspend() and resumed from there. This is how C++17 (which has no coroutines) holds the two forms
// of C# that stop in the middle of a body: an iterator (`yield return`) and an async method (`await`). Stackful, so the suspension can be
// in a loop, a try block, a nested call: whatever the C# was. When a coroutine is dropped before its body ended (a `foreach` left with
// `break`), suspend() throws Cancelled in the body, which unwinds it through its `finally` blocks, as Dispose() runs them in C#.
struct CoroutineCancelled {};

class Coroutine {
public:
    explicit Coroutine(std::function<void(Coroutine&)> body) : body_(std::move(body)) {}
    Coroutine(const Coroutine&) = delete;
    Coroutine& operator=(const Coroutine&) = delete;
    ~Coroutine() {
        if (started_ && !done_) {
            std::unique_lock<std::mutex> lock(m_);
            cancel_ = true;
            switch_in(lock);
        }
        if (thread_.joinable()) thread_.join();
    }
    // Runs the body until it suspends (true) or ends (false); an exception the body ended with is thrown here.
    bool resume() {
        if (done_) return false;
        std::unique_lock<std::mutex> lock(m_);
        switch_in(lock);
        if (error_) {
            std::exception_ptr e = error_;
            error_ = nullptr;
            std::rethrow_exception(e);
        }
        return !done_;
    }
    // From inside the body: give control back to the caller, and wait to be resumed.
    void suspend() {
        std::unique_lock<std::mutex> lock(m_);
        body_turn_ = false;
        cv_.notify_all();
        cv_.wait(lock, [this] { return body_turn_; });
        if (cancel_) throw CoroutineCancelled{};
    }
    bool finished() const { return done_; }
    // The coroutine whose body is running on this thread (null on any other).
    static Coroutine*& current() {
        thread_local Coroutine* c = nullptr;
        return c;
    }

private:
    void switch_in(std::unique_lock<std::mutex>& lock) {
        body_turn_ = true;
        if (!started_) {
            started_ = true;
            thread_ = std::thread([this] { entry(); });
        } else {
            cv_.notify_all();
        }
        cv_.wait(lock, [this] { return !body_turn_; });
    }
    void entry() {
        current() = this;
        try {
            body_(*this);
        } catch (const CoroutineCancelled&) {
        } catch (...) {
            error_ = std::current_exception();
        }
        std::unique_lock<std::mutex> lock(m_);
        done_ = true;
        body_turn_ = false;
        cv_.notify_all();
    }
    std::function<void(Coroutine&)> body_;
    std::thread thread_;
    std::mutex m_;
    std::condition_variable cv_;
    bool started_ = false, body_turn_ = false, done_ = false, cancel_ = false;
    std::exception_ptr error_;
};

}  // namespace corort

#endif  // CORO_HPP
