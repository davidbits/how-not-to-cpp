// thread_unsafety_demo.cpp
//
// BAD IDEA: shared mutable state accessed from multiple threads without synchronization
// WHY SOMEONE DOES IT: "threads don't run at the same time on my machine" / forgot
// WHY IT'S TERRIBLE: data races are undefined behavior; torn reads, lost updates,
//   crashes that only appear under load, on other OSes, or in production
// HOW TO FIX IT: std::mutex, std::atomic, message passing, or avoid shared state

#include "everything.hpp"

// This is the "thread-safe" counter. The name is aspirational.
struct ThreadSafeCounter {
    int m_nValue = 0;  // not atomic, not protected by a mutex

    void increment() {
        m_nValue++;    // read-modify-write: three operations, zero protection
    }

    void decrement() {
        m_nValue--;    // likewise
    }

    int get() {
        return m_nValue;  // might return a value mid-write on another thread
    }
};

// Global. Shared. Unprotected.
ThreadSafeCounter G_COUNTER;

// A global vector that multiple threads will push_back into simultaneously.
// std::vector is not thread-safe.
// This comment is the only safety mechanism in this file.
vector<int> G_RESULTS;

void workerIncrement(int iterations) {
    for (int I = 0; I < iterations; I++) {
        G_COUNTER.increment();
        G_RESULTS.push_back(I);  // data race: concurrent push_back = UB
    }
}

void workerDecrement(int iterations) {
    for (int I = 0; I < iterations; I++) {
        G_COUNTER.decrement();
    }
}

int main() {
    cout << "=== Thread Un-safety Demo ===" << endl;
    cout << "Starting 4 threads sharing mutable state with no synchronization." << endl;
    cout << "(Results are undefined. That's kind of the point.)" << endl;

    const int ITERATIONS = 10000;

    thread T1(workerIncrement, ITERATIONS);
    thread T2(workerIncrement, ITERATIONS);
    thread T3(workerDecrement, ITERATIONS);
    thread T4(workerDecrement, ITERATIONS);

    T1.join();
    T2.join();
    T3.join();
    T4.join();

    // Expected: 0. Actual: anyone's guess.
    cout << "Counter final value (expected 0): " << G_COUNTER.get() << endl;
    cout << "Results vector size (expected ~20000): " << G_RESULTS.size() << endl;
    cout << "(if this crashed, that's also valid)" << endl;

    return 0;
}
