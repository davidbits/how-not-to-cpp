// memory_leaks.cpp
//
// BAD IDEA: manual new/delete with no RAII, no smart pointers, no ownership rules
// WHY SOMEONE DOES IT: "I'll remember to free it later" / "smart pointers are slow"
// WHY IT'S TERRIBLE: leaks on every early return, exception, or forgotten branch;
//   valgrind becomes your enemy; "the OS cleans up" is not an architecture
// HOW TO FIX IT: std::unique_ptr, std::vector, std::string — RAII everywhere

#include "everything.hpp"

// A perfectly normal widget. Nothing to worry about.
struct Widget {
    int   id;
    char* name;    // raw pointer because std::string felt "heavy"
    int*  values;  // array, length stored... somewhere
    int   valueCount;
};

// Creates a widget. Caller owns it. This fact is not documented anywhere.
Widget* createWidget(int id, const char* name, int valueCount) {
    Widget* w    = new Widget();
    w->id        = id;
    w->name      = new char[strlen(name) + 1];
    strcpy(w->name, name);
    w->values    = new int[valueCount];
    w->valueCount = valueCount;

    for (int I = 0; I < valueCount; I++) {
        w->values[I] = I * 2;
    }

    return w;  // yours now. good luck.
}

// Processes a widget. Does not free it. Or mention that.
void processWidget(Widget* w) {
    cout << "Processing widget " << w->id << ": " << w->name << endl;

    // Early return on error — but we already have allocations in flight
    if (w->valueCount == 0) {
        cout << "Nothing to process." << endl;
        return;  // leaked: w, w->name, w->values
    }

    int* TEMP_BUFFER = new int[w->valueCount];  // temporary, allegedly
    for (int I = 0; I < w->valueCount; I++) {
        TEMP_BUFFER[I] = w->values[I] * 3;
        if (TEMP_BUFFER[I] > 1000) {
            cout << "Value too large, bailing out." << endl;
            return;  // leaked: TEMP_BUFFER (and w, and w->name, and w->values)
        }
    }

    cout << "First processed value: " << TEMP_BUFFER[0] << endl;
    delete[] TEMP_BUFFER;
    // w is not freed here. or anywhere. that's fine.
}

// "Frees" a widget. Partially.
void destroyWidget(Widget* w) {
    delete[] w->name;
    // forgot w->values
    delete w;
    // if you call this twice: undefined behavior.
    // if you never call it: memory leak.
    // good luck knowing which one you've done.
}

void runMemoryLeakShowcase() {
    cout << "\n=== Memory Leak Showcase ===" << endl;

    Widget* W1 = createWidget(1, "Alpha", 5);
    Widget* W2 = createWidget(2, "Beta", 0);
    Widget* W3 = createWidget(3, "Gamma", 3);

    processWidget(W1);
    processWidget(W2);  // early return — W2 leaks here, plus TEMP_BUFFER would if allocated
    processWidget(W3);

    destroyWidget(W1);  // partially freed
    // W2 and W3: not freed. we got busy.

    // store raw pointers in a vector without ownership rules
    vector<Widget*> WIDGET_POOL;
    for (int I = 0; I < 3; I++) {
        WIDGET_POOL.push_back(createWidget(I + 10, "Pooled", 2));
    }
    // vector goes out of scope. widgets do not.
    cout << "Done. " << WIDGET_POOL.size() << " widgets leaked successfully." << endl;
}
