// macros_are_templates_now.cpp
//
// BAD IDEA: use macros to simulate generics, type checking, OOP, and error handling
// WHY SOMEONE DOES IT: legacy codebase, unfamiliarity with templates, or a dare
// WHY IT'S TERRIBLE: no type safety, no scope, debugger can't see through them,
//   error messages are incomprehensible, they compose catastrophically, they lie
// HOW TO FIX IT: templates, constexpr functions, if constexpr, concepts (C++20)

#include "everything.hpp"

// ============================================================
// MACRO-BASED TYPE SYSTEM
// (what could go wrong)
// ============================================================

// "Generic" max function via macro. No types, no safety, just vibes.
#define MAX(A, B) ((A) > (B) ? (A) : (B))
// Note: MAX(x++, y++) increments x or y TWICE. That's a feature.

// "Generic" swap. Requires that A and B are the same type.
// Enforces nothing.
#define SWAP(A, B) { auto TEMP_SWAP_VAR = (A); (A) = (B); (B) = TEMP_SWAP_VAR; }

// "Safe" division. Uses MACRO_ERROR to handle the error case.
// MACRO_ERROR is defined later. Or maybe earlier. Depends on include order.
#define SAFE_DIV(A, B) ((B) == 0 ? (MACRO_ERROR("division by zero"), 0) : (A) / (B))
#define MACRO_ERROR(MSG) (cerr << "MACRO ERROR: " << MSG << endl)

// "Object system" via macros. Java 2006 at home.
#define DECLARE_CLASS(ClassName)       struct ClassName {
#define DECLARE_FIELD(Type, Name)          Type m_##Name;
#define DECLARE_METHOD(RetType, Name)      RetType Name()
#define END_CLASS                       };

DECLARE_CLASS(Point)
    DECLARE_FIELD(int, X)
    DECLARE_FIELD(int, Y)
    DECLARE_METHOD(void, print) {
        cout << "Point(" << m_X << ", " << m_Y << ")" << endl;
    }
END_CLASS

// "Smart" pointer via macro. Not smart.
#define MAKE_PTR(Type, Name, ...) Type* Name = new Type(__VA_ARGS__)
#define FREE_PTR(Name)            delete Name; Name = nullptr
// Forgetting FREE_PTR is just as easy as forgetting delete. Nothing learned.

// Loop macros, because for loops were too readable.
#define REPEAT(N, Body)    for (int _I = 0; _I < (N); _I++) { Body }
#define FOREACH(V, C, T)   for (T V : C)
// Note: FOREACH shadows any existing variable named V. Good luck.

// Error handling "framework"
#define TRY_DO(Expr, ErrLabel)  if (!(Expr)) goto ErrLabel
// Yes, this encourages goto. It's called "structured error handling."

#define RETURN_IF_NULL(Ptr)     if ((Ptr) == nullptr) { cerr << "null: " #Ptr << endl; return; }

// Stringification magic that looks important but mostly causes confusion
#define STRINGIFY(X)         #X
#define TOSTRING(X)          STRINGIFY(X)
#define LOCATION_STR         __FILE__ ":" TOSTRING(__LINE__)
#define LOG_HERE(Msg)        cerr << "[" LOCATION_STR "] " << Msg << endl

void runMacroShowcase() {
    cout << "\n=== Macros Are Templates Now ===" << endl;

    // MAX with side effects
    int A = 3, B = 5;
    int RESULT = MAX(A++, B++);  // A incremented once, B incremented once... or twice?
    cout << "MAX result: " << RESULT << " (A=" << A << " B=" << B << ")" << endl;

    // SAFE_DIV in action
    int Q1 = SAFE_DIV(10, 2);
    int Q2 = SAFE_DIV(10, 0);  // "safe"
    cout << "10/2=" << Q1 << ", 10/0=" << Q2 << endl;

    // The "object system"
    Point P;
    P.m_X = 4; P.m_Y = 7;
    P.print();

    // MAKE_PTR "smart" pointer
    MAKE_PTR(Point, PP, );
    PP->m_X = 1; PP->m_Y = 2;
    PP->print();
    FREE_PTR(PP);

    // REPEAT loop (the I variable is hidden)
    int SUM = 0;
    REPEAT(5, SUM += _I;)
    cout << "Sum 0..4 = " << SUM << endl;

    // LOG_HERE works fine until someone moves the macro to a different file
    LOG_HERE("We are here. Wherever here is.");

    // TRY_DO with goto — classic
    int* PTR = new int(42);
    TRY_DO(PTR != nullptr, cleanup);  // always true, so goto never fires here
    cout << "PTR value: " << *PTR << endl;
    delete PTR;
    return;

cleanup:
    cout << "Cleanup (never reached in this case)" << endl;
    // PTR might not be allocated here — deleting would be UB
    // but we can't know without reading the macro expansion
}
