import socket
import json
import sys
import errno
import threading
import gl
import doSql

def handle_connection(sock,address):
	gl.conn=gl.connect()
	try:
		while True:
			byteData=sock.recv(1024)
			if not byteData:
				print("disconnect from %s:%s"%address)
				break
			else:
				recvData=json.loads(byteData.decode('utf-8'))
				sendData=doSql.operate(recvData)
				sock.send(json.dumps(sendData).encode('utf-8'))
	except:
		raise
	gl.conn.close()
	sock.close()

def main():
	port=gl.config["port"]
	listen=gl.config["listen"]
	server=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	server.bind((listen,port))
	server.listen(10)
	while True:
		try:
			client_socket,client_address=server.accept()
		except IOError:
			if e.errno==errno.EINTR:
				continue
			else:
				raise
		else:
			print("Get connection from %s:%s"%client_address)
			t=threading.Thread(target=handle_connection,args=(client_socket,client_address))
			t.start()

if __name__ == '__main__':
	main()
