#pragma once

#include "glslang/Public/ShaderLang.h"
#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/disassemble.h"
#include "glslang/Include/ResourceLimits.h"

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

std::string compile(std::string& source, ShaderFormat inputFormat,
                    ShaderFormat outputFormat, Options options);
}