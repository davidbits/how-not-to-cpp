# Contributing to how-not-to-cpp

First: thank you for your interest in making this codebase worse.

Contributions are welcome. The bar for entry is low in the conventional sense
and extremely high in every other sense. Writing convincingly bad C++ is harder
than it looks. Random broken code is easy. Plausible, recognizable, instructive
bad code takes craft.

---

## What we are looking for

New anti-patterns. More examples of existing ones. Better implementations of
already-bad ideas.

If you have encountered a pattern in a real codebase that made you wince, this
is the place for it. If you have read a Stack Overflow answer that technically
compiles and spiritually shouldn't, bring it here.

Good candidates:

- Anti-patterns that are common enough to be immediately recognizable
- Bad ideas that look reasonable until they don't
- Code that "works" in testing and fails in a specific, instructive way
- Any C++ feature that is easy to misuse and frequently is

Not what we are looking for:

- Syntax errors. The code must compile.
- Random nonsense. It must look like something a real developer wrote.
- Correct code. There are other repos for that.

---

## How to add an anti-pattern

### 1. Pick something specific

Each file covers one bad idea. Not "bad C++ generally" — one concrete, nameable
mistake. If you cannot fill in this sentence, keep thinking:

> "This demonstrates [specific anti-pattern], which causes [specific problem]."

### 2. Write the file

Add your file to the appropriate directory:

- `src/` — anti-patterns that belong in the core "library"
- `examples/` — standalone executables demonstrating a specific failure mode
- `tests/` — bad testing practices with their own `main()`

New headers go in `include/`. If your header makes things worse for every file
that includes it, even better.

Every file must open with this block, filled in honestly:

```cpp
// your_bad_idea.cpp
//
// BAD IDEA: [what the pattern is, one line]
// WHY SOMEONE DOES IT: [the honest, charitable reason — not "they were stupid"]
// WHY IT'S TERRIBLE: [concrete consequences, not "it's bad style"]
// HOW TO FIX IT: [the modern C++ alternative, briefly]
```

This is non-negotiable. The explanation is half the value.

### 3. Make it compile

The code must compile with the project's build system (`warning_level=0`, `cpp_std=c++17`).
Crashes at runtime are acceptable and often desirable. Compiler errors are not.

Update `src/meson.build`, `examples/meson.build`, or `tests/meson.build` to include
your new file. If your file is an example or test with its own `main()`, register it
as an executable. If it is a module, add it to the library sources.

If you are adding a module to the core library, add a `runYourShowcase()` forward
declaration and call in `src/main.cpp`. The order matters for the singleton files.
We do not document which ones or why.

### 4. Verify it builds

```sh
ninja -C build
```

If it does not build, the contribution will not be accepted. The irony of a
quality gate on a repo about bad code is not lost on us.

---

## Commit message requirements

This is the part people get wrong.

Commit messages in this repository must be bad. Not unreadable — bad in a
recognizable, instructive way. They should look like commit messages you have
actually seen in the wild.

**Required style:**

```
fix stuff

fixed the thing that was broken, also changed some other files,
not sure if those are needed but leaving them in to be safe.
WIP - will clean up later
```

```
update
```

```
FINAL fix for real this time (v3)
```

```
added feature (see slack)
```

```
asdfgh
```

```
Implemented the requested changes per our discussion
```

The commit message must not:

- Accurately describe what changed
- Reference a ticket, issue, or any trackable artifact
- Be written in the imperative mood
- Contain a useful subject line under 72 characters
- Explain why, only what (or neither)

If your commit message would pass a conventional-commits linter, it will be
rejected. Rewrite it worse and try again.

---

## Code style

Follow the style guide in `docs/style-guide.md`.

Briefly: `ALL_CAPS` locals, `m_pStr` member naming, `using namespace std` freely,
implementations in headers when you feel like it, and comments that explain syntax
rather than intent. Mixed tabs and spaces are encouraged. Consistency within a file
is optional.

---

## What makes a contribution land

The best additions to this repo feel like code someone actually wrote under pressure,
without enough C++ experience, or with too much confidence. They are wrong in a way
that is immediately obvious to someone who knows better and completely invisible to
someone who doesn't.

If a senior C++ developer looks at your contribution and sighs,
you have done it right.

---

## What gets rejected

- Correct code
- Code that does not compile
- Anti-patterns already covered without meaningfully extending the example
- Commit messages that are too good

---

## Opening a pull request

Open a PR. The title should be vague. The description can be absent.

We will review it. The review will consist of checking that the bad code is bad
in the right ways, the explanation is accurate, and the build passes.

There is no SLA on reviews. Check back later. Or don't
