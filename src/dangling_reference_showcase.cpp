// dangling_reference_showcase.cpp
//
// BAD IDEA: return references/pointers to locals, store string_views to temporaries,
//   keep iterators past invalidation
// WHY SOMEONE DOES IT: "I want to avoid the copy" — achieves undefined behavior instead
// WHY IT'S TERRIBLE: silently corrupt data, crashes that only happen on alternate Tuesdays,
//   sanitizers will find you, the optimizer is allowed to do anything it wants here
// HOW TO FIX IT: return by value (move semantics make this free), use std::string not
//   string_view when ownership is needed, think about lifetimes

#include "everything.hpp"

// Returns a reference to a local string.
// The string is destroyed when the function returns.
// The reference is a lie.
const string& getGreeting(string name) {
    string LOCAL_GREETING = "Hello, " + name + "!";
    return LOCAL_GREETING;  // the address of a variable about to be vaporized
}

// A "config" object. Short-lived.
struct Config {
    string host     = "localhost";
    string username = "admin";
    string password = "password123";  // also a security showcase
};

// Returns a string_view into a Config that's about to be destroyed.
// string_view is great — when the data it views is still alive.
string_view getHostView() {
    Config TEMP_CONFIG;               // lives on the stack
    return string_view(TEMP_CONFIG.host);  // view into stack memory
    // TEMP_CONFIG destroyed here. string_view now points at freed memory.
}

// Stores a string_view from a temporary. Classic.
struct ConnectionInfo {
    string_view m_Host;     // danger: no ownership
    string_view m_User;     // danger: no ownership

    // Initialize from a temporary string — the string will die, the view won't know
    ConnectionInfo(const string& host, const string& user)
        : m_Host(host), m_User(user) {}
};

// Iterator invalidation: index-based erase shifts elements, so the index
// skips the element immediately after every erased one.
// This silently produces wrong results rather than crashing — the sneakiest kind of bug.
void processItems(vector<int>& items) {
    for (int I = 0; I < (int)items.size(); I++) {
        if (items[I] % 2 == 0) {
            items.erase(items.begin() + I);
            // I is now pointing at the element that slid into the erased slot.
            // We don't decrement I, so it gets skipped on the next iteration.
            // Some even numbers will survive. The result is wrong. No crash to tell you.
        }
    }
}

// Stores pointers into a vector, then causes the vector to reallocate.
// Now all the pointers are dangling.
void vectorPointerDance() {
    vector<int> NUMBERS = {1, 2, 3, 4, 5};

    int* PTR_TO_FIRST = &NUMBERS[0];  // valid... for now

    for (int I = 0; I < 100; I++) {
        NUMBERS.push_back(I);  // may reallocate — PTR_TO_FIRST may now dangle
    }

    // PTR_TO_FIRST might now point at freed memory.
    // This dereference is undefined behavior, but it often "works" just enough
    // to fool you in testing and fail dramatically in production.
    cout << "First element (maybe): " << *PTR_TO_FIRST << endl;
}

void runDanglingReferenceShowcase() {
    cout << "\n=== Dangling Reference Showcase ===" << endl;

    // Dangling reference from getGreeting()
    // Reading the returned reference is UB — local is already destroyed
    // We just call it and move on to demonstrate the pattern
    cout << "Calling getGreeting (returned ref is dangling)..." << endl;
    (void)getGreeting("World");

    // Dangling string_view from getHostView()
    // The view points at memory that was on the stack and is now gone
    string_view HOST_VIEW = getHostView();
    cout << "Got a host view (dangling): " << HOST_VIEW.length() << " chars" << endl;

    // Iterator invalidation
    vector<int> ITEMS = {1, 2, 3, 4, 5, 6, 7, 8};
    cout << "Erasing even items (with undefined behavior)..." << endl;
    processItems(ITEMS);

    // Pointer into reallocating vector
    vectorPointerDance();

    cout << "Still running! (UB is shy today)" << endl;
}
