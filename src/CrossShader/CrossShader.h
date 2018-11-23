#pragma once

#include "glslang/Public/ShaderLang.h"
#include "spirv.hpp"
#include "spirv_glsl.hpp"

#include <string>

namespace xsdr
{
enum ShaderFormat
{
    GLSL,
    HLSL,
    MSL,
    SPIRV,
    ShaderFormatMax
};

struct Options
{
    // OpenGL Options
    unsigned glslVersion;
    bool es;
};

class Compiler
{
public:

    std::string compile(std::string& source, ShaderFormat inputFormat,
                        ShaderFormat outputFormat, Options options);

protected:
    spirv_cross::CompilerGLSL glsl;
    spirv_cross::CompilerHLSL hlsl;
    spirv_cross::CompilerMSL msl;
    ShHandle spirv;
}

}