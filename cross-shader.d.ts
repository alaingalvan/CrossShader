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
  stage: ShaderStage

  // ⚪ GLSL
  es?: boolean,
  glslVersion?: number

};

type OutputOptions = {
  
  // ⚪ GLSL
  es?: boolean,
  glslVersion?: number

};

type compile = (inputString: string, inputFormat: ShaderFormat, outputFormat: ShaderFormat, ioptions: InputOptions, ooptions: OutputOptions) => string;