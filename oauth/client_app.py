"""
OAuth 2.0 客户端应用 - 演示授权码流程
"""

from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse, RedirectResponse
import requests
import secrets

app = FastAPI(title="OAuth 2.0 Client Application")

# 客户端配置
CLIENT_ID = "client_id_1"
CLIENT_SECRET = "client_secret_1"
REDIRECT_URI = "http://localhost:8001/callback"
AUTH_SERVER_URL = "http://localhost:8000"

# 存储令牌（生产环境应使用数据库）
token_store = {}


@app.get("/")
async def index():
    return HTMLResponse(content="""
    <!DOCTYPE html>
    <html>
    <head>
        <title>OAuth 2.0 Client</title>
        <style>
            body { font-family: Arial, sans-serif; margin: 50px; }
            .container { max-width: 800px; margin: 0 auto; }
            .btn { 
                padding: 15px 30px; 
                margin: 10px; 
                cursor: pointer;
                background-color: #4CAF50;
                color: white;
                border: none;
                border-radius: 5px;
                font-size: 16px;
            }
            .section { margin: 30px 0; padding: 20px; border: 1px solid #ddd; }
        </style>
    </head>
    <body>
        <div class="container">
            <h1>OAuth 2.0 客户端示例</h1>
            
            <div class="section">
                <h2>授权码模式 (Authorization Code Grant)</h2>
                <p>最安全的 OAuth 2.0 流程，适用于 Web 应用</p>
                <button class="btn" onclick="location.href='/login'">开始授权码流程</button>
            </div>
            
            <div class="section">
                <h2>访问受保护资源</h2>
                <button class="btn" onclick="location.href='/resource'">获取用户信息</button>
            </div>
            
            <div id="result"></div>
        </div>
    </body>
    </html>
    """)


@app.get("/login")
async def login():
    """步骤1：重定向用户到授权服务器"""
    state = secrets.token_urlsafe(16)
    auth_url = (
        f"{AUTH_SERVER_URL}/authorize?"
        f"response_type=code&"
        f"client_id={CLIENT_ID}&"
        f"redirect_uri={REDIRECT_URI}&"
        f"scope=read write&"
        f"state={state}"
    )
    return RedirectResponse(url=auth_url)


@app.get("/callback")
async def callback(request: Request):
    """步骤3：处理授权回调并交换令牌"""
    code = request.query_params.get("code")
    state = request.query_params.get("state")
    error = request.query_params.get("error")
    
    if error:
        return HTMLResponse(content=f"<h1>授权被拒绝</h1><p>Error: {error}</p>")
    
    if not code:
        return HTMLResponse(content="<h1>错误</h1><p>未收到授权码</p>", status_code=400)
    
    # 步骤4：用授权码交换访问令牌
    token_data = {
        "grant_type": "authorization_code",
        "code": code,
        "redirect_uri": REDIRECT_URI,
        "client_id": CLIENT_ID,
        "client_secret": CLIENT_SECRET
    }
    
    try:
        response = requests.post(f"{AUTH_SERVER_URL}/token", data=token_data)
        response.raise_for_status()
        token_info = response.json()
        
        # 存储令牌
        token_store["access_token"] = token_info["access_token"]
        
        return HTMLResponse(content=f"""
        <!DOCTYPE html>
        <html>
        <head>
            <title>OAuth 2.0 Client</title>
            <style>
                body {{ font-family: Arial, sans-serif; margin: 50px; }}
                .container {{ max-width: 800px; margin: 0 auto; }}
                .success {{ background-color: #d4edda; padding: 20px; border-radius: 5px; }}
                .btn {{ 
                    padding: 15px 30px; 
                    margin: 10px; 
                    cursor: pointer;
                    background-color: #4CAF50;
                    color: white;
                    border: none;
                    border-radius: 5px;
                    font-size: 16px;
                }}
            </style>
        </head>
        <body>
            <div class="container">
                <div class="success">
                    <h2>✅ 授权成功！</h2>
                    <p><strong>访问令牌：</strong></p>
                    <pre>{token_info['access_token']}</pre>
                    <p><strong>令牌类型：</strong> {token_info['token_type']}</p>
                    <p><strong>过期时间：</strong> {token_info['expires_in']} 秒</p>
                    <p><strong>权限范围：</strong> {token_info['scope']}</p>
                </div>
                <button class="btn" onclick="location.href='/'">返回首页</button>
                <button class="btn" onclick="location.href='/resource'">访问受保护资源</button>
            </div>
        </body>
        </html>
        """)
    except Exception as e:
        return HTMLResponse(content=f"<h1>错误</h1><p>获取令牌失败: {str(e)}</p>", status_code=500)


@app.get("/resource")
async def resource():
    """步骤5：使用访问令牌访问受保护资源"""
    access_token = token_store.get("access_token")
    
    if not access_token:
        return HTMLResponse(content="""
        <h1>未授权</h1>
        <p>请先进行授权流程</p>
        <button onclick="location.href='/login'">开始授权</button>
        """)
    
    try:
        # 使用令牌访问受保护资源
        headers = {"Authorization": f"Bearer {access_token}"}
        response = requests.get(f"{AUTH_SERVER_URL}/userinfo", headers=headers)
        response.raise_for_status()
        user_info = response.json()
        
        return HTMLResponse(content=f"""
        <!DOCTYPE html>
        <html>
        <head>
            <title>OAuth 2.0 Client</title>
            <style>
                body {{ font-family: Arial, sans-serif; margin: 50px; }}
                .container {{ max-width: 800px; margin: 0 auto; }}
                .success {{ background-color: #d4edda; padding: 20px; border-radius: 5px; }}
                .btn {{ 
                    padding: 15px 30px; 
                    margin: 10px; 
                    cursor: pointer;
                    background-color: #4CAF50;
                    color: white;
                    border: none;
                    border-radius: 5px;
                    font-size: 16px;
                }}
            </style>
        </head>
        <body>
            <div class="container">
                <div class="success">
                    <h2>✅ 成功获取用户信息</h2>
                    <pre>{user_info}</pre>
                </div>
                <button class="btn" onclick="location.href='/'">返回首页</button>
            </div>
        </body>
        </html>
        """)
    except Exception as e:
        return HTMLResponse(content=f"<h1>错误</h1><p>访问资源失败: {str(e)}</p>", status_code=500)


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8001)
