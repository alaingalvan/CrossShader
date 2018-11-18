# CrossShader

[![Npm Package][npm-img]][npm-url]
[![cmake-img]][cmake-url]
[![License][license-img]][license-url]
[![Travis Tests][travis-img]][travis-url]
[![Coverage Tests][codecov-img]][codecov-url]

> 🚧 Currently a Work in Progress! 🚧

A cross compiler for shader languages. Convert between GLSL, HLSL, Metal Shader Language, or older versions of GLSL.

## Installation

### Node.js

```bash
npm i cross-shader
```

CrossShader will be available as as an npm module that you can include in your project as a dependency. This will require Node 8.x or above, or [a browser that supports WebAssembly](https://caniuse.com/#feat=wasm).

### C++

First add the repo as a submodule in your dependencies folder such as `external/`:

```bash
cd external
git submodule add https://github.com/alaingalvan/crossshader.git
```

Then in your `CMakeLists.txt` file, include the following:

```cmake
# ⬇ Add your dependency:
add_subdirectories(external/crossshader)

# 🔗 Link CrossShader to your project:
target_link_libraries(
    ${PROJECT_NAME}
    CrossShader
)
```

## Usage

### Node.js

```js
import compiler from 'crossshader';

compiler.run('input.glsl', '/my/output/dir');
```

### C++

```cpp
#include "CrossShader/CrossShader.h"

void main(int argc, const char** argv)
{
  xsdr::Compiler compiler;
  compiler.run('input.glsl', '/my/output/dir');
}
```

## Development

Be sure to have:

- [Git](https://git-scm.com/downloads)

- Any terminal such as [Hyper](https://hyper.is/)

And type the following in any folder:

```bash
# 🐑 Clone the repo
git clone https://github.com/alaingalvan/crossshader.git --recurse-submodules

# 💿 go inside the folder
cd crossshader

# 👯 If you forget to `recurse-submodules` you can always run:
git submodule update --init

```

From there we'll need to set up our build files. Be sure to have the following installed:

- [CMake](https://cmake.org/)

- An IDE such as [Visual Studio](https://visualstudio.microsoft.com/downloads/), [XCode](https://developer.apple.com/xcode/), or a compiler such as [GCC](https://gcc.gnu.org/).

Then type the following in your terminal from the repo folder:

```bash
# 👷 Make a build folder
mkdir build
cd build

# 🖼️ To build your Visual Studio solution on Windows x64
cmake .. -A x64 -DXSDR_TESTS=ON

# 🍎 To build your XCode project on Mac OS
cmake .. -G Xcode -DXSDR_TESTS=ON

# 🐧 To build your .make file on Linux
cmake .. -DXSDR_TESTS=ON

# 🔨 Build on any platform:
cmake --build .
```

Whenever you add new files to the project, run `cmake ..` from your solution/project folder, and if you edit the `CMakeLists.txt` file be sure to delete the generated files and run Cmake again.

### WebAssembly

```bash
emcmake cmake ..
cmake --build .
```

## License

CrossShader is licensed as either **MIT** or **Apache-2.0**, whichever you would prefer.

[cmake-img]: https://img.shields.io/badge/cmake-3.6-1f9948.svg?style=flat-square
[cmake-url]: https://cmake.org/
[license-img]: https://img.shields.io/:license-mit-blue.svg?style=flat-square
[license-url]: https://opensource.org/licenses/MIT
[travis-img]: https://img.shields.io/travis/alaingalvan/CrossShader.svg?style=flat-square&logo=travis
[travis-url]: https://travis-ci.org/alaingalvan/crossshader
[npm-img]: https://img.shields.io/npm/v/cross-shader.svg?style=flat-square
[npm-url]: http://npm.im/cross-shader
[npm-download-img]: https://img.shields.io/npm/dm/cross-shader.svg?style=flat-square
[david-url]: https://david-dm.org/alaingalvan/crossshader
[david-img]: https://david-dm.org/alaingalvan/crossshader.svg?style=flat-square
[david-dev-url]: https://david-dm.org/alaingalvan/crossshader#info=devDependencies
[david-dev-img]: https://david-dm.org/alaingalvan/crossshader/dev-status.svg?&style=flat-square
[codecov-img]: https://img.shields.io/codecov/c/github/alaingalvan/crossshader.svg?style=flat-square
[codecov-url]: https://codecov.io/gh/alaingalvan/crossshader