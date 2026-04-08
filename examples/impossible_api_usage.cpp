// impossible_api_usage.cpp
//
// BAD IDEA: APIs that are technically callable but practically unusable;
//   mixed error strategies, ownership ambiguity, overload traps
// WHY SOMEONE DOES IT: API grew organically over years by different authors
// WHY IT'S TERRIBLE: every caller must defensively handle every possible failure mode;
//   impossible to use correctly without reading the source
// HOW TO FIX IT: consistent error strategy (exceptions OR error codes, not both),
//   clear ownership semantics, simple signatures

#include "everything.hpp"

// ============================================================
// THE API
// Error handling: sometimes returns -1, sometimes returns nullptr,
// sometimes sets errno, sometimes throws, sometimes does all four.
// ============================================================

// Global error state. Also set by errno. They may disagree.
int G_LAST_ERROR_CODE = 0;
string G_LAST_ERROR_MSG = "";

// A Widget. Returned by various functions. Ownership: unclear.
struct Widget {
    int    id;
    string name;
    bool   isValid;
};

// Gets a widget by ID.
// Returns: a valid pointer (you own it), or nullptr (error set in G_LAST_ERROR_CODE),
//   or throws runtime_error (for a *different* class of errors),
//   or returns a pointer to a static Widget (do NOT free this one).
Widget* getWidget(int id) {
    if (id < 0) {
        throw runtime_error("negative ID is a logic error");
    }
    if (id == 0) {
        G_LAST_ERROR_CODE = 42;
        G_LAST_ERROR_MSG  = "ID 0 is reserved";
        return nullptr;
    }
    if (id == 999) {
        // Special sentinel: returns pointer to static — caller must NOT delete
        static Widget STATIC_WIDGET{999, "sentinel", true};
        return &STATIC_WIDGET;
    }
    // All other IDs: heap allocated, caller owns it. probably.
    return new Widget{id, "Widget_" + to_string(id), true};
}

// Updates a widget.
// Returns 0 on success, -1 on error (check G_LAST_ERROR_MSG),
// or throws if the widget is nullptr (instead of returning -1, for some reason).
int updateWidget(Widget* w, string newName) {
    if (!w) {
        throw invalid_argument("widget is null");  // inconsistent with nullptr return above
    }
    if (newName.empty()) {
        G_LAST_ERROR_CODE = 7;
        G_LAST_ERROR_MSG  = "name cannot be empty";
        return -1;
    }
    w->name = newName;
    return 0;
}

// Deletes a widget. Maybe.
// Does nothing if w is the static sentinel (id == 999).
// Crashes if called twice on the same pointer.
// Undefined behavior if w is stack-allocated.
void deleteWidget(Widget* w) {
    if (!w) return;
    if (w->id == 999) return;  // don't free the static one! (this is not documented in getWidget)
    delete w;
}

// ============================================================
// USING THE API
// ============================================================

int main() {
    cout << "=== Impossible API Usage ===" << endl;

    // To use getWidget correctly, you must know:
    // - which IDs throw vs. return nullptr vs. return static pointer
    // - whether you own the returned pointer
    // - how to check for errors (G_LAST_ERROR_CODE? errno? exception?)

    // Attempt 1: normal usage (works)
    Widget* W1 = getWidget(42);
    if (W1) {
        cout << "Got widget: " << W1->name << endl;
        int RES = updateWidget(W1, "Updated");
        if (RES != 0) cerr << "Update failed: " << G_LAST_ERROR_MSG << endl;
        deleteWidget(W1);
    }

    // Attempt 2: sentinel — do NOT delete, but how do you know?
    Widget* W2 = getWidget(999);
    cout << "Got sentinel: " << W2->name << endl;
    deleteWidget(W2);  // correctly handles this case, but only if you read the source

    // Attempt 3: zero ID — sets error code, returns nullptr
    Widget* W3 = getWidget(0);
    if (!W3) {
        cout << "Error for ID 0: " << G_LAST_ERROR_MSG << " (code=" << G_LAST_ERROR_CODE << ")" << endl;
    }

    // Attempt 4: negative ID — throws instead
    try {
        Widget* W4 = getWidget(-1);
        (void)W4;
    } catch (runtime_error& E) {
        cout << "Exception for ID -1: " << E.what() << endl;
    }

    // Attempt 5: update a nullptr — throws instead of returning -1, unlike updateWidget with empty name
    try {
        updateWidget(nullptr, "anything");
    } catch (invalid_argument& E) {
        cout << "Exception updating nullptr: " << E.what() << endl;
    }

    cout << "\nYou now understand the API. Please document it." << endl;
    return 0;
}
