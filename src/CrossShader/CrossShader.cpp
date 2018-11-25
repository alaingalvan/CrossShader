#include "CrossShader.h"

#include <utility>
#include <vector>

namespace xsdr
{
std::string compile(std::string& source, ShaderFormat inputFormat,
                    ShaderFormat outputFormat, Options options)
{
    if (inputFormat == outputFormat)
    {
        return source;
    }

    if (inputFormat == ShaderFormat::MSL)
    {
        // Metal Shader Language is not supported as an input format (maybe this
        // should be typesafe, but it probably will be supported in the future).
        throw std::exception(
            "Metal is currently not supported as an input format.");
    }

    std::vector<uint32_t> spirvSource;

    if (inputFormat == ShaderFormat::GLSL || inputFormat == ShaderFormat::HLSL)
    {
        bool es = false;
        const char* strs = source.c_str();

        glslang::InitializeProcess();
        // std::vector<glslang::ShaderCompUnit> compUnits;

        glslang::TShader vert(EShLangVertex);
        vert.setStrings(&strs, 1);
        //vert.setSourceEntryPoint("main");

        // if the lang is hlsl
        if (inputFormat == ShaderFormat::HLSL)
        {
            vert.setEnvTargetHlslFunctionality1();
        }

        TBuiltInResource builtInResources = glslang::DefaultTBuiltInResource;
        EShMessages messages = EShMsgSpvRules;

        vert.parse(&builtInResources, options.glslVersion, true, messages);

        glslang::SpvOptions spvOptions;
        spv::SpvBuildLogger logger;

        glslang::TIntermediate* inter = vert.getIntermediate();

        glslang::GlslangToSpv(*inter, spirvSource, &logger, &spvOptions);

        // not sure of the scope of this library, but writing files is probably
        // out of it, returning the spirSource is probably good enough...
        // glslang::OutputSpvBin(spirvSource, "basename");

        // maybe we want spirv disassembly, though shader playground is probably
        // good enough for that, we should just stick with cross compilation...
        // spv::Disassemble(std::cout, spirvSource);

        // maybe we should also bundle a cli tool for this library, though that
        // would probably not be as useful as the cli tools of regular old
        // spirv-cross and glslang, could be easier though...

        glslang::FinalizeProcess();
    }

    // Output data

    if (outputFormat == ShaderFormat::GLSL)
    {
        spirv_cross::CompilerGLSL glsl(spirvSource);
        spirv_cross::CompilerGLSL::Options scoptions;
        scoptions.version = options.glslVersion;
        scoptions.es = options.es;
        glsl.set_common_options(scoptions);
        return glsl.compile();
    }
    else if (outputFormat == ShaderFormat::HLSL)
    {
        spirv_cross::CompilerHLSL hlsl(spirvSource);
        return hlsl.compile();
    }
    else if (outputFormat == ShaderFormat::MSL)
    {
        spirv_cross::CompilerMSL msl(spirvSource);
        return msl.compile();
    }
    else if (outputFormat == ShaderFormat::SPIRV)
    {
        std::stringstream result;
        std::copy(spirvSource.begin(), spirvSource.end(),
                  std::ostream_iterator<uint32_t>(result, " "));
        return result.str().c_str();
    }

    return "";
}
}

int main()
{
    std::string vertSource =
        "#version 450\n"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout (location = 0) in vec3 inPos;\n"
        "layout (location = 1) in vec3 inColor;\n"

        "layout (binding = 0) uniform UBO\n"
        "{\n"
        "mat4 projectionMatrix;\n"
        "mat4 modelMatrix;\n"
        "mat4 viewMatrix;\n"
        "} ubo;\n"

        "layout (location = 0) out vec3 outColor;\n"

        "out gl_PerVertex\n"
        "{\n"
        "vec4 gl_Position;\n"
        "};\n"

        "void main()\n"
        "{\n"
        "outColor = inColor;\n"
        "gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.modelMatrix "
        "* vec4(inPos.xyz, 1.0);\n"
        "}\n";

    std::string fragSource =
        "#version 450"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout(location = 0) in vec3 inColor;\n"
        "layout(location = 0) out vec4 outFragColor;\n"
        "void main() { outFragColor = vec4(inColor, 1.0); }\n";

    xsdr::Options options;
    options.es = false;
    options.glslVersion = 110;

    std::string out = xsdr::compile(vertSource, xsdr::ShaderFormat::GLSL,
                                    xsdr::ShaderFormat::MSL, options);

    return 0;
}
