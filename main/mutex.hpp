#pragma once

#include <mutex>

namespace mutex {
    template<typename T>
    class Mutex;

    template<typename T>
    class MutexGuard {
        friend class Mutex<T>;

        private:
            Mutex<T> &m;
            
            MutexGuard(Mutex<T>& mut): m(mut) {}
        public:
            auto get() const -> T {
                return this->m.value;
            }

            auto get_ref() -> T& {
                return this->m.value;
            }

            ~MutexGuard() {
                this->m.m.unlock();
            }
    };

    template<typename T>
    class Mutex {
        friend class MutexGuard<T>;

        private:
            std::mutex m;
            T value;

            Mutex(T v) : value(v) {}

        public:
            static auto init(T value) -> Mutex<T> {
                return Mutex(value);
            }

            auto lock() -> MutexGuard<T> {
                this->m.lock();
                return MutexGuard(*this);
            }

            ~Mutex() {}
    };
}
