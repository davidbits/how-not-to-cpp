# how-not-to-cpp

> *Doing C++ the wrong way, on purpose.*

A curated collection of C++ anti-patterns that compile, run, and disappoint.
Every file demonstrates a real mistake found in real codebases — written deliberately,
explained honestly, and organized so the wrongness is easy to find and learn from.

This is not random broken code. It is a serious engineering codebase built on
systematically awful choices. The contrast is the joke. The explanation is the point.

---

## Build

Requires a C++17 compiler, [meson](https://mesonbuild.com/), and ninja.

```sh
meson setup build
ninja -C build
```

Run the main showcase:

```sh
./build/src/how-not-to-cpp
```

Run the examples individually:

```sh
./build/examples/exception_safety_who
./build/examples/impossible_api_usage
./build/examples/thread_unsafety_demo
```

Run the tests:

```sh
meson test -C build
```

---

## Anti-pattern index

Each file has a header block explaining the bad idea, why someone reaches for it,
why it is terrible, and what the fix looks like.

### Headers

| File | Anti-pattern |
|------|-------------|
| `include/everything.hpp` | Blanket `#include` of the entire standard library + `using namespace std` in a header |
| `include/utilities_final_v2.hpp` | Function implementations in headers with no `inline` or template reason (ODR violations) |
| `include/global_state_manager_singleton_factory.hpp` | Global mutable state wrapped in a singleton, exposed through a factory |

### Core source (`src/`)

| File | Anti-pattern |
|------|-------------|
| `memory_leaks.cpp` | Manual `new`/`delete`, raw owning pointers, no RAII — leaks on every early return |
| `singleton_registry.cpp` | Singleton everything; singletons that register themselves into a singleton registry |
| `bool_parameter_hell.cpp` | 12-bool function signatures; output parameters; mixed error strategies |
| `inheritance_abuse.cpp` | `Dog`, `Cat`, `Vector3`, and `DatabaseConnection` all inherit from `EntityBaseComponentObject` |
| `dangling_reference_showcase.cpp` | References to locals, `string_view` into temporaries, iterator invalidation |
| `macros_are_templates_now.cpp` | Macros replacing templates, generics, OOP, and error handling |

### Examples (`examples/`)

| File | Anti-pattern |
|------|-------------|
| `thread_unsafety_demo.cpp` | Shared mutable state accessed from multiple threads with no synchronization |
| `exception_safety_who.cpp` | Manual resource acquisition followed by exceptions — leaks on every failure path |
| `impossible_api_usage.cpp` | Mixed error strategies, ambiguous ownership, overload traps |

### Tests (`tests/`)

| File | Anti-pattern |
|------|-------------|
| `test_that_depends_on_execution_order.cpp` | Tests that share global mutable state and must run in a specific, unenforced order |

---

## How each file is structured

```
// bad_idea.cpp
//
// BAD IDEA: what the pattern is
// WHY SOMEONE DOES IT: the honest reason a developer reaches for this
// WHY IT'S TERRIBLE: the concrete consequences
// HOW TO FIX IT: the modern C++ alternative
```

The fixes are usually simpler than the bad versions.

---

## What this is not

- A reason to avoid C++. Modern C++ has excellent idioms. This repo is a map of the potholes.
- Purely academic. Every anti-pattern here has caused production bugs somewhere.
- Exhaustive. These are representative samples, not a complete taxonomy.

---

## Further reading

- `docs/rationale.md` — why this repo exists and how to read it
- `docs/style-guide.md` — the official coding standards (read carefully)
