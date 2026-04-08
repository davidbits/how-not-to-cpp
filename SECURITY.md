# Security Policy

## Supported Versions

| Version | Supported |
|---------|-----------|
| 1.0.0.0.final.FINAL.release | :white_check_mark: |
| All others | We don't know what versions exist |

## Reporting a Vulnerability

If you discover a security vulnerability in this repository, please open a
GitHub issue with the title "Security thing" and a description of what you found.

Do not worry about disclosure timelines. We operate on a "whenever" cadence.

## Known Issues

The following security concerns are known and will be addressed at some point:

- `include/global_state_manager_singleton_factory.hpp` — the singleton is initialized
  with `new` and never freed. The destructor is never called. Secrets stored in
  `GlobalApplicationState` persist in memory for the lifetime of the process. This
  is noted as a TODO. The TODO is from 2019.

- `include/utilities_final_v2.hpp` — `intToString()` allocates a 64-byte buffer with
  `new char[64]` and returns it without freeing it. This is technically a memory leak.
  It is also the least of our concerns.

- `src/singleton_registry.cpp` — the database connection string is hardcoded to
  `192.168.1.1`. This is not a production IP. We think.

- `src/singleton_registry.cpp` — there is no authentication on the database singleton.
  The `query()` method accepts arbitrary strings. The inputs are logged but not
  validated. SQL injection is demonstrable. This is intentional in the sense that
  we did not think about it.

- `include/utilities_final_v2.hpp` — `getCurrentTime()` calls `ctime()`, which
  returns a pointer to a static buffer. This is not thread-safe. See also:
  `examples/thread_unsafety_demo.cpp`.

- The password `password123` appears in `examples/impossible_api_usage.cpp` inside a
  struct definition. It is a demo. Probably. The comment saying "also a security
  showcase" was added after the fact.

- `ctime()` is used in headers. `sprintf()` is used in headers. These are banned by
  several security standards we are not complying with.

## Threat Model

This software is a demonstration of bad C++ practices. The threat model is:
educational use, local execution, no network exposure, no user data.

If you are running this in production, the vulnerability is the decision to run
this in production. Please open an issue.

## Scope

Security reports about the intentionally bad code are appreciated but not
actionable. The bad code is load-bearing.

Security reports about the build system, CI configuration, or repository
settings are welcome and will be taken seriously.
