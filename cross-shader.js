let m = require('./bin/CrossShader.js');

module.exports = {
    ShaderFormat: m.shaderFormat,
    ShaderStage: m.ShaderStage,
    compile: m.compile,
    default: m.compile
};