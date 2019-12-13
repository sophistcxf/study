import subprocess

proc = subprocess.Popen(['ls'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
stdout_str = proc.communicate()[0]
code = proc.returncode
#print code
print stdout_str
