# About <a href="https://cmake.org/cmake/help/v3.15"><img align="right" src="https://img.shields.io/badge/CMake-3.15-BA1F28?logo=CMake" alt="CMake 3.15" /></a><a href="https://en.wikipedia.org/wiki/C_(programming_language)"><img align="right" src="https://img.shields.io/badge/C-99-A5B4C6?logo=data:image/svg+xml;base64,PHN2ZyByb2xlPSJpbWciIHZpZXdCb3g9IjAgMCAyNCAyNCIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj48dGl0bGU+QysrIGljb248L3RpdGxlPjxwYXRoIGZpbGw9IiNmZmZmZmYiIGQ9Ik0yMi4zOTMgNmMtLjE2Ny0uMjktLjM5OC0uNTQzLS42NTItLjY5TDEyLjkyNS4yMmMtLjUwOC0uMjkzLTEuMzM5LS4yOTMtMS44NDcgMEwyLjI2IDUuMzFjLS41MDguMjkzLS45MjMgMS4wMTMtLjkyMyAxLjZ2MTAuMThjMCAuMjk0LjEwNC42Mi4yNzEuOTEuMTY3LjI5LjM5OC41NDMuNjUyLjY4OWw4LjgxNiA1LjA5MWMuNTA4LjI5MyAxLjMzOS4yOTMgMS44NDcgMGw4LjgxNi01LjA5MWMuMjU0LS4xNDYuNDg1LS4zOTkuNjUyLS42ODlzLjI3MS0uNjE2LjI3MS0uOTFWNi45MWMuMDAyLS4yOTQtLjEwMi0uNjItLjI2OS0uOTF6TTEyIDE5LjEwOWMtMy45MiAwLTcuMTA5LTMuMTg5LTcuMTA5LTcuMTA5UzguMDggNC44OTEgMTIgNC44OTFhNy4xMzMgNy4xMzMgMCAwIDEgNi4xNTYgMy41NTJsLTMuMDc2IDEuNzgxQTMuNTY3IDMuNTY3IDAgMCAwIDEyIDguNDQ1Yy0xLjk2IDAtMy41NTQgMS41OTUtMy41NTQgMy41NTVTMTAuMDQgMTUuNTU1IDEyIDE1LjU1NWEzLjU3IDMuNTcgMCAwIDAgMy4wOC0xLjc3OGwzLjA3NyAxLjc4QTcuMTM1IDcuMTM1IDAgMCAxIDEyIDE5LjEwOXoiLz48L3N2Zz4=" alt="C 99" /></a>

*Gift* is a festive esoteric programming language that takes your wishlist as input, and outputs one of two things:

- the first character of each line, one after the other
- the string `coal`, if you ask for something that is too expensive

For each line in the input, a hash is computed (32-bit FNV-1a). If the hash result is a multiple of 4, the interpreter
outputs `coal` and halts. Otherwise, it carries on. If it doesn't output `coal` by the time it reaches the end, the
interpreter outputs the first character of each line.

The hash calculation is case-insensitive and ignores whitespace.

# Usage

```shell
gift <FILE>
```

# Implementation

- `gift`: interprets a string and returns the output as a string (malloc'ed)
- `gift_result_free`: frees the memory used by a string returned by `gift` (this function is just an alias for `free`)

Note that even if the string is `coal` (i.e. compile-time constant), it is still malloc'ed in order to provide consistent behavior.

# Compiling to WASM

The interpreter can be compiled to WASM via Emscripten. See the [CMakeLists.txt](CMakeLists.txt) file for the necessary
function exports if you don't want to use CMake. After compilation, 2 files will be generated: one `.js` and one `.wasm`.
You can import the js file and use it like this:

```javascript
// Import the js file via <script src="...">, require(), or import (should also work in Node.js)
// You can then write a wrapper like this:

const gift = (src) => {
    srcPtr = Module.allocate(Module.intArrayFromString(src), Module.ALLOC_NORMAL);

    var result = Module._gift(srcPtr);

    Module._free(srcPtr);

    var str = Module.UTF8ToString(result);
    Module._gift_free_result(result);

    return str;
}

// Now you can call gift("source code here")
```

Note that the `gift` function (`Module._gift`) allocates memory via `malloc`. The `gift_free_result` function was
provided for convenience (`Module._gift_free_result`). Using `ccall` or `cwrap` isn't a good idea in this context
as they will lead to memory leaks.

# License <a href="https://github.com/UnexomWid/gift/blob/master/LICENSE"><img align="right" src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License: MIT"></a>
This project is licensed under the [MIT](https://github.com/UnexomWid/gift/blob/master/LICENSE) license.