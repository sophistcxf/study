const addon = require('./build/Release/addon');

// 同步调用
console.log('同步调用:');
console.log(addon.greetSync('Alice'));

// 异步调用
console.log('\n异步调用:');
addon.greetAsync('Bob')
  .then(result => console.log(result))
  .catch(err => console.error(err));

// 错误处理
try {
  console.log('\n错误处理:');
  console.log(addon.greetSync(123)); // 故意传入错误类型
} catch (err) {
  console.error('捕获错误:', err.message);
}
