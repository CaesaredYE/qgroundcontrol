import socket
import json
import threading

LOCAL_PORT = 6001
REMOTE_IP = "172.16.0.48"
REMOTE_PORT = 6000

recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
recv_sock.bind(("0.0.0.0", LOCAL_PORT))

def receive_loop():
    print(f"监听端口 {LOCAL_PORT} 等待返回结果...")
    while True:
        data, addr = recv_sock.recvfrom(4096)
        print(f"\n收到远端 {addr} 的结果：\n{data.decode()}")


def send_command(command: str):
    message = {
        "type": "exec",
        "cmd": command
    }
    payload = json.dumps(message).encode("utf-8")
    recv_sock.sendto(payload, (REMOTE_IP, REMOTE_PORT))

if __name__ == "__main__":
    threading.Thread(target=receive_loop, daemon=True).start()
    print("输入命令回车执行。输入 'exit' 退出。\n")

    while True:
        user_input = input("请输入命令：")
        if user_input.lower() in ["exit", "quit"]:
            break
        if user_input.strip():
            send_command(user_input)
