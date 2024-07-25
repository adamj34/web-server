#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "threadsafe_queue.hpp"
#include <atomic>
#include <cassert>
#include <functional>
#include <thread>
#include <vector>

// clean up the threads before the object is destroyed
class join_threads {
    private:
        std::vector<std::thread>& worker_threads;

    public:
        explicit join_threads(std::vector<std::thread>& threads_)
            : worker_threads{threads_} {}

        // join all worker_threads before the object is destroyed
        ~join_threads() {
            for (auto& thread : worker_threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }

        join_threads(join_threads const&) = delete;
        join_threads& operator=(join_threads const&) = delete;
};

class thread_pool {
    private:
        std::atomic_bool is_pool_active;
        threadsafe_queue<std::function<void()>> work_queue;
        std::vector<std::thread> worker_threads;
        join_threads joiner;

        // worker thread checks if there is a task in the queue
        void worker_thread() {
            while (is_pool_active) {
                std::function<void()> task;
                if (work_queue.try_pop(task)) {
                    task();
                } else {
                    // give up the rest of the thread's CPU time slice
                    std::this_thread::yield();
                }
            }
        }

    public:
        thread_pool()
            : is_pool_active{true}
            , joiner{worker_threads} {
            unsigned const thread_count = std::thread::hardware_concurrency() - 6; // magic num to delete
            assert(thread_count > 0 && "Hardware concurrency is not detected!");
            // initialize the worker threads
            try {
                for (unsigned i = 0; i < thread_count; ++i) {
                    worker_threads.emplace_back(std::thread(&thread_pool::worker_thread, this));
                }
            } catch (...) {
                is_pool_active = false;
                throw;
            }
        }

        ~thread_pool() { is_pool_active = false; }

        template <typename FunctionType>
        void submit(FunctionType f) {
            work_queue.push(std::function<void()>(f));
        }
};

#endif // THREAD_POOL_HPP
