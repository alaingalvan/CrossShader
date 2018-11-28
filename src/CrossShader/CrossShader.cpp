#include "CrossShader.h"

#include <utility>
#include <vector>

namespace xsdr
{
std::string compile(std::string source, ShaderFormat inputFormat,
                    ShaderFormat outputFormat, InputOptions ioptions, OutputOptions ooptions)
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
	
	std::string compileWeb(std::string source)
	{
		xsdr::InputOptions ioptions;
		ioptions.stage = xsdr::ShaderStage::Vertex;
		ioptions.es = false;
		ioptions.glslVersion = 110;
		
		xsdr::OutputOptions ooptions;
		ooptions.es = true;
		ooptions.glslVersion = 100;
		
		std::string out = xsdr::compile(source, xsdr::ShaderFormat::GLSL,
										xsdr::ShaderFormat::GLSL, ioptions, ooptions);
		
		return out;
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
	std::string test = xsdr::compileWeb("#version 450\n"
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
								"}\n");

    return 0;
}
