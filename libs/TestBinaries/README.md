# Test binaries

Some tests will use test binaries.
Those binaries are possibly targetting a different platform than the one the project is built on.

For this reason, they are fetched from a dedicated registry.

See https://gitlab.com/scandyna/testbinaries

The test binaries text files are managed by hand.
See the [CMakeLists.txt](CMakeLists.txt) for details.

But, while adding new binaries, or changing the registry version,
a helper can be used to download the new text files.

To download the new binaries text files,
use the `FETCH_TEST_BINARIES_TEXT_FILES` option.

See also https://gitlab.com/scandyna/testbinaries/-/issues/1
