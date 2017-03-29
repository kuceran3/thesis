import sys
import struct

if (len(sys.argv) < 2):
	print("Usage: python readChunk.py <INPUT_FILE>")
	exit()

with open(sys.argv[1], 'rb') as inp:
	while(True):
		a = inp.read(1)
		if(not a):
			break
		print(struct.unpack('b', a))
