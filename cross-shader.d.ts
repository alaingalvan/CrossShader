enum EShaderFormat { }
type ShaderFormat = {
	GLSL: EShaderFormat,
	HLSL: EShaderFormat,
	MSL: EShaderFormat,
	SPIRV: EShaderFormat,
	ShaderFormatMax: EShaderFormat
}

enum EShaderStage { }
type ShaderStage = {
	Vertex: EShaderStage,
	TessControl: EShaderStage,
	TessEvaluation: EShaderStage,
	Geometry: EShaderStage,
	Fragment: EShaderStage,
	Compute: EShaderStage
}

type InputOptions = {
	// All
	format: EShaderFormat;
	stage: EShaderStage;

	// ⚪ GLSL
	es?: boolean;
	glslVersion?: number;
};

type OutputOptions = {
	// All
	format: EShaderFormat;

	// ⚪ GLSL
	es?: boolean;
	glslVersion?: number;
};

type compile = (inputString: string, ioptions: InputOptions, ooptions: OutputOptions) => string;

declare const compile_promise: Promise<{ compile: compile; ShaderFormat: ShaderFormat; ShaderStage: ShaderStage }>;

export default compile_promise;
