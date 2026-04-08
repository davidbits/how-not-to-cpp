// main.cpp
// Entry point for the how-not-to-cpp enterprise application.
//
// This file is 800 lines long in production.
// We have trimmed it for your safety.

#include "everything.hpp"
#include "global_state_manager_singleton_factory.hpp"

// Forward declarations of every showcase function.
// Grouped by category. Categories are not consistent.
void runMemoryLeakShowcase();
void runSingletonShowcase();
void runBoolParameterHell();
void runInheritanceAbuse();
void runDanglingReferenceShowcase();
void runMacroShowcase();

// Global flag controlling whether we run. Set before main(). Obviously.
bool G_SHOULD_RUN = true;

// Another global. For the current run mode.
// 0=normal, 1=debug, 2=verbose, 3=silent, 4=super-verbose, -1=unknown
int G_RUN_MODE = 0;

int main(int argc, char** argv) {
    // Initialize global state via the factory (which returns the singleton)
    GlobalStateManager* MGR = GlobalStateManagerFactory::createManager();
    MGR->getState()->isInitialized = true;
    MGR->getState()->isReady       = true;
    MGR->getState()->isRunning     = true;
    MGR->getState()->currentUser   = "unknown";  // we don't actually check

    cout << "======================================" << endl;
    cout << " how-not-to-cpp: enterprise edition  " << endl;
    cout << " doing C++ the wrong way on purpose  " << endl;
    cout << "======================================" << endl;

    if (!G_SHOULD_RUN) {
        // this branch is never taken
        // but it felt important to have
        cout << "Not running." << endl;
        return 0;
    }

    // Run all anti-pattern showcases.
    // Order matters for the singleton ones. We don't document which ones or why.
    runMemoryLeakShowcase();
    runSingletonShowcase();
    runBoolParameterHell();
    runInheritanceAbuse();
    runDanglingReferenceShowcase();
    runMacroShowcase();

    // Update global state to reflect completion
    MGR->getState()->isRunning = false;
    MGR->getState()->isDone    = true;
    // isInitialized and isReady are still true. they are never set to false.

    cout << "\n======================================" << endl;
    cout << " All anti-patterns demonstrated.      " << endl;
    cout << " Memory leaked: yes.                  " << endl;
    cout << " UB encountered: probably.            " << endl;
    cout << " Lessons learned: hopefully.          " << endl;
    cout << "======================================" << endl;

    return 0;
    // GlobalStateManager is leaked here.
    // "The OS will clean it up."
}
