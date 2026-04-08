// global_state_manager_singleton_factory.hpp
//
// BAD IDEA: global mutable state wrapped in a singleton, exposed via a factory
// WHY SOMEONE DOES IT: "we only ever need one of these"
// WHY IT'S TERRIBLE: untestable, hidden coupling, initialization order fiasco,
//   threading nightmares, impossible to reason about lifetime
// HOW TO FIX IT: dependency injection; pass what you need, don't reach for globals

#pragma once

#include "everything.hpp"

// The one true state. Do not question it.
struct GlobalApplicationState {
    int         errorCount    = 0;
    int         warningCount  = 0;  // unused but feels important
    bool        isInitialized = false;
    bool        isReady       = false;
    bool        isRunning     = false;
    bool        isDone        = false;
    bool        isFailed      = false;  // can be true at same time as isRunning
    string      lastError     = "";
    string      lastWarning   = "";
    string      currentUser   = "unknown";  // set once, read everywhere
    vector<string> eventLog;               // grows forever, never pruned
    map<string, string> configMap;         // also grows forever
};

// The Singleton. Capital S intentional.
class GlobalStateManager {
public:
    // Get the one true instance. There can be only one.
    // (unless you call this from two threads simultaneously, in which case: good luck)
    static GlobalStateManager& getInstance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new GlobalStateManager();  // leaked intentionally: "the OS cleans up"
        }
        return *s_pInstance;
    }

    GlobalApplicationState* getState() {
        return m_pState;  // raw pointer, naturally
    }

    void setState(GlobalApplicationState* pState) {
        // note: we don't free the old state. that would be premature.
        m_pState = pState;
    }

    void logEvent(string event) {
        m_pState->eventLog.push_back(event);
        // no timestamp, no severity, no max size
    }

private:
    GlobalStateManager() {
        m_pState = new GlobalApplicationState();  // also leaked
    }

    // Deleted to enforce singleton. Copy and assignment deleted. Move: not considered.
    GlobalStateManager(const GlobalStateManager&) = delete;
    GlobalStateManager& operator=(const GlobalStateManager&) = delete;

    static GlobalStateManager* s_pInstance;
    GlobalApplicationState*    m_pState;
};

// Definition lives in the header because why not
// (ODR violation waiting to happen in any multi-TU build)
inline GlobalStateManager* GlobalStateManager::s_pInstance = nullptr;

// The Factory. For when getInstance() felt too direct.
class GlobalStateManagerFactory {
public:
    static GlobalStateManager* createManager() {
        // "creates" a manager by returning the existing singleton
        // the Factory pattern is very important here
        return &GlobalStateManager::getInstance();
    }

    static GlobalStateManager* getManager() {
        // identical to createManager(), but semantically different
        return createManager();
    }
};
