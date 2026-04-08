# tests/meson.build
#
# Registers the test executable with meson's test runner.
# The test depends on running in a specific order.
# Meson does not guarantee test execution order.
# This is intentional (for the repo) and terrible (in general).

test_exe = executable(
    'test_that_depends_on_execution_order',
    'test_that_depends_on_execution_order.cpp',
    include_directories : enterprise_inc,
)

# Register with meson test runner.
# Run with: meson test -C build
# Or: ninja -C build test
test(
    'order_dependent_tests',
    test_exe,
)
