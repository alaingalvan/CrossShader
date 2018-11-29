#include "CrossShader.h"

#include <utility>
#include <vector>

namespace xsdr
{
std::string compile(std::string source, ShaderFormat inputFormat,
                    ShaderFormat outputFormat, InputOptions ioptions,
                    OutputOptions ooptions)
{

    // ⬇️ Input Compilation

    if (inputFormat == ShaderFormat::MSL)
    {
        // Metal Shader Language is not supported as an input format (maybe this
        // should be typesafe, but it probably will be supported in the future).
        throw std::runtime_error(
            "Metal is currently not supported as an input format.");
    }

    std::vector<uint32_t> spirvSource;

    if (inputFormat == ShaderFormat::GLSL || inputFormat == ShaderFormat::HLSL)
    {
        const char* strs = source.c_str();

        glslang::InitializeProcess();

        glslang::TShader shader(static_cast<EShLanguage>(ioptions.stage));
        shader.setStrings(&strs, 1);

        if (inputFormat == ShaderFormat::HLSL)
        {
            shader.setEnvTargetHlslFunctionality1();
        }

        TBuiltInResource builtInResources = glslang::DefaultTBuiltInResource;
        EShMessages messages = EShMsgSpvRules;

        shader.parse(&builtInResources, ioptions.glslVersion, true, messages);

        glslang::SpvOptions spvOptions;
        spv::SpvBuildLogger logger;

        glslang::TIntermediate* inter = shader.getIntermediate();

        glslang::GlslangToSpv(*inter, spirvSource, &logger, &spvOptions);

        glslang::FinalizeProcess();
    }

    // ⬆️ Output Transpliation

    if (outputFormat == ShaderFormat::GLSL)
    {
        spirv_cross::CompilerGLSL glsl(spirvSource);
        spirv_cross::CompilerGLSL::Options scoptions;
        scoptions.version = ooptions.glslVersion;
        scoptions.es = ooptions.es;
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

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(cross_shader)
{
    emscripten::enum_<xsdr::ShaderFormat>("ShaderFormat")
        .value("GLSL", xsdr::ShaderFormat::GLSL)
        .value("HLSL", xsdr::ShaderFormat::HLSL)
        .value("MSL", xsdr::ShaderFormat::MSL)
        .value("SPIRV", xsdr::ShaderFormat::SPIRV)
        .value("ShaderFormatMax", xsdr::ShaderFormat::ShaderFormatMax);

    emscripten::enum_<xsdr::ShaderStage>("ShaderStage")
        .value("Vertex", xsdr::ShaderStage::Vertex)
        .value("TessControl", xsdr::ShaderStage::TessControl)
        .value("TessEvaluation", xsdr::ShaderStage::TessEvaluation)
        .value("Geometry", xsdr::ShaderStage::Geometry)
        .value("Fragment", xsdr::ShaderStage::Fragment)
        .value("Compute", xsdr::ShaderStage::Compute)
        .value("ShaderStageMax", xsdr::ShaderStage::ShaderStageMax);

    emscripten::value_object<xsdr::InputOptions>("InputOptions")
        .field("stage", &xsdr::InputOptions::stage)
        .field("glslVersion", &xsdr::InputOptions::glslVersion)
        .field("es", &xsdr::InputOptions::es);

    emscripten::value_object<xsdr::OutputOptions>("OutputOptions")
        .field("glslVersion", &xsdr::OutputOptions::glslVersion)
        .field("es", &xsdr::OutputOptions::es);

    emscripten::function("compile", &xsdr::compile);

    emscripten::function("compileTest", &xsdr::compileWeb);
}
#endif

int main()
{
    return 0;
}
