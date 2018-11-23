#include "CrossShader.h"

#include <utility>
#include <vector>

int main()
{
    // Read SPIR-V from disk or similar.
    spirv_cross::CompilerGLSL glsl(nullptr, 0);

    // The SPIR-V is now parsed, and we can perform reflection on it.
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    // Get all sampled images in the shader.
    for (auto& resource : resources.sampled_images)
    {
        unsigned set =
            glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
        unsigned binding =
            glsl.get_decoration(resource.id, spv::DecorationBinding);
        printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(),
               set, binding);

        // Modify the decoration to prepare it for GLSL.
        glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

        // Some arbitrary remapping if we want.
        glsl.set_decoration(resource.id, spv::DecorationBinding,
                            set * 16 + binding);
    }

}

namespace xsdr
{
std::string Compiler::compile(std::string& source, ShaderFormat inputFormat,
                    ShaderFormat outputFormat, Options options)
{
    if (inputFormat == ShaderFormat::GLSL)
    {
        ShHandle spirvCompiler = ShConstructCompiler(FindLanguage("stdin"), Options);
        ShCompile(spirvCompiler, &shaderString, 1, nullptr, EShOptNone, &Resources, Options, (Options & EOptionDefaultDesktop) ? 110 : 100, false, messages);
    }

    if (outputFormat == ShaderFormat::GLSL)
    {
        spirv_cross::CompilerGLSL glsl(spirv);
        spirv_cross::CompilerGLSL::Options scoptions;
        scoptions.version = options.glslVersion;
        scoptions.es = options.es;
        glsl.set_options(scoptions);
        return glsl.compile();
    }
    else if (outputFormat == ShaderFormat::HLSL)
    {
        spirv_cross::CompilerHLSL hlsl(spirv);
        return hlsl.compile();
    }
    else if (outputFormat == ShaderFormat::MSL)
    {
        spirv_cross::CompilerMSL msl(spirv);
        return msl.compile();
    }
    else if (outputFormat == ShaderFormat::SPIRV)
    {
        return spirv;
    }

    return "";
}
}