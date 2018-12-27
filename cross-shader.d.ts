declare enum ShaderFormat {
	GLSL,
	HLSL,
	MSL,
	SPIRV,
	ShaderFormatMax
}

declare enum ShaderStage {
	Vertex,
	TessControl,
	TessEvaluation,
	Geometry,
	Fragment,
	Compute
}

type InputOptions = {
	// All
	format: ShaderFormat;
	stage: ShaderStage;

	// ⚪ GLSL
	es?: boolean;
	glslVersion?: number;
};

type OutputOptions = {
	// All
	format: ShaderFormat;

	// ⚪ GLSL
	es?: boolean;
	glslVersion?: number;
};

type compile = (inputString: string, ioptions: InputOptions, ooptions: OutputOptions) => string;

export default Promise<{ compile: compile; ShaderFormat: ShaderFormat; ShaderStage: ShaderStage }>();
