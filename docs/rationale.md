# Rationale

## Why does this code look like this?

Every file in this repository demonstrates a real anti-pattern that appears in
real production C++ codebases. The code is wrong on purpose.

The goal is to make the wrongness *recognizable* — not random chaos, but the kind
of systematic, plausible badness that grows in codebases over years. Code that
compiles, ships, and disappoints.

---

## How to read each file

Each source file has a header block with four fields:

**BAD IDEA** — the anti-pattern, stated plainly.

**WHY SOMEONE DOES IT** — the honest reason a developer reaches for this pattern.
Often laziness, deadline pressure, cargo-culting, or lack of exposure to better tools.

**WHY IT'S TERRIBLE** — the concrete consequences. Not "it's bad style" but
"here is how this breaks, when, and why the bug is hard to find."

**HOW TO FIX IT** — the modern C++ alternative. Usually simpler than the bad version.

---

## Anti-pattern index

| File | Anti-pattern |
|------|-------------|
| `include/everything.hpp` | Blanket includes + `using namespace` in headers |
| `include/utilities_final_v2.hpp` | Function implementations in headers (ODR violations) |
| `include/global_state_manager_singleton_factory.hpp` | Global mutable state via singleton |
| `src/memory_leaks.cpp` | Manual `new`/`delete`, raw owning pointers, no RAII |
| `src/singleton_registry.cpp` | Singleton everything, singletons registering singletons |
| `src/bool_parameter_hell.cpp` | 12-bool function signatures, output parameters |
| `src/inheritance_abuse.cpp` | Inheritance where composition (or nothing) fits better |
| `src/dangling_reference_showcase.cpp` | References to locals, `string_view` to temporaries |
| `src/macros_are_templates_now.cpp` | Macros replacing templates, functions, and OOP |
| `examples/thread_unsafety_demo.cpp` | Shared mutable state, no synchronization |
| `examples/exception_safety_who.cpp` | Manual resource management + exceptions = leaks |
| `examples/impossible_api_usage.cpp` | Mixed error strategies, ambiguous ownership |
| `tests/test_that_depends_on_execution_order.cpp` | Non-isolated tests, shared global test state |

---

## What this repository is not

- A collection of broken code for its own sake. Every example should teach something.
- A complete reference. These are representative samples, not an exhaustive taxonomy.
- A reason to avoid C++. C++ has excellent modern idioms. This repo is a map of the potholes.

---

## The right version

For each anti-pattern shown, the fix is usually one of:

- **RAII**: let constructors acquire, destructors release — `unique_ptr`, `vector`, `string`
- **Value semantics**: return by value, let the compiler optimize
- **Named types**: options structs, enums, strong typedefs — instead of `bool, bool, bool`
- **Composition**: hold members, don't inherit from them
- **Single error strategy**: pick exceptions or error codes, not both
- **Explicit ownership**: `unique_ptr` or `shared_ptr` make ownership part of the type
- **Isolated tests**: each test is a function; each function sets up its own state

Modern C++ (C++11 and later) makes almost all of these the path of least resistance.
The code in this repository goes out of its way to take the hard path.
