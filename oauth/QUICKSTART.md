# OAuth 2.0 快速入门指南

## 🚀 5分钟快速体验

### 1️⃣ 启动服务

```bash
# 终端1 - 启动授权服务器
uv run python auth_server.py

# 终端2 - 启动客户端应用
uv run python client_app.py
```

### 2️⃣ 体验授权流程

打开浏览器访问：`http://localhost:8001`

点击"开始授权码流程"按钮，按照提示完成 OAuth 2.0 授权。

### 3️⃣ 运行演示脚本

```bash
# 查看所有授权模式的演示
echo "" | uv run python demo.py
```

## 📖 核心概念速览

### OAuth 2.0 四种授权模式

| 模式 | 适用场景 | 安全性 |
|------|---------|--------|
| **授权码模式** | Web 应用（最安全） | ⭐⭐⭐⭐⭐ |
| **密码模式** | 高度信任的第一方应用 | ⭐⭐⭐ |
| **客户端凭证** | 机器对机器通信 | ⭐⭐⭐⭐ |
| **隐式模式** | ❌ 已弃用 | ⭐⭐ |

### 主要角色

- **Resource Owner（资源所有者）**: 用户
- **Client（客户端）**: 请求授权的应用
- **Authorization Server（授权服务器）**: 颁发令牌
- **Resource Server（资源服务器）**: 托管受保护资源

## 🔑 默认配置

```
用户账户:
  用户名: user1
  密码: secret123

客户端:
  Client ID: client_id_1
  Client Secret: client_secret_1
  Redirect URI: http://localhost:8001/callback
```

## 🛠️ API 端点

### 授权服务器 (http://localhost:8000)

- `GET /authorize` - 授权端点
- `POST /token` - 令牌端点
- `GET /userinfo` - 用户信息端点（需要令牌）

### 客户端应用 (http://localhost:8001)

- `GET /` - 首页
- `GET /login` - 开始授权流程
- `GET /callback` - 授权回调
- `GET /resource` - 访问受保护资源

## 📝 示例代码

### 获取访问令牌（密码模式）

```python
import requests

token_data = {
    "grant_type": "password",
    "username": "user1",
    "password": "secret123",
    "client_id": "client_id_1",
    "client_secret": "client_secret_1"
}

response = requests.post("http://localhost:8000/token", data=token_data)
token = response.json()["access_token"]
print(f"Access Token: {token}")
```

### 使用访问令牌访问资源

```python
headers = {"Authorization": f"Bearer {token}"}
response = requests.get("http://localhost:8000/userinfo", headers=headers)
print(f"User Info: {response.json()}")
```

## 🎯 下一步

1. ✅ 完成快速体验
2. 📚 阅读完整的 [README.md](README.md)
3. 🔍 查看源代码了解实现细节
4. 🧪 修改配置，尝试不同的场景
5. 📖 学习 OAuth 2.0 规范和安全最佳实践

## ⚠️ 注意事项

- 🚫 本项目仅用于学习目的
- 🔒 生产环境需要使用正确的 HTTPS、数据库和安全的密钥管理
- 📦 建议使用成熟的 OAuth 2.0 库（如 Authlib、OAuthLib）
- 📖 参考 [RFC 6749](https://tools.ietf.org/html/rfc6749) 了解完整规范

## 🆘 常见问题

**Q: 为什么密码模式报错？**
A: 确保授权服务器正在运行，并且用户名密码正确（user1/secret123）

**Q: 如何重置令牌？**
A: 重新启动授权服务器会清除所有令牌

**Q: 可以在生产环境使用吗？**
A: 不可以！这只是一个学习示例，生产环境需要使用专业的 OAuth 2.0 解决方案

## 📚 更多资源

- [OAuth 2.0 官方规范](https://tools.ietf.org/html/rfc6749)
- [OAuth 2.1 Draft](https://oauth.net/2.1/)
- [OpenID Connect](https://openid.net/connect/)
- [FastAPI OAuth2](https://fastapi.tiangolo.com/tutorial/security/oauth2-jwt/)
