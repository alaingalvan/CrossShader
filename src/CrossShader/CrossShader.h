#pragma once

#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/disassemble.h"
#include "glslang/Include/ResourceLimits.h"
#include "glslang/Public/ShaderLang.h"

#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/SPVRemapper.h"
#include "SPIRV/disassemble.h"
#include "SPIRV/doc.h"
#include "StandAlone/ResourceLimits.h"
#include "glslang/Public/ShaderLang.h"

#include "spirv.hpp"
#include "spirv_glsl.hpp"
#include "spirv_hlsl.hpp"
#include "spirv_msl.hpp"

#include "dxc/dxcapi.h"

#include <string>
#include <exception>
#include <stdexcept>
#include <ostream>
#include <iterator>
#include <sstream>

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

enum ShaderStage
{
    Vertex,
    TessControl,
    TessEvaluation,
    Geometry,
    Fragment,
    Compute,
    ShaderStageMax
};

struct InputOptions
{
    // All
    ShaderFormat format;
    ShaderStage stage;
    bool ignoreErrors;

    // GLSL
    unsigned glslVersion;
    bool es;

    // HLSL
    //unsigned hlhlShaderModel;

    // MSL
    //unsigned metalPlatform;

};

struct OutputOptions
{
    // All
    ShaderFormat format;

    // GLSL
    unsigned glslVersion;
    bool es;

    // HLSL
    //unsigned hlhlShaderModel;

    // MSL
    //unsigned metalPlatform;
};

std::string compile(std::string source, InputOptions ioptions, OutputOptions ooptions);
}
