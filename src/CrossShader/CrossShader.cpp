#include "CrossShader.h"

#include <utility>
#include <vector>

namespace xsdr
{
std::string compile(std::string& source, ShaderFormat inputFormat,
                    ShaderFormat outputFormat, Options options)
{
    std::vector<uint32_t> spirvSource;

	if (inputFormat == outputFormat)
	{
        return source;
	}

    if (inputFormat == ShaderFormat::MSL)
    {
        // Metal Shader Language is not supported as an input format (maybe this
        // should be typesafe, but it probably will be supported in the future).
        return "";
    }

    if (inputFormat == ShaderFormat::GLSL)
    {
        bool es = false;
        const char* strs = source.c_str();

        glslang::InitializeProcess();
        // std::vector<glslang::ShaderCompUnit> compUnits;

        glslang::TShader vert(EShLangVertex);
        vert.setStrings(&strs, 1);
        vert.setSourceEntryPoint("main");

        // if the lang is hlsl
        if (inputFormat == ShaderFormat::HLSL)
        {
            vert.setEnvTargetHlslFunctionality1();
            // vert.parse()
        }

        glslang::TProgram program;
        program.addShader(&vert);
        program.link(EShMsgDefault);
        const char* infoLog = program.getInfoLog();

        glslang::SpvOptions spvOptions;
        spv::SpvBuildLogger logger;

        glslang::TIntermediate* inter = program.getIntermediate(EShLangVertex);

        glslang::GlslangToSpv(*inter, spirvSource, &spvOptions);

		// not sure of the scope of this library, but writing files is probably out of it, returning the spirSource is probably good enough...
        glslang::OutputSpvBin(spirvSource, "basename");

		// maybe we want spirv disassembly, though shader playground is probably good enough for that, we should just stick with cross compilation...
        spv::Disassemble(std::cout, spirvSource);

		// maybe we should also bundle a cli tool for this library, though that would probably not be as useful as the cli tools of regular old spirv-cross and glslang, could be easier though...

        glslang::FinalizeProcess();
    }

	// Output data

    if (outputFormat == ShaderFormat::GLSL)
    {
        spirv_cross::CompilerGLSL glsl(spirvSource);
        spirv_cross::CompilerGLSL::Options scoptions;
        scoptions.version = options.glslVersion;
        scoptions.es = options.es;
        glsl.set_options(scoptions);
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
        std::copy(spirvSource.begin(),spirvSource.end(),
                  std::ostream_iterator<uint32_t>(result, " "));
        return result.str().c_str();
    }

    return "";
}
}

int main()
{
    std::string vertSource = "";

	std::string fragSource = "";

	xsdr::Options options;

	std::string out = xsdr::compile(vertSource, xsdr::ShaderFormat::GLSL,
                  xsdr::ShaderFormat::HLSL, options);

	return 0;

}
