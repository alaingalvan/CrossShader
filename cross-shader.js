let xsdrModule = require('./bin/CrossShader.js');

const m = xsdrModule();

module.exports = new Promise((res, rej) => {
	m.onRuntimeInitialized = () => {
		res({
			ShaderFormat: m.ShaderFormat,
			ShaderStage: m.ShaderStage,
			compile: m.compile,
			default: m.compile
		});
	};
})
