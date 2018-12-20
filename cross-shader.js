const Module = require(bin/CrossShader.js);

module.exports = {
    ShaderFormat: Module.shaderFormat,
    ShaderStage: Module.ShaderStage,
    compile: Module.compile
};