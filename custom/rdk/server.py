import socket
import json
import subprocess
import os
import logging
import threading
import traceback

UDP_IP = "0.0.0.0"
UDP_PORT = 6000

logging.basicConfig(
    filename='udpserver.log',
    level=logging.INFO,
    format='%(asctime)s [%(levelname)s] %(message)s'
)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
print(f"[INFO] UDP server listening on {UDP_IP}:{UDP_PORT}")

COMMAND_SCRIPTS = {
    "ros": "./start_ros.sh",        # 启动ros
    "imageDetect": "./detect.sh",   # 启动图像检测
    "intercept": "./detect.sh",     # 启动拦截主程序
    "check": "./detect.sh"          # 启动飞机自检程序
}

def run_script_realtime(cmd, script_path, addr):
    try:
        logging.info(f"Start running script for cmd: {cmd}, from: {addr}")

        with subprocess.Popen(
            ["bash", script_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            bufsize=1,
            text=True
        ) as proc:
            for line in proc.stdout:
                output = line.strip()
                message = json.dumps({"cmd": cmd, "output": output})
                sock.sendto(message.encode(), addr)

            proc.wait()

    except Exception as e:
        logging.error(f"[ERROR] Running {cmd}.sh failed: {e}")
        logging.error(traceback.format_exc())

def handle_request(data, addr):
    try:
        msg = json.loads(data.decode())
        if msg.get("type") != "exec":
            return

        cmd = msg.get("cmd")
        if not cmd:
            return

        script_file = COMMAND_SCRIPTS.get(cmd)
        if not script_file:
            logging.warning(f"[WARN] Unsupported command: {cmd}")
            return

        if not os.path.isfile(script_file):
            logging.error(f"[ERROR] Script not found: {script_file}")
            return

        threading.Thread(
            target=run_script_realtime,
            args=(cmd, script_file, addr),
            daemon=True
        ).start()

        logging.info(f"Thread started for cmd: {cmd}, addr: {addr}")

    except Exception as e:
        logging.error(f"[ERROR] Failed to handle request from {addr}: {e}")
        logging.error(traceback.format_exc())

def start_udp_server():
    while True:
        try:
            data, addr = sock.recvfrom(1024)
            logging.info(f"Received data from {addr}: {data.decode().strip()}")
            handle_request(data, addr)
        except Exception as e:
            logging.error(f"[ERROR] Failed to receive data: {e}")
            logging.error(traceback.format_exc())

if __name__ == "__main__":
    start_udp_server()