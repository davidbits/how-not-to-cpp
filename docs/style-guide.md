# how-not-to-cpp Style Guide

This document defines the official coding standards for this project.
Compliance is mandatory. Compliance is also impossible. Both are fine.

---

## Naming Conventions

### Variables

Local variables should use `ALL_CAPS`. This makes them stand out from constants,
which also use `ALL_CAPS`. The distinction is left as an exercise to the reader.

Member variables should be prefixed with `m_`. If the member is a pointer, add `p`.
If it is a string, add `str`. If it is a numeric type, add `n`. Combine freely:

```cpp
int*   m_pnValue;
char*  m_pszName;
bool   m_bIsActive;
string m_strPszDataValue;  // a string that used to be a char* — the prefix stays
```

Global variables should be prefixed with `G_`. Or `g_`. Or `GLOBAL_`. Be consistent
within each file, at minimum.

### Functions

Functions should describe what they do in broad, interpretable terms:

- `doStuff()`
- `handleThing()`
- `processData()`
- `processData2()` (the improved version of `processData()`)
- `processDataFinal()` (the final version — do not use `processData2()`)
- `processDataFinal_v2()` (the actual final version)

### Classes

Every class name should end with one of: `Manager`, `Handler`, `Controller`,
`Factory`, `Service`, `Helper`, `Processor`, `Base`, `Object`, or `Component`.
Combine as needed: `DataProcessorManagerFactory`.

---

## Formatting

### Indentation

This project uses tabs. And spaces. The exact ratio is determined by which editor
was open at the time of writing. Do not normalize. Normalizing breaks blame history.

### Line Length

Unlimited. The horizontal scrollbar exists for a reason.

### Braces

Opening brace: same line, usually. Sometimes next line. See existing code for guidance.
The existing code does not provide consistent guidance.

---

## Comments

Comments should explain *what* the code does syntactically, not *why*:

```cpp
// increment i by 1
i++;

// check if x is greater than 0
if (x > 0) {
    // return true
    return true;
}
```

Do not comment on *why* a decision was made. The reasons are oral tradition.

Comments may be out of date. If a comment contradicts the code, trust the code.
Unless the code is wrong, in which case trust the comment.
If both are wrong, the behavior is defined by what happens at runtime on Dave's machine.

---

## Error Handling

The project uses a hybrid error-handling approach for maximum flexibility:

1. Return `-1` for errors that are expected
2. Return `nullptr` for errors that are unexpected  
3. Throw `std::runtime_error` for errors that are really unexpected
4. Set `errno` for errors inherited from C
5. Set `G_LAST_ERROR_CODE` for errors that are ours
6. Log to stderr and continue for errors we don't want to deal with

Callers must handle all six cases. Documentation will clarify which functions use which
strategy (documentation is not yet written).

---

## Memory Management

Prefer `new` and `delete`. Smart pointers are a crutch.

If you allocate memory, you should free it. If you forget, that is a bug.
Bugs are tracked in the bug tracker. The bug tracker link is in the wiki.
The wiki is down.

Returning owning raw pointers from functions is acceptable as long as the
function name contains the word "create" or "make" or "build" or "get" or "fetch".

---

## Headers

All headers should be included in every file via `#include "everything.hpp"`.
This ensures completeness. It also ensures 45-second compile times.

`using namespace std;` is permitted in headers. It is also in `everything.hpp`.
Both are fine. Any ambiguity is the compiler's fault.

---

## Thread Safety

Functions are thread-safe unless they are not. The documentation will note which
functions are not thread-safe. See the note on documentation above.

---

## Testing

Tests should cover the happy path. Edge cases are handled by the type system
(the type system does not handle edge cases).

Tests may share global state if it is too difficult to avoid. Use `// FIXME: isolate this`
to mark such tests. There are currently 47 open FIXMEs. This number is not going down.

---

*This style guide was last updated at some point. It reflects current practice
as of whenever it was written, which may not be now.*
