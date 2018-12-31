import 'cross-shader/bin/CrossShader.wasm';
import xsdr from 'cross-shader';

async function main() {
    let { compile, ShaderStage, ShaderFormat } = await xsdr;
    let inputString = `
#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;
} ubo;
layout (location = 0) out vec3 outColor;
out gl_PerVertex 
{
    vec4 gl_Position;   
};
void main() 
{
	outColor = inColor;
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.modelMatrix * vec4(inPos.xyz, 1.0);
}
`;

    const ioptions = {
        format: ShaderFormat.GLSL,
        stage: ShaderStage.Vertex,
        es: false,
        glslVersion: 450
    };

    const ooptions = {
        format: ShaderFormat.GLSL,
        es: true,
        glslVersion: 100
    };

    let outputString = compile(inputString, ioptions, ooptions);

    console.log(outputString);
}

main();
