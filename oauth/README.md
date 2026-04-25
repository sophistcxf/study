# OAuth 2.0 学习示例

这是一个完整的 OAuth 2.0 学习项目，使用 Python 和 FastAPI 实现，包含四种授权模式的完整演示。

## 📚 项目结构

```
oauth/
├── auth_server.py      # OAuth 2.0 授权服务器
├── client_app.py       # OAuth 2.0 客户端应用
├── demo.py             # 四种授权模式演示脚本
├── README.md           # 项目说明文档
└── pyproject.toml      # 项目依赖配置
```

## 🎯 学习目标

通过本项目，你将学习到：
- OAuth 2.0 的核心概念和工作原理
- 四种主要授权模式的流程和适用场景
- 如何构建 OAuth 2.0 授权服务器
- 如何实现 OAuth 2.0 客户端
- JWT 令牌的生成和验证
- 安全最佳实践

## 🔧 环境要求

- Python 3.10+
- uv（Python 包管理工具）

## 📦 安装依赖

```bash
uv sync
```

## 🚀 快速开始

### 1. 启动授权服务器

```bash
uv run python auth_server.py
```

授权服务器将运行在 `http://localhost:8000`

### 2. 启动客户端应用（新终端）

```bash
uv run python client_app.py
```

客户端应用将运行在 `http://localhost:8001`

### 3. 体验授权流程

访问 `http://localhost:8001`，点击"开始授权码流程"按钮，体验完整的 OAuth 2.0 授权。

### 4. 运行演示脚本

```bash
uv run python demo.py
```

演示脚本会自动展示各种授权模式的 API 调用。

## 📖 OAuth 2.0 四种授权模式详解

### 1. 授权码模式 (Authorization Code Grant)

**最安全的流程，适用于 Web 应用**

#### 流程图
```
User          Client              Auth Server           Resource Server
 |               |                     |                       |
 |---(1)-------->|                     |                       |
 |   访问应用     |                     |                       |
 |               |---(2)-------------->|                       |
 |               |   请求授权           |                       |
 |<--------------|---------------------|                       |
 |   登录/授权    |                     |                       |
 |---(3)-------->|                     |                       |
 |   确认授权     |                     |                       |
 |               |<--(4)---------------|                       |
 |               |   返回授权码         |                       |
 |               |---(5)-------------->|                       |
 |               |   用授权码换令牌     |                       |
 |               |<--(6)---------------|                       |
 |               |   返回访问令牌       |                       |
 |               |---(7)------------------------------->|      |
 |               |   使用令牌访问资源   |                       |
 |               |<--(8)-------------------------------|      |
 |               |   返回受保护资源     |                       |
```

#### 步骤说明
1. 用户访问客户端应用
2. 客户端重定向到授权服务器（带上 client_id、redirect_uri、scope）
3. 用户登录并授权
4. 授权服务器返回授权码（通过 redirect_uri）
5. 客户端用授权码向授权服务器请求访问令牌
6. 授权服务器验证授权码，返回访问令牌
7. 客户端使用访问令牌访问资源服务器
8. 资源服务器验证令牌，返回受保护资源

#### API 调用示例

**步骤 2：请求授权**
```
GET http://localhost:8000/authorize?
    response_type=code&
    client_id=client_id_1&
    redirect_uri=http://localhost:8001/callback&
    scope=read write&
    state=random_state
```

**步骤 5：交换令牌**
```bash
POST http://localhost:8000/token
Content-Type: application/x-www-form-urlencoded

grant_type=authorization_code&
code=AUTH_CODE&
redirect_uri=http://localhost:8001/callback&
client_id=client_id_1&
client_secret=client_secret_1
```

**响应：**
```json
{
  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "token_type": "bearer",
  "expires_in": 1800,
  "scope": "read write"
}
```

**步骤 7：访问资源**
```bash
GET http://localhost:8000/userinfo
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...
```

### 2. 密码模式 (Password Grant)

**适用于高度信任的应用（如第一方应用）**

⚠️ **注意**：不推荐用于第三方应用，OAuth 2.1 已将其标记为弃用

#### API 调用示例

```bash
POST http://localhost:8000/token
Content-Type: application/x-www-form-urlencoded

grant_type=password&
username=user1&
password=secret123&
client_id=client_id_1&
client_secret=client_secret_1
```

**响应：**
```json
{
  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "token_type": "bearer",
  "expires_in": 1800,
  "scope": "read write"
}
```

### 3. 客户端凭证模式 (Client Credentials Grant)

**适用于机器对机器通信，无用户上下文**

#### API 调用示例

```bash
POST http://localhost:8000/token
Content-Type: application/x-www-form-urlencoded

grant_type=client_credentials&
client_id=client_id_1&
client_secret=client_secret_1
```

**响应：**
```json
{
  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "token_type": "bearer",
  "expires_in": 1800,
  "scope": "read"
}
```

### 4. 隐式模式 (Implicit Grant)

**⚠️ 已在 OAuth 2.1 中弃用**

原因：安全性问题，访问令牌直接暴露在 URL 中

替代方案：使用授权码模式 + PKCE

## 🔑 默认账户信息

### 用户账户
- 用户名：`user1`
- 密码：`secret123`

### 客户端应用
- Client ID: `client_id_1`
- Client Secret: `client_secret_1`
- Redirect URI: `http://localhost:8001/callback`

## 🛡️ 安全最佳实践

1. **永远不要在前端暴露 Client Secret**
2. **始终使用 HTTPS**（生产环境）
3. **使用 state 参数防止 CSRF 攻击**
4. **设置合理的令牌过期时间**
5. **实现刷新令牌机制**
6. **验证所有输入参数**
7. **使用 PKCE 增强安全性**（特别是移动应用和 SPA）

## 📝 核心概念

### Access Token（访问令牌）
- 用于访问受保护资源的凭证
- 通常有过期时间
- 使用 JWT 格式

### Refresh Token（刷新令牌）
- 用于获取新的访问令牌
- 生命周期较长
- 只能在令牌端点使用

### Scope（权限范围）
- 定义客户端可以访问的资源范围
- 例如：`read`、`write`、`admin`

### State 参数
- 防止 CSRF 攻击
- 客户端生成的随机字符串
- 授权服务器会原样返回

## 🎓 学习建议

1. **先理解理论**：阅读本 README 了解 OAuth 2.0 的基本概念
2. **观察流程**：使用浏览器开发者工具查看网络请求
3. **动手实践**：修改代码，尝试不同的配置
4. **深入学习**：
   - RFC 6749: OAuth 2.0 规范
   - OAuth 2.1 的新特性
   - OpenID Connect (OIDC)
   - PKCE (Proof Key for Code Exchange)

## 🔗 相关资源

- [OAuth 2.0 官方规范 (RFC 6749)](https://tools.ietf.org/html/rfc6749)
- [OAuth 2.1 Draft](https://oauth.net/2.1/)
- [OpenID Connect](https://openid.net/connect/)
- [FastAPI 文档](https://fastapi.tiangolo.com/)

## 📄 许可证

MIT License
