<p align="center">
  <a href="https://alain.xyz/libraries/crossshader">
    <img alt="Logo" src="docs/cover.png"/>
  </a>
</p>
<h1 align="center">
  ⚔️ CrossShader
</h1>
<p align="center">

[![Npm Package][npm-img]][npm-url]
[![cmake-img]][cmake-url]
[![License][license-img]][license-url]
[![Travis Tests][travis-img]][travis-url]
[![Dependency Status][deps-img]][deps-url]

</p>

A cross compiler for shader languages. Convert between **SPIR-V**, **GLSL / GLSL ES**, **HLSL**, **Metal Shader Language**, or older versions of a given language. Cross Shader wraps [DirectX Shader Compiler](https://github.com/microsoft/DirectXShaderCompiler), [glslang](https://github.com/KhronosGroup/glslang), [Mozilla Naga](https://github.com/gfx-rs/naga) and [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross/), exposing a simpler interface to transpile shaders.

## Installation

### Node.js Installation

```bash
npm i cross-shader -S
```

Using this module will require Node 8.x or above, or [a browser that supports WebAssembly](https://caniuse.com/#feat=wasm).

### C++ Installation

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

This library exposes a single function `compile(...)` and its config structs/enums, and returns either the output string, or throws an exception if there's an error compiling, with the error message exposed in the exception object.

### Node.js Example

TypeScript types are included, refer to [`cross-shader.d.ts`](/cross-shader.d.ts) for more details.

Similar to other WebAssembly modules, importing the module gives you a promise to the compiled WebAssembly binary:

```js
import xsdr from "cross-shader";

xsdr.then(({ compile, ShaderFormat, ShaderStage }) => {
  const ioptions = {
    format: ShaderFormat.GLSL,
    stage: ShaderStage.Vertex,
    es: false,
    glslVersion: 450,
  };

  const ooptions = {
    format: ShaderFormat.GLSL,
    es: true,
    glslVersion: 100,
  };

  let outputString = compile(inputString, ioptions, ooptions);
});
```

### C++ Example

Refer to [`src/CrossShader/CrossShader.h`](/src/CrossShader.h) for more details.

```cpp
#include "CrossShader/CrossShader.h"

void main()
{
  xsdr::InputOptions ioptions;
  ioptions.format = xsdr::ShaderFormat::GLSL;
  ioptions.stage = xsdr::ShaderStage::Vertex;
  ioptions.es = false;
  ioptions.glslVersion = 110;

  xsdr::OutputOptions ooptions;
  ooptions.format = xsdr::ShaderFormat::GLSL;
  ooptions.es = true;
  ooptions.glslVersion = 100;

  std::string out = xsdr::compile(vertSource, ioptions, ooptions);
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
# 🖼️ To build your Visual Studio solution on Windows x64
cmake . -B build/vs -A x64

# 🍎 To build your XCode project on Mac OS
cmake .. -B build/xcode -G Xcode

# 🐧 To build your MakeFile on Linux
cmake -B build/make ..

# 🔨 Build on any platform:
cmake --build .
```

Whenever you add new files to the project, run `cmake ..` from your solution/project folder, and if you edit the `CMakeLists.txt` file be sure to delete the generated files and run Cmake again.

### WebAssembly

**Note**: if you're on Windows, I would highly recommend using the [Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10#install-the-windows-subsystem-for-linux).

First, install the latest version of Emscripten via the [Emscripten SDK](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html). Make sure to add it's Emscripten installation to your `PATH`, then:

```bash
# ⚠️ Possible dependencies you might need:
sudo apt-get update
sudo apt-get install cmake build-essential llvm

# 🏃 Then run the following:
emcmake cmake . -B build/wasm
emmake make CrossShader -j
```

## License

CrossShader is licensed as either **MIT** or **Apache-2.0**, whichever you would prefer.

[cmake-img]: https://img.shields.io/badge/cmake-3.6-1f9948.svg?style=flat-square
[cmake-url]: https://cmake.org/
[license-img]: https://img.shields.io/:license-mit-blue.svg?style=flat-square
[license-url]: https://opensource.org/licenses/MIT
[travis-img]: https://img.shields.io/travis/com/alaingalvan/crossshader.svg?style=flat-square
[travis-url]: https://www.travis-ci.com/github/alaingalvan/crossshader
[npm-img]: https://img.shields.io/npm/v/cross-shader.svg?style=flat-square
[npm-url]: http://npm.im/cross-shader
[npm-download-img]: https://img.shields.io/npm/dm/cross-shader.svg?style=flat-square
[deps-url]: https://libraries.io/npm/cross-shader
[deps-img]: https://img.shields.io/librariesio/release/npm/cross-shader?style=flat-square
[codecov-img]: https://img.shields.io/codecov/c/github/alaingalvan/crossshader.svg?style=flat-square
[codecov-url]: https://codecov.io/gh/alaingalvan/crossshader
