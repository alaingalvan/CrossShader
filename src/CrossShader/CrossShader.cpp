#include "CrossShader.h"

#include <utility>
#include <vector>

namespace xsdr
{
std::string compile(std::string source, InputOptions ioptions,
                    OutputOptions ooptions)
{

    // ⬇️ Input Compilation
    ShaderFormat& inputFormat = ioptions.format;

    if (inputFormat == ShaderFormat::MSL)
    {
        // Metal Shader Language is not supported as an input format (maybe this
        // should be typesafe, but it probably will be supported in the future).
        return "Metal is currently not supported as an input format.";
    }

    std::vector<uint32_t> spirvSource;

    if (inputFormat == ShaderFormat::GLSL || inputFormat == ShaderFormat::HLSL)
    {
        const char* strs = source.c_str();

        glslang::InitializeProcess();

        EShLanguage stage = static_cast<EShLanguage>(ioptions.stage);
        glslang::TShader shader(stage);
        shader.setStrings(&strs, 1);

        TBuiltInResource builtInResources = glslang::DefaultTBuiltInResource;
        EShMessages messages = static_cast<EShMessages>(
            EShMsgSpvRules |
            (inputFormat == ShaderFormat::HLSL
                 ? EShMsgReadHlsl | EShMsgHlslOffsets | EShMsgHlslLegalization
                 : 0) |
            EShMsgKeepUncalled | EShMsgSuppressWarnings);

        shader.setAutoMapBindings(true);
        shader.setAutoMapLocations(true);

        shader.setEnvInput(inputFormat == ShaderFormat::HLSL
                               ? glslang::EShSourceHlsl
                               : glslang::EShSourceGlsl,
                           stage, glslang::EShClientVulkan, 100);
        shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_5);
        shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
        
        shader.parse(&builtInResources, inputFormat == ShaderFormat::HLSL ? 100 : ioptions.glslVersion, true, messages);

        glslang::SpvOptions spvOptions;
        spvOptions.validate = false;
        spvOptions.disableOptimizer = true;
        spvOptions.optimizeSize = false;

        spv::SpvBuildLogger logger;

        const char* log = shader.getInfoLog();

        if (strlen(log) > 0)
        {
            return log;
        }

        glslang::TIntermediate* inter = shader.getIntermediate();
        
        try
        {
            glslang::GlslangToSpv(*inter, spirvSource, &logger, &spvOptions);
        }
        catch (...)
        {
        }

        glslang::FinalizeProcess();
    }

    // ⬆️ Output Transpliation

    ShaderFormat& outputFormat = ooptions.format;

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
        spirv_cross::CompilerHLSL::Options hlslOptions;
        hlslOptions.shader_model = 500;
        hlsl.set_hlsl_options(hlslOptions);
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
        .field("format", &xsdr::InputOptions::format)
        .field("stage", &xsdr::InputOptions::stage)
        .field("glslVersion", &xsdr::InputOptions::glslVersion)
        .field("es", &xsdr::InputOptions::es);

    emscripten::value_object<xsdr::OutputOptions>("OutputOptions")
        .field("format", &xsdr::OutputOptions::format)
        .field("glslVersion", &xsdr::OutputOptions::glslVersion)
        .field("es", &xsdr::OutputOptions::es);

    emscripten::function("compile", &xsdr::compile);
}
#endif

int main() { return 0; }