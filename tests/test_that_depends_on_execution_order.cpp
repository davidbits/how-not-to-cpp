// test_that_depends_on_execution_order.cpp
//
// BAD IDEA: tests that share global mutable state and depend on running in a specific order
// WHY SOMEONE DOES IT: tests grew organically; it was "easier" to share setup
// WHY IT'S TERRIBLE: tests are not isolated — one failure cascades to all that follow;
//   you can't run a subset of tests; passing order ≠ passing in isolation
// HOW TO FIX IT: each test sets up and tears down its own state; no global state between tests

#include "everything.hpp"

// Shared "test state." Modified by tests. Depended on by later tests.
// This is the problem, distilled.
static int  G_TEST_COUNTER       = 0;
static bool G_WAS_INITIALIZED    = false;
static vector<string> G_TEST_LOG;
static int  G_TESTS_PASSED       = 0;
static int  G_TESTS_FAILED       = 0;

// "Test framework." Minimal and stateful.
#define ASSERT_EQ(A, B) do { \
    if ((A) == (B)) { \
        cout << "  PASS: " #A " == " #B << endl; \
        G_TESTS_PASSED++; \
    } else { \
        cout << "  FAIL: " #A " (" << (A) << ") != " #B " (" << (B) << ")" << endl; \
        G_TESTS_FAILED++; \
        G_TEST_LOG.push_back("FAIL: " #A " != " #B); \
    } \
} while(0)

#define ASSERT_TRUE(X) ASSERT_EQ((X), true)

// Test 1: initializes the global counter.
// Must run first. This is not enforced anywhere.
void test_initialization() {
    cout << "[test_initialization]" << endl;
    G_TEST_COUNTER    = 0;
    G_WAS_INITIALIZED = true;
    ASSERT_TRUE(G_WAS_INITIALIZED);
    ASSERT_EQ(G_TEST_COUNTER, 0);
}

// Test 2: increments the counter.
// Assumes test_initialization() ran and set G_TEST_COUNTER to 0.
// If you run this test alone, it fails — G_TEST_COUNTER starts at whatever it was.
void test_increment() {
    cout << "[test_increment]" << endl;
    G_TEST_COUNTER++;
    ASSERT_EQ(G_TEST_COUNTER, 1);  // only true if test_initialization ran first
}

// Test 3: checks that the counter can be doubled.
// Depends on test_increment() having run — expects G_TEST_COUNTER to be 1.
void test_double() {
    cout << "[test_double]" << endl;
    G_TEST_COUNTER *= 2;
    ASSERT_EQ(G_TEST_COUNTER, 2);  // only true if test_increment ran and set it to 1
}

// Test 4: tests that the log is populated.
// G_TEST_LOG is only populated when a test FAILs.
// This test assumes some tests may have failed, which is not a valid test invariant.
void test_log_populated() {
    cout << "[test_log_populated]" << endl;
    // "test" that the log has at most 0 failures — passes when nothing else failed
    // but depends on all prior tests having passed, which depends on their ordering
    ASSERT_EQ(G_TEST_LOG.size(), (size_t)0);
}

// Test 5: teardown. Must run last. Also not enforced.
void test_teardown() {
    cout << "[test_teardown]" << endl;
    G_WAS_INITIALIZED = false;
    G_TEST_COUNTER    = -1;
    ASSERT_TRUE(!G_WAS_INITIALIZED);
}

int main() {
    cout << "=== Tests (order-dependent) ===" << endl;
    cout << "Running in the only correct order. Hopefully." << endl;

    // These must be called in this exact order.
    // There is no framework enforcing this.
    // A well-meaning contributor will add a test in the wrong place someday.
    test_initialization();
    test_increment();
    test_double();
    test_log_populated();
    test_teardown();

    cout << "\nResults: " << G_TESTS_PASSED << " passed, " << G_TESTS_FAILED << " failed." << endl;

    // Return exit code based on failures — at least this part is right
    return G_TESTS_FAILED == 0 ? 0 : 1;
}
