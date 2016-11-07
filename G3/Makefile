genkey: rsa.py genkey.py
	python3 genkey.py -m ${MPRIME} -n ${NPRIME}

keycrack: rsa.py cracker.py
	python3 cracker.py -e ${E} -c ${C}

server: rsa.py server.py
	python3 server.py -port ${PORT}

client: rsa.py client.py
	python3 client.py -host ${SERVER} -port ${PORT}
