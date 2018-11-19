#pragma once

#include "glslang/Public/ShaderLang.h"
#include "spirv.hpp"
#include "spirv_glsl.hpp"

enum ShaderFormat
{
    GLSL,
    HLSL,
    MSL,
    SPIRV,
    ShaderFormatMax
};

typedef ShaderFormat lol[ShaderFormatMax];

struct Options
{
	// OpenGL Options
    unsigned glslVersion;
	bool es;
};

const char* compile(const char* source, ShaderFormat inputFormat,
              ShaderFormat outputFormat, Options options);