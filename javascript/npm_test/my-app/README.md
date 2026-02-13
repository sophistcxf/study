此示例讲解如何安装package及引用package
- 创建package
执行
```
mkdir my-utils && cd my-utils
npm init -y
```
编写代码 index.js
```
// my-utils/index.js
function sayHello(name) {
  return `Hello, ${name}! This is from the local my-utils package.`;
}

module.exports = { sayHello };
```
- 创建 app
执行
```
cd ..
mkdir my-app && cd my-app
npm init -y
```
安装 my-utils
```
npm install ../my-utils
```
此时查看 package.json，会发现 dependencies 中多了一行 `"my-utils": "file:../my-utils"` ，同时 node_modules 目录中也会有 my-utils （副本或软链）

编写代码 app.js
```
// my-app/app.js
const utils = require('my-utils'); // 直接引用包名，不需要写路径

const message = utils.sayHello('Gemini');
console.log(message);
```
运行
```
node app.js
```
输出 `Hello, Gemini! This is from the local my-utils package.`

# require('my-utils') 的底层
当调用 require('my-utils')，node.js 首先在 node_modules 中查找。
读取该文件夹下的 package.json，寻找main字段对应的文件
加载该文件并执行
