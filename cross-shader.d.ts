enum EShaderFormat {
	GLSL,
	HLSL,
	MSL,
	SPIRV,
	ShaderFormatMax,
}

export type ShaderFormat = {
	GLSL: EShaderFormat,
	HLSL: EShaderFormat,
	MSL: EShaderFormat,
	SPIRV: EShaderFormat,
	ShaderFormatMax: EShaderFormat
}

enum EShaderStage {
	Vertex,
	TessControl,
	TessEvaluation,
	Geometry,
	Fragment,
	Compute,
}

export type ShaderStage = {
	Vertex: EShaderStage,
	TessControl: EShaderStage,
	TessEvaluation: EShaderStage,
	Geometry: EShaderStage,
	Fragment: EShaderStage,
	Compute: EShaderStage
}

export type InputOptions = {
	// All
	format: EShaderFormat;
	stage: EShaderStage;

	// ⚪ GLSL
	es?: boolean;
	glslVersion?: number;
};

export type OutputOptions = {
	// All
	format: EShaderFormat;

	// ⚪ GLSL
	es?: boolean;
	glslVersion?: number;
};

export type compile = (inputString: string, ioptions: InputOptions, ooptions: OutputOptions) => string;

declare const compile_promise: Promise<{ compile: compile; ShaderFormat: ShaderFormat; ShaderStage: ShaderStage }>;

export default compile_promise;
