#!/usr/bin/python

import sys
import random

def loop_rec(length, dimNum, pos, printDim, attNum, attMax, outF):
	if (dimNum > 1):
		for j in range(0, length):
			printDim[pos] = j
			loop_rec(length, dimNum - 1, pos + 1, printDim, attNum, attMax, outF)
	else:
		for j in range(0, length):
			printDim[pos] = j
			for x in range(0, len(printDim)):
				#print(printDim[x], ",", sep="", end="")
				outF.write(str(printDim[x]) + ",")
			for x in range(0, attNum):
				#print(random.randint(0, attMax), sep='', end='')
				outF.write(str(random.randint(0, attMax)))
				#outF.write(str(1))
				if(x + 1 < attNum):
				#	print(",", sep='', end='')
					outF.write(",")
			#print()
			outF.write('\n')

if (len(sys.argv) < 6):
	print("Usage: python generate.py <NUM_OF_DIMENSIONS> <DIMENSION_LENGTH> <NUM_OF_ATTRIBUTES> <MAX_ATTRIBUTE_VALUE> <OUTPUT_FILE>")
	exit()

script = sys.argv[0]
dimNum = int(sys.argv[1])
length = int(sys.argv[2])
attNum = int(sys.argv[3])
attMax = int(sys.argv[4])
outF = sys.argv[5]

outF = open(outF, 'w')

for x in range(0, dimNum):
	#print(x, ":dim", length, ",", sep='', end='')
	outF.write(str(x) + ":dim" + str(length) + ",")
for x in range(0, attNum):
	#print(x, ":int", sep='', end='')
	outF.write(str(x) + ":int")
	if (x < attNum - 1):
	#	print(",", sep='', end='')
		outF.write(",")
#print()
outF.write('\n')

printDim = []
for x in range(0, dimNum):
	printDim.append(0)

loop_rec(length, dimNum, 0, printDim, attNum, attMax, outF)

outF.close()
