import socket
import json
import subprocess
import os
import logging
import threading

UDP_IP = "0.0.0.0"
UDP_PORT = 6000

logging.basicConfig(
    filename='udpserver_error.log',
    level=logging.ERROR,
    format='%(asctime)s [%(levelname)s] %(message)s'
)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on {UDP_PORT}...")

def run_script_realtime(cmd, script_path, addr):
    try:
        with subprocess.Popen(
            ["bash", script_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            bufsize=1,
            text=True
        ) as proc:
            for line in proc.stdout:
                sock.sendto(json.dumps({
                    "cmd": cmd,
                    "output": line.strip()
                }).encode(), addr)

            proc.wait()

    except Exception as e:
        logging.error(f"[ERROR]{cmd}.sh -> {e}")

while True:
    data, addr = sock.recvfrom(1024)
    print(data, addr)
    
    cmd = None
    script_file = ""
    try:
        msg = json.loads(data.decode())
        if msg.get("type") == "exec":
            cmd = msg.get("cmd")

            if not cmd:
                continue

            if cmd == 'ros':
                script_file = "./start_ros.sh"
            elif cmd == 'imageDetect':
                script_file = "./detect.sh"
            elif cmd == 'intercept':
                script_file = "./detect.sh"
            elif cmd == 'check':
                script_file = "./detect.sh"
            else:
                continue

            if not os.path.isfile(script_file):
                logging.error(f"找不到文件: {script_file}")
                continue

            threading.Thread(
                target=run_script_realtime,
                args=(cmd, script_file, addr),
                daemon=True  # 设置为守护线程，主线程退出时它也会结束
            ).start()

    except Exception as e:
        logging.error(f"[ERROR]{cmd}.sh -> {e}")