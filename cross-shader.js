let m = require('./bin/CrossShader.js');

module.exports = {
    default: new Promise((res, rej) => {
        m.onRuntimeInitialized = () => {
            res({
                ShaderFormat: m.ShaderFormat,
                ShaderStage: m.ShaderStage,
                compile: m.compile,
                default: m.compile
            });
        };
    })
};