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

#include <string>
#include <exception>
#include <stdexcept>
#include <ostream>

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
    /*
    // Raytracing Stages
    RayGen,
    Intersect,
    AnyHit,
    ClosestHit,
    Miss,
    Callable,
    Task,
    Mesh,
    */
    ShaderStageMax
};

struct InputOptions
{
    ShaderStage stage;

    // OpenGL
    unsigned glslVersion;
    bool es;
};

struct OutputOptions
{
    // OpenGL
    unsigned glslVersion;
    bool es;
};

std::string compile(std::string source, ShaderFormat inputFormat,
                    ShaderFormat outputFormat, InputOptions ioptions = InputOptions(),
                    OutputOptions ooptions = OutputOptions());
	
std::string compileWeb(std::string source);
}
