/**
 * 前端交互逻辑（纯原生，使用浏览器内置的 fetch / EventSource，无需任何库）
 *
 * 核心概念：
 *   - 前端通过 HTTP 请求与后端通信，数据通常用 JSON 格式
 *   - fetch() 发起请求，返回 Promise，res.json() 解析响应体
 *   - 本页面与 API 同源（都由 server.js 提供），因此不涉及跨域；
 *     若前端部署在别的域名/端口，后端需配置 CORS 头
 */

// ================= 1. GET 请求：获取后端时间 =================
document.getElementById('btn-time').addEventListener('click', async () => {
  const resultEl = document.getElementById('time-result');
  resultEl.textContent = '请求中...';
  try {
    // fetch 发起 GET 请求；相对路径即可（同源）
    const res = await fetch('/api/time');
    if (!res.ok) {
      throw new Error(`HTTP ${res.status}`);
    }
    // 将响应体按 JSON 解析成 JS 对象
    const data = await res.json();
    resultEl.textContent = `${data.message}: ${data.time}`;
  } catch (err) {
    resultEl.textContent = `请求失败: ${err.message}`;
  }
});

// ================= 2. JSON CRUD：待办事项 =================
const todoList = document.getElementById('todo-list');
const todoError = document.getElementById('todo-error');

// 拉取列表并渲染
async function loadTodos() {
  const res = await fetch('/api/todos');
  const { todos } = await res.json();
  todoList.innerHTML = '';
  for (const todo of todos) {
    const li = document.createElement('li');
    li.textContent = todo.text;
    const del = document.createElement('button');
    del.textContent = '删除';
    // DELETE 请求：路径参数携带 id
    del.addEventListener('click', async () => {
      await fetch(`/api/todos/${todo.id}`, { method: 'DELETE' });
      loadTodos(); // 删除成功后重新拉取列表
    });
    li.appendChild(del);
    todoList.appendChild(li);
  }
}

// 表单提交：POST JSON 到后端
document.getElementById('todo-form').addEventListener('submit', async (e) => {
  e.preventDefault(); // 阻止表单默认刷新行为
  const input = document.getElementById('todo-input');
  const text = input.value.trim();
  if (!text) return;
  todoError.textContent = '';
  try {
    const res = await fetch('/api/todos', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' }, // 告诉后端请求体是 JSON
      body: JSON.stringify({ text }),                  // JS 对象序列化为 JSON 字符串
    });
    const data = await res.json();
    if (!res.ok) {
      // 后端返回 400 等非 2xx 状态码时，fetch 不会抛错，需要手动检查
      todoError.textContent = `后端错误: ${data.error}`;
      return;
    }
    input.value = '';
    loadTodos();
  } catch (err) {
    todoError.textContent = `请求失败: ${err.message}`;
  }
});

// ================= 3. SSE：接收服务端推送 =================
let eventSource = null;
const sseLog = document.getElementById('sse-log');
const btnStart = document.getElementById('btn-sse-start');
const btnStop = document.getElementById('btn-sse-stop');

btnStart.addEventListener('click', () => {
  // EventSource 是浏览器内置的 SSE 客户端，保持长连接并自动接收推送
  eventSource = new EventSource('/api/events');
  sseLog.textContent = '已连接，等待推送...\n';
  btnStart.disabled = true;
  btnStop.disabled = false;

  // 后端每写入一条 "data: {...}\n\n"，这里就触发一次回调
  eventSource.onmessage = (event) => {
    const data = JSON.parse(event.data);
    sseLog.textContent += `[${data.pushedAt}] 收到后端推送的消息\n`;
    sseLog.scrollTop = sseLog.scrollHeight;
  };

  eventSource.onerror = () => {
    sseLog.textContent += '(连接出现错误)\n';
  };
});

btnStop.addEventListener('click', () => {
  if (eventSource) {
    eventSource.close(); // 关闭长连接，后端会感知到并停止推送
    eventSource = null;
  }
  sseLog.textContent += '已断开连接\n';
  btnStart.disabled = false;
  btnStop.disabled = true;
});

// 页面加载时先拉一次列表
loadTodos();
