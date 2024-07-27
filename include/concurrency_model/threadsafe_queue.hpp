#ifndef THREADSAFE_QUEUE_HPP
#define THREADSAFE_QUEUE_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template <typename T>
class threadsafe_queue {
    private:
        mutable std::mutex mut;
        std::queue<std::shared_ptr<T>> data_queue;
        std::condition_variable data_cond;

    public:
        threadsafe_queue() noexcept = default;

        void push(T new_value) {
            // allocate memory for the new value (outside the lock)
            std::shared_ptr<T> data{std::make_shared<T>(std::move(new_value))};
            std::lock_guard<std::mutex> lk{mut};
            data_queue.push(data);
            // notify the threads that might be waiting to pop
            data_cond.notify_one();
        }

        void wait_and_pop(T& value) {
            std::unique_lock<std::mutex> lk{mut};
            // if the queue is empty the thread:
            // 1. calls lk.unlock() (so that other threads can eg. push)
            // 2. after notify_one() checks the pred and if true
            // tries to reaquire the lk mutex
            // 3. if pred == false it keeps waiting
            data_cond.wait(lk, [this] { return !data_queue.empty(); });
            value = std::move(*data_queue.front());
            data_queue.pop();
        }

        std::shared_ptr<T> wait_and_pop() {
            std::unique_lock lk{mut};
            data_cond.wait(lk, [this] { return !data_queue.empty(); });
            std::shared_ptr<T> res{data_queue.front()};
            data_queue.pop();
            return res;
        }

        bool try_pop(T& value) {
            std::lock_guard<std::mutex> lk{mut};
            if (data_queue.empty()) {
                return false;
            }
            value = std::move(*data_queue.front());
            data_queue.pop();
            return true;
        }

        std::shared_ptr<T> try_pop() {
            std::lock_guard<std::mutex> lk{mut};
            if (data_queue.empty()) {
                return std::shared_ptr<T>{};
            }
            std::shared_ptr<T> res{data_queue.front()};
            data_queue.pop();
            return res;
        }

        bool empty() const {
            std::lock_guard<std::mutex> lk{mut};
            return data_queue.empty();
        }
};

#endif // THREADSAFE_QUEUE_HPP
