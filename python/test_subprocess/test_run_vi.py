import subprocess
import os

if os.getenv('EDITOR') is None:
    print("no EDITOR")

#editor = os.getenv('EDITOR', 'vi')

#subprocess.call([editor, '/tmp/log'])
