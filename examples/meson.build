# examples/meson.build
#
# Each example is a standalone executable demonstrating a specific anti-pattern.
# They do not share a library because they already share enough global state.

examples = [
    'thread_unsafety_demo',
    'exception_safety_who',
    'impossible_api_usage',
]

foreach ex : examples
    executable(
        ex,
        ex + '.cpp',
        include_directories : enterprise_inc,
    )
endforeach
