#!/usr/bin/python

import sys
import random
import numpy as np
import math
#length, length/pat1D, dimNum, 0, printDim, pat, pos, distr, outF
#def loop_rec(length, size, dimNum, pos, printDim, pat, pos, distr, attNum, attMax, outF):
#	if (dimNum > 1):
#		for j in range(0, length):
#			printDim[pos] = j
#			loop_rec(length, dimNum - 1, pos + 1, printDim, attNum, attMax, outF)
#	else:
#		for j in range(0, length):
#			printDim[pos] = j
#			if (gauss())
#
#			else
#				for x in range(0, len(printDim)):
#					#print(printDim[x], ",", sep="", end="")
#					outF.write(str(printDim[x]) + ",")
#				for x in range(0, attNum):
#					#print(random.randint(0, attMax), sep='', end='')
#					outF.write(str(random.randint(0, attMax)))
#					if(x + 1 < attNum):
#					#	print(",", sep='', end='')
#						outF.write(",")
#				#print()
#				outF.write('\n')

def loop_rec(length, size, dimNum, pat, pos, distr, outF):
	for x in range(0, length):
		print(x)
		pos1 = int(x/size)
		pop1 = x % size
		for y in range(0, length):
			pos2 = int(y/size)
			pop2 = y % size
			for z in range(0, length):
				pos3 = int(z/size)
				pop3 = z % size
				#try:
				if(pos[pos1][pos2][pos3]):
					outF.write(str(x) + "," + str(y) + "," + str(z) + ",")
					outF.write(str(pat[pop1][pop2][pop3]) + '\n')
				#except Exception:
				#	print(pos1, pos2, pos3, len(pos), z, size)
				#	asd = input()
				else:
					tmp = random.random()
					if (tmp < distr[pos1][pos2][pos3]):
						outF.write(str(x) + "," + str(y) + "," + str(z) + ",")
						outF.write(str(random.randint(0,10)) + '\n')

def readPattern(file, size, dim):
	pat = []
	file.readline()
	for x in range(0, size ** dim):
		pat.append(file.readline()[:-1].split(',')[-1])
	res = []
	for x in range(0, dim - 1):
		res = []
		for y in range(0, len(pat), size):
			res.append(pat[y : y + size])
		pat = []
		for y in res:
			pat.append(y)
	#print(len(pat), len(pat[0]), len(pat[0][0]))
	return res

def gaussMean(dim, l):
	gaussNum = 3
	one = []
	two = []
	three = []
	for y in range(0, dim):
		one.append(int(l/3))
		two.append(int(l / 2) )
		three.append(int( (l*5) / 6) )
	two[0] = int(l/3)
	three[1] = int((l*2)/3)
	return [one, two, three]
	#return [one]

def gaussVal(mean, v, dim, corr):
	res = 0
	mean = np.array(mean)
	v = np.array(v)
	res = np.exp((-0.5 * np.dot((v - mean), (v - mean).T)) / corr)
	res = res / (math.sqrt((2 * math.pi * corr) ** dim))
	return res

def gaussSum(mean, v, dim, pos):
	res = 0
	for x in mean:
		res += gaussVal(x, v, dim, float(pos)/3)
	return res * 5

def genPos(dim, length, density, percres, patS):
	pos = int(length / patS)
	mean = gaussMean(dim, pos)
	distr = []
	patPos = []
	true = 0
	for x in range(0, pos):
		distr.append([])
		patPos.append([])
		for y in range(0, pos):
			distr[x].append([])
			patPos[x].append([])
			for z in range(0, pos):
				g = gaussSum(mean, [x, y, z], dim, pos)
				distr[x][y].append(g * density * 200000000000)
				tmp = random.random()
				#print(tmp)
				if (tmp >= g * density * percres * 80 * patS*dim * (1/3)):
					patPos[x][y].append(False)
				else:
					patPos[x][y].append(True)
					true += 1

				#print(x, y, z, distr[x][y][z], patPos[x][y][z])
				#p = raw_input()
	print(true)
	return patPos, distr

if (len(sys.argv) < 7):
	print("Usage: python generate_Gauss.py <NUMBER_OF_DIM> <DIM_SIZE> <%_DENSITY> <%_OF_RESULTS> <PATTERN_SIZE> <PATTERN_FILE> <OUTPUT_FILE>")
	exit()

script = sys.argv[0]
outF = sys.argv[7]

dimNum = int(sys.argv[1])
length = int(sys.argv[2])
attNum = 1
attMax = 100
density = float(sys.argv[3]) 
results = float(sys.argv[4])
pat1D = int(sys.argv[5])
patSize = int(sys.argv[5]) ** dimNum
patF = sys.argv[6]
patF = open(patF, 'r')
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

pat = readPattern(patF, pat1D, dimNum)
pos, distr = genPos(dimNum, length, density, results, pat1D)
loop_rec(length, pat1D, dimNum, pat, pos, distr, outF)

patF.close()
outF.close()
