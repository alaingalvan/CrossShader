const path = require('path');
const CleanWebpackPlugin = require('clean-webpack-plugin');

module.exports = {
    mode: 'production',
    context: path.join(__dirname, 'src'),
    entry: {
        app: './main.ts'
    },
    plugins: [ new CleanWebpackPlugin([ 'dist' ]) ],
    output: {
        filename: 'build.js',
        path: path.resolve(__dirname, 'dist')
    },
    resolve: {
        extensions: [ '.ts', '.tsx', '.js' ],
        modules: [ path.resolve(__dirname, 'src'), 'node_modules' ]
    },
    target: 'web',
    node: {
        __dirname: false,
        fs: 'empty',
        Buffer: false,
        process: false
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
            },
            {
                test: /\.wasm$/,
                type: 'javascript/auto',
                loader: 'file-loader',
                options: {
                    publicPath: 'dist/'
                }
            }
        ]
    },
    optimization: {
        occurrenceOrder: true
    }
};
