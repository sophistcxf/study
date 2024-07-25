import subprocess

process = subprocess.Popen("./test.sh", stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

while True:
    output = process.stdout.readline()
    if output == '' and process.poll() is not None:
        break
    if output:
        print(output.strip())
