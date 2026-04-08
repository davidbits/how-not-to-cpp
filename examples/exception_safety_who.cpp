// exception_safety_who.cpp
//
// BAD IDEA: acquire resources manually, then throw exceptions halfway through
// WHY SOMEONE DOES IT: mixing C-style resource management with C++ exceptions
// WHY IT'S TERRIBLE: resources leak whenever an exception is thrown before cleanup;
//   you cannot write correct cleanup code in every catch clause reliably
// HOW TO FIX IT: RAII — constructors acquire, destructors release; use smart pointers

#include "everything.hpp"

// A "connection" that must be opened and closed manually.
struct Connection {
    bool m_bIsOpen = false;
    string m_strHost;

    Connection(string host) : m_strHost(host) {}

    void open() {
        cout << "[Connection] Opening to " << m_strHost << endl;
        m_bIsOpen = true;
    }

    void close() {
        cout << "[Connection] Closing." << endl;
        m_bIsOpen = false;
    }

    ~Connection() {
        if (m_bIsOpen) {
            cout << "[Connection] DESTRUCTOR: was not closed! leaking connection." << endl;
        }
    }
};

// A "file" that must be opened and closed manually.
struct ManagedFile {
    FILE* m_pHandle = nullptr;

    void open(const char* path) {
        m_pHandle = fopen(path, "w");
        cout << "[File] Opened " << path << endl;
    }

    void close() {
        if (m_pHandle) {
            fclose(m_pHandle);
            m_pHandle = nullptr;
            cout << "[File] Closed." << endl;
        }
    }

    ~ManagedFile() {
        if (m_pHandle) {
            cout << "[File] DESTRUCTOR: was not closed! file handle leaked." << endl;
            fclose(m_pHandle);
        }
    }
};

// Processes data using manual resource management + exceptions.
// This function has exactly zero exception-safe code paths.
void processWithResources(bool shouldFail) {
    Connection* CONN = new Connection("db.prod.internal");
    CONN->open();

    int* BUFFER = new int[1024];  // raw allocation

    ManagedFile FILE_HANDLE;
    FILE_HANDLE.open("/tmp/how-not-to-cpp-output.txt");

    cout << "[Process] Resources acquired. About to do work..." << endl;

    if (shouldFail) {
        throw runtime_error("something went wrong mid-transaction");
        // CONN is leaked
        // BUFFER is leaked
        // FILE_HANDLE.close() never called (destructor will catch it though)
        // CONN is on the heap — no destructor called at all
    }

    cout << "[Process] Work done." << endl;

    FILE_HANDLE.close();
    CONN->close();
    delete[] BUFFER;
    delete CONN;
}

int main() {
    cout << "=== Exception Safety: Who? ===" << endl;

    // Case 1: no exception — works fine (by luck)
    cout << "\n-- Run without failure --" << endl;
    try {
        processWithResources(false);
    } catch (exception& E) {
        cout << "Caught: " << E.what() << endl;
    }

    // Case 2: exception thrown — CONN and BUFFER leaked
    cout << "\n-- Run with failure --" << endl;
    try {
        processWithResources(true);
    } catch (exception& E) {
        cout << "Caught: " << E.what() << endl;
        // no cleanup here. we meant to do it. we forgot.
    }

    cout << "\nResources leaked in failure path. RAII would have prevented this." << endl;
    return 0;
}
