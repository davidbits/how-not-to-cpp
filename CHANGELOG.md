# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
This file does not follow that format.

---

## [Unreleased]

- stuff in progress
- TODO: add more anti-patterns (see Slack)
- TODO: fix the thing from last week
- TODO: document which singletons depend on initialization order (low priority)

---

## [1.0.0.0.final.FINAL.release] - sometime recently

### Added
- everything

### Changed
- some things

### Fixed
- the bug
- also the other bug
- misc

### Removed
- old stuff
- files that were breaking the build on Dave's machine
- `utilities_v1.hpp`, `utilities_v2.hpp`, `utilities_v3.hpp`, `utilities_BACKUP.hpp`
  (kept `utilities_final_v2.hpp`)

---

## [0.9.0] - before that

### Added
- initial implementation
- more files
- GlobalStateManagerFactory (was just GlobalStateManager before, needed the factory
  for reasons that are in the Slack thread, Slack is no longer available)

### Fixed
- build was broken
- build was broken again
- build was broken (different issue, same error message)
- memory leak (different one, the first one is still there)

---

## [0.2.1] - earlier

### Fixed
- HOTFIX: reverted 0.2.0 changes, they broke everything on Windows
  (we do not support Windows)

---

## [0.2.0] - earlier

### Changed
- refactored everything
- renamed things for clarity
- added abstraction layer between the abstraction layer and the other abstraction layer

### Removed
- This version was reverted. See 0.2.1.

---

## [0.1.0] - the beginning

### Added
- project
- README (empty)
- `main.cpp` (also mostly empty)
- `.gitignore` (copied from a Python project)

---

*Note: versions prior to 0.1.0 were in a different repository that no longer exists.
The history is gone. The lessons it contained are also gone. This is fine.*
