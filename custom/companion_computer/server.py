import socket
import json
import subprocess
import os

UDP_IP = "0.0.0.0"
UDP_PORT = 6000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on {UDP_PORT}...")

while True:
    data, addr = sock.recvfrom(1024)
    print(data, addr)
    try:
        msg = json.loads(data.decode())
        if msg.get("type") == "exec":
            cmd = msg.get("cmd")

            if cmd == 'start_ros':
                bash = './start_ros.sh'
            elif cmd == 'detect':
                bash = './detect.sh'
            else:
                bash = None
                output = f"Unknown cmd: {cmd}"

            if bash:
                result = subprocess.run(f"bash {bash}", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=5)
                output = (result.stdout + result.stderr).decode()

            response = {
                "cmd": cmd,
                "output": output
            }

            # 回传结果
            sock.sendto(json.dumps(response).encode(), addr)
    except Exception as e:
        print(f"Error: {e}")
