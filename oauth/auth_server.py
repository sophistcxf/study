"""
OAuth 2.0 授权服务器 - 简化实现
演示授权码模式、密码模式、客户端凭证模式
"""

from fastapi import FastAPI, HTTPException, Depends, Request, Form
from fastapi.responses import HTMLResponse, RedirectResponse, JSONResponse
from pydantic import BaseModel
from typing import Optional
import secrets
import time
from datetime import datetime, timedelta
from jose import JWTError, jwt
from passlib.context import CryptContext

app = FastAPI(title="OAuth 2.0 Authorization Server")

# ==================== 配置 ====================
SECRET_KEY = "your-secret-key-change-in-production"
ALGORITHM = "HS256"
ACCESS_TOKEN_EXPIRE_MINUTES = 30

# 模拟数据库
USERS_DB = {
    "user1": {
        "username": "user1",
        "password_hash": "$2b$12$LQv3c1yqBWVHxkd0LHAkCOYz6TtxMQJqhN8/LewY5GyYILp92S.0i",  # password: secret123
        "email": "user1@example.com"
    }
}

CLIENTS_DB = {
    "client_id_1": {
        "client_id": "client_id_1",
        "client_secret": "client_secret_1",
        "redirect_uris": ["http://localhost:8001/callback"],
        "scopes": ["read", "write"]
    }
}

# 临时存储（生产环境应使用数据库）
authorization_codes = {}
access_tokens = {}
refresh_tokens = {}

pwd_context = CryptContext(schemes=["bcrypt"], deprecated="auto")


# ==================== 数据模型 ====================
class AuthorizationRequest(BaseModel):
    response_type: str
    client_id: str
    redirect_uri: Optional[str] = None
    scope: Optional[str] = None
    state: Optional[str] = None


class TokenRequest(BaseModel):
    grant_type: str
    code: Optional[str] = None
    redirect_uri: Optional[str] = None
    client_id: Optional[str] = None
    client_secret: Optional[str] = None
    username: Optional[str] = None
    password: Optional[str] = None
    refresh_token: Optional[str] = None


class UserLogin(BaseModel):
    username: str
    password: str


# ==================== 辅助函数 ====================
def verify_password(plain_password, hashed_password):
    return pwd_context.verify(plain_password, hashed_password)


def get_user(username: str, password: str):
    user = USERS_DB.get(username)
    if user and verify_password(password, user["password_hash"]):
        return user
    return None


def create_access_token(data: dict, expires_delta: Optional[timedelta] = None):
    to_encode = data.copy()
    expire = datetime.utcnow() + (expires_delta or timedelta(minutes=ACCESS_TOKEN_EXPIRE_MINUTES))
    to_encode.update({"exp": expire})
    encoded_jwt = jwt.encode(to_encode, SECRET_KEY, algorithm=ALGORITHM)
    return encoded_jwt


def generate_authorization_code():
    return secrets.token_urlsafe(32)


# ==================== OAuth 2.0 端点 ====================

@app.get("/")
async def index():
    return {
        "message": "OAuth 2.0 Authorization Server",
        "endpoints": {
            "authorize": "/authorize",
            "token": "/token",
            "userinfo": "/userinfo",
            "login": "/login"
        }
    }


@app.get("/authorize", response_class=HTMLResponse)
async def authorize(
    response_type: str,
    client_id: str,
    redirect_uri: Optional[str] = None,
    scope: Optional[str] = None,
    state: Optional[str] = None
):
    """授权端点 - 显示授权页面"""
    
    # 验证客户端
    client = CLIENTS_DB.get(client_id)
    if not client:
        raise HTTPException(status_code=400, detail="Invalid client_id")
    
    # 验证 redirect_uri
    if redirect_uri and redirect_uri not in client["redirect_uris"]:
        raise HTTPException(status_code=400, detail="Invalid redirect_uri")
    
    # 返回授权页面
    html = f"""
    <!DOCTYPE html>
    <html>
    <head>
        <title>OAuth 2.0 Authorization</title>
        <style>
            body {{ font-family: Arial, sans-serif; margin: 50px; }}
            .container {{ max-width: 600px; margin: 0 auto; }}
            .btn {{ padding: 10px 20px; margin: 10px; cursor: pointer; }}
            .btn-approve {{ background-color: #4CAF50; color: white; border: none; }}
            .btn-deny {{ background-color: #f44336; color: white; border: none; }}
        </style>
    </head>
    <body>
        <div class="container">
            <h1>授权请求</h1>
            <p>应用 <strong>{client_id}</strong> 请求访问您的账户</p>
            <p>请求的权限：</p>
            <ul>
                <li>{scope or 'read'}</li>
            </ul>
            <form action="/authorize/decision" method="post">
                <input type="hidden" name="response_type" value="{response_type}">
                <input type="hidden" name="client_id" value="{client_id}">
                <input type="hidden" name="redirect_uri" value="{redirect_uri or ''}">
                <input type="hidden" name="scope" value="{scope or ''}">
                <input type="hidden" name="state" value="{state or ''}">
                <button type="submit" name="decision" value="approve" class="btn btn-approve">同意授权</button>
                <button type="submit" name="decision" value="deny" class="btn btn-deny">拒绝</button>
            </form>
        </div>
    </body>
    </html>
    """
    return HTMLResponse(content=html)


@app.post("/authorize/decision")
async def authorize_decision(
    request: Request,
    response_type: str = Form(...),
    client_id: str = Form(...),
    redirect_uri: str = Form(...),
    scope: str = Form(""),
    state: str = Form(""),
    decision: str = Form(...)
):
    """处理授权决策"""
    from starlette.formparsers import FormValue
    
    if decision == "deny":
        # 用户拒绝授权
        redirect_url = redirect_uri or "http://localhost:8001/callback"
        redirect_url += "?error=access_denied"
        if state:
            redirect_url += f"&state={state}"
        return RedirectResponse(url=redirect_url)
    
    # 用户同意授权 - 生成授权码
    auth_code = generate_authorization_code()
    
    # 存储授权码（5分钟过期）
    authorization_codes[auth_code] = {
        "client_id": client_id,
        "redirect_uri": redirect_uri,
        "scope": scope or "read",
        "expires_at": time.time() + 300,  # 5分钟
        "username": "user1"  # 假设已登录用户
    }
    
    # 重定向回客户端，带上授权码
    redirect_url = redirect_uri or "http://localhost:8001/callback"
    redirect_url += f"?code={auth_code}"
    if state:
        redirect_url += f"&state={state}"
    
    return RedirectResponse(url=redirect_url)


@app.post("/token")
async def token(
    grant_type: str = Form(...),
    code: Optional[str] = Form(None),
    redirect_uri: Optional[str] = Form(None),
    client_id: Optional[str] = Form(None),
    client_secret: Optional[str] = Form(None),
    username: Optional[str] = Form(None),
    password: Optional[str] = Form(None),
    refresh_token: Optional[str] = Form(None)
):
    """令牌端点 - 交换访问令牌"""
    
    # 验证客户端
    client = CLIENTS_DB.get(client_id)
    if not client:
        raise HTTPException(status_code=400, detail="Invalid client")
    
    # 验证客户端密钥
    if client["client_secret"] != client_secret:
        raise HTTPException(status_code=400, detail="Invalid client_secret")
    
    if grant_type == "authorization_code":
        # 授权码模式
        if not code:
            raise HTTPException(status_code=400, detail="Missing authorization code")
        
        auth_code = authorization_codes.get(code)
        if not auth_code:
            raise HTTPException(status_code=400, detail="Invalid authorization code")
        
        if time.time() > auth_code["expires_at"]:
            del authorization_codes[code]
            raise HTTPException(status_code=400, detail="Authorization code expired")
        
        if auth_code["client_id"] != client_id:
            raise HTTPException(status_code=400, detail="Client mismatch")
        
        # 删除已使用的授权码
        del authorization_codes[code]
        
        # 生成访问令牌
        access_token = create_access_token(
            data={"sub": auth_code["username"], "scope": auth_code["scope"]}
        )
        
        return {
            "access_token": access_token,
            "token_type": "bearer",
            "expires_in": ACCESS_TOKEN_EXPIRE_MINUTES * 60,
            "scope": auth_code["scope"]
        }
    
    elif grant_type == "password":
        # 密码模式
        if not username or not password:
            raise HTTPException(status_code=400, detail="Missing username or password")
        
        user = get_user(username, password)
        if not user:
            raise HTTPException(status_code=400, detail="Invalid credentials")
        
        # 生成访问令牌
        access_token = create_access_token(
            data={"sub": user["username"], "scope": "read write"}
        )
        
        return {
            "access_token": access_token,
            "token_type": "bearer",
            "expires_in": ACCESS_TOKEN_EXPIRE_MINUTES * 60,
            "scope": "read write"
        }
    
    elif grant_type == "client_credentials":
        # 客户端凭证模式
        access_token = create_access_token(
            data={"sub": client_id, "scope": "read"}
        )
        
        return {
            "access_token": access_token,
            "token_type": "bearer",
            "expires_in": ACCESS_TOKEN_EXPIRE_MINUTES * 60,
            "scope": "read"
        }
    
    else:
        raise HTTPException(status_code=400, detail=f"Unsupported grant_type: {grant_type}")


@app.get("/userinfo")
async def userinfo(request: Request):
    """获取用户信息 - 需要访问令牌"""
    authorization = request.headers.get("Authorization")
    if not authorization or not authorization.startswith("Bearer "):
        raise HTTPException(status_code=401, detail="Missing or invalid authorization header")
    
    token = authorization.split(" ")[1]
    
    try:
        payload = jwt.decode(token, SECRET_KEY, algorithms=[ALGORITHM])
        username = payload.get("sub")
        if not username:
            raise HTTPException(status_code=401, detail="Invalid token")
    except JWTError:
        raise HTTPException(status_code=401, detail="Token decode error")
    
    user = USERS_DB.get(username)
    if not user:
        raise HTTPException(status_code=404, detail="User not found")
    
    return {
        "username": user["username"],
        "email": user["email"]
    }


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
