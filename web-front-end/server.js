/**
 * 极简 Node.js 后端示例（零依赖，直接运行: node server.js）
 *
 * 演示前端与后端交互的三种常见方式：
 *   1. GET  请求/响应          -> GET  /api/time
 *   2. JSON 增删改查 (CRUD)     -> GET/POST /api/todos, DELETE /api/todos/:id
 *   3. SSE 服务端主动推送       -> GET  /api/events
 */
const http = require('http');
const fs = require('fs');
const path = require('path');

const PORT = 3000;

// ---------------- 内存数据（模拟数据库） ----------------
let nextId = 1;
const todos = [
  { id: nextId++, text: '学习 HTTP 请求/响应' },
  { id: nextId++, text: '理解 JSON 数据格式' },
];

// ---------------- 工具函数 ----------------

// 发送 JSON 响应
function sendJson(res, statusCode, data) {
  const body = JSON.stringify(data);
  res.writeHead(statusCode, {
    'Content-Type': 'application/json; charset=utf-8',
    // 前后端分离（不同端口/域名）时需要 CORS 头，浏览器才允许跨域读取
    'Access-Control-Allow-Origin': '*',
    'Access-Control-Allow-Methods': 'GET, POST, DELETE, OPTIONS',
    'Access-Control-Allow-Headers': 'Content-Type',
  });
  res.end(body);
}

// 读取请求体（POST 数据是流式的，需要拼接）
function readBody(req) {
  return new Promise((resolve, reject) => {
    let data = '';
    req.on('data', (chunk) => (data += chunk));
    req.on('end', () => resolve(data));
    req.on('error', reject);
  });
}

// 静态文件 MIME 类型
const MIME = {
  '.html': 'text/html; charset=utf-8',
  '.js': 'text/javascript; charset=utf-8',
  '.css': 'text/css; charset=utf-8',
};

// 提供静态文件（前端页面）
function serveStatic(req, res) {
  const urlPath = req.url === '/' ? '/index.html' : req.url;
  // 防止路径穿越攻击
  const filePath = path.normalize(path.join(__dirname, urlPath));
  if (!filePath.startsWith(__dirname)) {
    res.writeHead(403);
    return res.end('Forbidden');
  }
  fs.readFile(filePath, (err, content) => {
    if (err) {
      res.writeHead(404, { 'Content-Type': 'text/plain' });
      return res.end('404 Not Found');
    }
    const ext = path.extname(filePath);
    res.writeHead(200, { 'Content-Type': MIME[ext] || 'application/octet-stream' });
    res.end(content);
  });
}

// ---------------- API 路由 ----------------
const server = http.createServer(async (req, res) => {
  const url = new URL(req.url, `http://localhost:${PORT}`);
  const pathname = url.pathname;

  // CORS 预检请求（浏览器在跨域发送非简单请求前会先发 OPTIONS）
  if (req.method === 'OPTIONS') {
    res.writeHead(204, {
      'Access-Control-Allow-Origin': '*',
      'Access-Control-Allow-Methods': 'GET, POST, DELETE, OPTIONS',
      'Access-Control-Allow-Headers': 'Content-Type',
    });
    return res.end();
  }

  // 1) 最简单的请求/响应：前端问，后端答
  if (req.method === 'GET' && pathname === '/api/time') {
    return sendJson(res, 200, {
      message: '后端当前时间',
      time: new Date().toLocaleString('zh-CN', { hour12: false }),
    });
  }

  // 2) CRUD：获取列表
  if (req.method === 'GET' && pathname === '/api/todos') {
    return sendJson(res, 200, { todos });
  }

  // 2) CRUD：新增（前端发送 JSON 请求体）
  if (req.method === 'POST' && pathname === '/api/todos') {
    const body = await readBody(req);
    let parsed;
    try {
      parsed = JSON.parse(body); // 后端解析并校验前端传来的 JSON
    } catch {
      return sendJson(res, 400, { error: '请求体不是合法的 JSON' });
    }
    if (!parsed.text || typeof parsed.text !== 'string') {
      return sendJson(res, 400, { error: '字段 text 不能为空' });
    }
    const todo = { id: nextId++, text: parsed.text.trim() };
    todos.push(todo);
    return sendJson(res, 201, { todo }); // 201 Created
  }

  // 2) CRUD：删除（路径参数 /api/todos/:id）
  const deleteMatch = pathname.match(/^\/api\/todos\/(\d+)$/);
  if (req.method === 'DELETE' && deleteMatch) {
    const id = Number(deleteMatch[1]);
    const index = todos.findIndex((t) => t.id === id);
    if (index === -1) {
      return sendJson(res, 404, { error: `id=${id} 不存在` });
    }
    todos.splice(index, 1);
    return sendJson(res, 200, { ok: true });
  }

  // 3) SSE（Server-Sent Events）：服务端向浏览器持续推送数据
  if (req.method === 'GET' && pathname === '/api/events') {
    res.writeHead(200, {
      'Content-Type': 'text/event-stream; charset=utf-8',
      'Cache-Control': 'no-cache',
      Connection: 'keep-alive',
    });
    // 每 2 秒推送一条消息，格式遵循 SSE 协议: "data: ...\n\n"
    const timer = setInterval(() => {
      res.write(`data: ${JSON.stringify({ pushedAt: new Date().toLocaleTimeString('zh-CN', { hour12: false }) })}\n\n`);
    }, 2000);
    req.on('close', () => clearInterval(timer)); // 前端断开时停止推送
    return;
  }

  // 其余请求当作静态文件处理
  serveStatic(req, res);
});

server.listen(PORT, () => {
  console.log(`后端已启动: http://localhost:${PORT}`);
});
