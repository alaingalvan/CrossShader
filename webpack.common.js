const path = require('path');
const CleanWebpackPlugin = require('clean-webpack-plugin');

module.exports = {
	context: path.join(__dirname, 'src'),
	entry: {
		app: './cross-shader.ts'
	},
	plugins: [ new CleanWebpackPlugin([ 'dist' ]) ],
	output: {
		filename: 'cross-shader.js',
		path: path.resolve(__dirname, 'dist'),
		library: 'CrossShader',
		libraryTarget: 'commonjs2'
	},
	resolve: {
		extensions: [ '.ts', '.tsx' ]
	},
	module: {
		rules: [
			{
				test: /\.ts/,
				exclude: /node_modules/,
				loader: 'ts-loader',
				options: {
					transpileOnly: true,
					compilerOptions: {
						isolatedModules: true
					}
				}
			}
		]
	},
	node: {
		Buffer: false
	}
};