"""
OAuth 2.0 四种授权模式完整演示脚本
包含：授权码模式、密码模式、客户端凭证模式、隐式模式
"""

import requests
import time

# 配置
AUTH_SERVER_URL = "http://localhost:8000"
CLIENT_ID = "client_id_1"
CLIENT_SECRET = "client_secret_1"
REDIRECT_URI = "http://localhost:8001/callback"


def print_section(title):
    """打印分节标题"""
    print("\n" + "=" * 60)
    print(f"  {title}")
    print("=" * 60 + "\n")


def demo_authorization_code_grant():
    """
    演示授权码模式 (Authorization Code Grant)
    
    流程：
    1. 用户访问客户端
    2. 客户端重定向到授权服务器
    3. 用户登录并授权
    4. 授权服务器返回授权码
    5. 客户端用授权码交换访问令牌
    6. 使用访问令牌访问资源
    """
    print_section("1. 授权码模式 (Authorization Code Grant)")
    
    # 步骤1：模拟获取授权码（实际中需要用户浏览器交互）
    print("步骤1: 用户被重定向到授权服务器进行授权...")
    auth_params = {
        "response_type": "code",
        "client_id": CLIENT_ID,
        "redirect_uri": REDIRECT_URI,
        "scope": "read write",
        "state": "random_state_string"
    }
    print(f"  授权URL: {AUTH_SERVER_URL}/authorize?{ '&'.join([f'{k}={v}' for k, v in auth_params.items()]) }")
    
    # 在实际场景中，这里需要用户在浏览器中完成授权
    # 为了演示，我们直接调用授权服务器的密码模式来获取授权码
    print("\n步骤2: 模拟用户授权后获得授权码...")
    
    # 通过内部API获取授权码（仅用于演示）
    token_data = {
        "grant_type": "password",
        "username": "user1",
        "password": "secret123",
        "client_id": CLIENT_ID,
        "client_secret": CLIENT_SECRET
    }
    
    response = requests.post(f"{AUTH_SERVER_URL}/token", data=token_data)
    if response.status_code == 200:
        token_info = response.json()
        print(f"✅ 成功获取访问令牌（通过密码模式模拟）:")
        print(f"  Token: {token_info['access_token'][:50]}...")
        print(f"  Type: {token_info['token_type']}")
        print(f"  Expires In: {token_info['expires_in']}s")
        print(f"  Scope: {token_info['scope']}")
        
        # 使用令牌访问资源
        print("\n步骤3: 使用访问令牌访问受保护资源...")
        headers = {"Authorization": f"Bearer {token_info['access_token']}"}
        user_response = requests.get(f"{AUTH_SERVER_URL}/userinfo", headers=headers)
        
        if user_response.status_code == 200:
            print(f"✅ 成功获取用户信息: {user_response.json()}")
        else:
            print(f"❌ 获取用户信息失败: {user_response.text}")
    else:
        print(f"❌ 获取令牌失败: {response.text}")


def demo_password_grant():
    """
    演示密码模式 (Resource Owner Password Credentials Grant)
    
    适用场景：高度信任的应用
    注意：不推荐在第三方应用中使用
    """
    print_section("2. 密码模式 (Password Grant)")
    
    print("步骤1: 直接使用用户名和密码请求访问令牌...")
    
    token_data = {
        "grant_type": "password",
        "username": "user1",
        "password": "secret123",
        "client_id": CLIENT_ID,
        "client_secret": CLIENT_SECRET
    }
    
    response = requests.post(f"{AUTH_SERVER_URL}/token", data=token_data)
    
    if response.status_code == 200:
        token_info = response.json()
        print(f"✅ 成功获取访问令牌:")
        print(f"  Token: {token_info['access_token'][:50]}...")
        print(f"  Type: {token_info['token_type']}")
        print(f"  Scope: {token_info['scope']}")
        
        print("\n步骤2: 使用访问令牌访问受保护资源...")
        headers = {"Authorization": f"Bearer {token_info['access_token']}"}
        user_response = requests.get(f"{AUTH_SERVER_URL}/userinfo", headers=headers)
        
        if user_response.status_code == 200:
            print(f"✅ 成功获取用户信息: {user_response.json()}")
        else:
            print(f"❌ 获取用户信息失败: {user_response.text}")
    else:
        print(f"❌ 获取令牌失败: {response.text}")


def demo_client_credentials_grant():
    """
    演示客户端凭证模式 (Client Credentials Grant)
    
    适用场景：机器对机器通信，无用户上下文
    """
    print_section("3. 客户端凭证模式 (Client Credentials Grant)")
    
    print("步骤1: 使用客户端凭证请求访问令牌...")
    
    token_data = {
        "grant_type": "client_credentials",
        "client_id": CLIENT_ID,
        "client_secret": CLIENT_SECRET
    }
    
    response = requests.post(f"{AUTH_SERVER_URL}/token", data=token_data)
    
    if response.status_code == 200:
        token_info = response.json()
        print(f"✅ 成功获取访问令牌:")
        print(f"  Token: {token_info['access_token'][:50]}...")
        print(f"  Type: {token_info['token_type']}")
        print(f"  Scope: {token_info['scope']}")
        print("\n注意: 此模式下没有用户上下文，只能访问客户端自己的资源")
    else:
        print(f"❌ 获取令牌失败: {response.text}")


def demo_implicit_grant():
    """
    演示隐式模式 (Implicit Grant)
    
    注意：OAuth 2.1 已不再推荐使用此模式
    """
    print_section("4. 隐式模式 (Implicit Grant)")
    
    print("说明: 隐式模式已在 OAuth 2.1 中被弃用")
    print("原因: 安全性问题，令牌直接暴露在 URL 中")
    print("\n替代方案:")
    print("  - 单页应用 (SPA): 使用授权码模式 + PKCE")
    print("  - 移动应用: 使用授权码模式 + PKCE")
    print("\n授权URL示例（旧版）:")
    implicit_url = (
        f"{AUTH_SERVER_URL}/authorize?"
        f"response_type=token&"
        f"client_id={CLIENT_ID}&"
        f"redirect_uri={REDIRECT_URI}&"
        f"scope=read"
    )
    print(f"  {implicit_url}")
    print("\n⚠️  注意: 访问令牌会直接在重定向URL中返回，存在安全风险")


def main():
    """运行所有演示"""
    print("\n" + "🎓" * 30)
    print("  OAuth 2.0 授权模式学习演示")
    print("🎓" * 30)
    
    # 检查授权服务器是否运行
    try:
        response = requests.get(f"{AUTH_SERVER_URL}/")
        print(f"\n✅ 授权服务器运行正常: {AUTH_SERVER_URL}")
    except Exception as e:
        print(f"\n❌ 无法连接到授权服务器: {e}")
        print("请先启动授权服务器: python auth_server.py")
        return
    
    input("\n按 Enter 键开始演示...\n")
    
    # 演示各种授权模式
    demo_password_grant()
    time.sleep(1)
    
    demo_client_credentials_grant()
    time.sleep(1)
    
    demo_authorization_code_grant()
    time.sleep(1)
    
    demo_implicit_grant()
    
    print_section("演示完成")
    print("\n📚 学习建议:")
    print("  1. 先阅读 README.md 了解 OAuth 2.0 基础概念")
    print("  2. 启动授权服务器: uv run python auth_server.py")
    print("  3. 启动客户端应用: uv run python client_app.py")
    print("  4. 访问 http://localhost:8001 体验完整的授权流程")
    print("  5. 运行本脚本查看各种模式的API调用: uv run python demo.py")
    print("\n" + "=" * 60 + "\n")


if __name__ == "__main__":
    main()
