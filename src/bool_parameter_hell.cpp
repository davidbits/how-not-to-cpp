// bool_parameter_hell.cpp
//
// BAD IDEA: functions with long lists of boolean (or int-as-bool) parameters
// WHY SOMEONE DOES IT: "I'll just add one more flag" — repeated 11 times
// WHY IT'S TERRIBLE: call sites are unreadable, easy to transpose flags,
//   no compiler error when you swap two bools, impossible to extend without
//   breaking all callers
// HOW TO FIX IT: named parameters via options struct, or separate well-named functions

#include "everything.hpp"

// Sends a report. You can probably figure out the parameters.
// (you cannot figure out the parameters)
int sendReport(
    string data,
    bool   compress,
    bool   encrypt,
    bool   async,
    bool   retryOnFailure,
    bool   logToFile,
    bool   logToStdout,
    bool   includeTimestamp,
    bool   includeHostname,
    bool   validateFirst,
    bool   dryRun,
    bool   urgent
) {
    // 12 bools walk into a bar.
    // Nobody knows what they ordered.

    if (compress && encrypt && !async) {
        // this combination doesn't work but there's no documentation saying so
        return -1;
    }

    if (logToFile && logToStdout && includeTimestamp && includeHostname) {
        cout << "[REPORT] [" << (urgent ? "URGENT" : "normal") << "] Sending data..." << endl;
    } else if (logToStdout) {
        cout << "Sending." << endl;
    }

    if (validateFirst && !dryRun) {
        cout << "Validated (we didn't actually validate anything)" << endl;
    }

    return dryRun ? 0 : 1;
}

// Example of calling it. Guess what each argument means.
void demonstrateCallSite() {
    sendReport("payload", true, false, true, true, false, true, true, false, true, false, false);
    //                    ^compress  ^async         ^logToFile    ^includeTimestamp  ^validateFirst  ^urgent
    //                          ^encrypt    ^retry        ^logToStdout    ^includeHostname       ^dryRun
    // Good luck reviewing this in a PR.

    // A different call three files away, six months later:
    sendReport("payload", false, true, true, false, true, false, false, true, false, true, true);
    // Are these the same as above? Different? No one knows. The original author left.
}

// Output parameters as an art form.
// Returns an int (error code), outputs via pointer, outputs more via reference,
// and also throws. Choose your adventure.
int processData(
    string  input,
    string* outputResult,       // output param 1
    int&    outputCode,         // output param 2
    bool*   outputWasEmpty,     // output param 3
    string* outputDebugInfo     // output param 4, only written in debug builds
) {
    if (input.empty()) {
        *outputWasEmpty = true;
        outputCode      = 42;
        return -1;  // also returns an error code
    }

    *outputWasEmpty  = false;
    *outputResult    = "processed: " + input;
    outputCode       = 0;
    *outputDebugInfo = "all good";

    if (input.size() > 100) {
        throw runtime_error("too long");  // also throws, because why pick one error strategy
    }

    return 0;  // and returns 0 for success
}

void runBoolParameterHell() {
    cout << "\n=== Bool Parameter Hell ===" << endl;

    demonstrateCallSite();

    string RESULT, DEBUG;
    int    CODE    = -99;
    bool   WAS_EMPTY = false;

    int RET = processData("hello", &RESULT, CODE, &WAS_EMPTY, &DEBUG);
    cout << "ret=" << RET << " code=" << CODE << " result=" << RESULT
         << " wasEmpty=" << WAS_EMPTY << " debug=" << DEBUG << endl;
}
