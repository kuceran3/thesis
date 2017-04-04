import sys
import csv

class Mask:
	valid = 0
	a = 0
	fileName = None

	def __init__(self, f):
		self.valid = 0
		self.a = 0
		self.fileName = f

	def addZero(self):
		self.a *= 2
		self.check()

	def addOne(self):
		self.a = self.a*2 + 1
		self.valid += 1
		self.check()

	def check(self):
		if (self.valid >= 8):
			#print(self.a)
			self.fileName.write((self.a).to_bytes(1, byteorder='little'))
			self.valid = 0
			self.a = 0

	def writeEnd(self):
		if (self.valid <= 0):
			return
		for i in range(self.valid, 8):
			self.a *= 2
		#print(self.a)
		self.fileName.write((self.a).to_bytes(1, byteorder='little'))
		self.a = 0


class Eighth:
	bitMask = []
	vals = []
	cells = 0
	split = 0

	def __init__(self, noA, s):
		self.bitMask = [0]*noA
		self.vals = []
		self.cells = 0
		self.split = s

	def addCell(self, row, l, attr):
		#print("addCell eighth\n")
		#print("vals: " + str(self.vals))
		#print(row[l-1])
		for i in range(l - self.split, l):
			self.vals += (int(row[i]).to_bytes(2, byteorder='little'))
		#print(self.vals)
		self.cells += 1
		for i in range(len(row[l:])):
			self.bitMask[i] *= 2
			it = row[l + i]
			if (attr[i][1] == 'int'):
				it = row[l + i]
				#print("int: " + it)
				if (row[l + i] != ''):
					self.vals += (int(it).to_bytes(2, byteorder='little'))
					self.bitMask[i] += 1
			else:
				if (row[l + i] == ''):
					self.bitMask[i] += 1
					self.vals += (len(row[l + i]) + 1).to_bytes(2, byteorder='little')
					self.vals += bytes(row[l + i]).encoding('UTF-8')
					self.vals += (0).to_bytes(1, byteorder='little')

	def write(self, f):
		#print("write eighth")
		for b in self.bitMask:
			f.write(b.to_bytes(1, byteorder='little'))
		for b in self.vals:
			f.write(b.to_bytes(1, byteorder='little'))

	def writeEnd(self, f):
		#print("write end")
		for i in range(self.cells, 8):
			for j in range(len(self.bitMask)):
				self.bitMask[j] *= 2
		self.write(f)

def movePos(pos, dim, split):
	#print("movePos\n")
	over = 1
	for i in range(len(dim) - split - 1, -1, -1):
		pos[i] += 1
		if (pos[i] >= dim[i][1]):
			pos[i] = 0
		else:
			break
	return pos

def fillMask(mask, row, index, dim, end = False):
	#print("fillMask\n")
	#print(str(row))
	#tmp = [0]*(len(dim))
	#print("tmp: " + str(tmp) + " len: " + str(len(dim)))

	while (index != list(map(int, row[:len(dim)]))):
		mask.addZero()
		#print(str(row))
		#print("index: " + str(index) + " row: " + str(list(map(int, row[:len(dim)]))) + " " + str(len(dim)))
		for x in range(len(dim) - 1, -1, -1):
			#print("index[x]: " + str(index[x]) + " dim[x][1]: " + str(dim[x][1]))
			index[x] += 1
			if (index[x] < dim[x][1]):
				break
			else:
				index[x] = 0

	if (not end):
		mask.addOne()

		for x in range(len(dim) - 1, -1, -1):
			index[x] += 1
			if (index[x] < dim[x][1]):
				break
			else:
				index[x] = 0	

	return mask, index

if (len(sys.argv) < 2):
	print("Usage: python chunk.py <INPUT_FILE>")
	exit()

with open(sys.argv[1], 'r') as csvF:
	with open(sys.argv[1].split('.')[0] + "_meta.csv", "w") as meta, open(sys.argv[1].split('.')[0] + "_mask.bin", "wb") as mask:
		cF = csv.reader(csvF, delimiter=',')

		header = next(cF)
		dim = []
		attr = []
		i = 0
		j = 0

		m = Mask(mask)

		for item in header:
			if (item.find(":dim") >= 0):
				dim.append([(item.split(":"))[0], int((item.split(":"))[1][3:])])
				i += 1
			else:
				attr.append([(item.split(":"))[0], (item.split(":"))[1]])
				j += 1
		for d in dim:
			meta.write(d[0] + ":dim" + str(d[1]) + ',')
		split = 0
		size = 1
		for d in range(len(dim) -1, -1, -1):
			size *= dim[d][1]
			if (size > 200000):
				break
			split += 1

		for a in range(0, j):
			if (a < j - 1):
				meta.write(str(attr[a][0]) + ":" + str(attr[a][1]) + ",")
			else:
				meta.write(str(attr[a][0]) + ":" + str(attr[a][1]) + '\n')
		meta.write("dimName: " + str(split - 1))
		
		s = 1
		pos = [0] * (len(dim) - split)
		for x in range(0, i - split):
			s *= dim[x][1]
		index = [0]*len(dim)

		for x in range(0, s):
			fileNameNum = ''
			for p in pos:
				fileNameNum += '_' + str(p)
			fileName = sys.argv[1].split('.')[0] + fileNameNum + '.bin' 
			#print(fileName)
			with open(fileName, 'wb') as out:
				ec = 0
				e = Eighth(len(attr), split)
				for row in cF:
					m, index = fillMask(m, row, index, dim)

					e.addCell(row, len(dim), attr)
					ec += 1
					if(ec % 8 == 0):
						e.write(out)
						e = Eighth(len(attr), split)
						ec = 0
					if (index[len(dim) - split:] == [0] * split):
						break
				e.writeEnd(out)

				pos = movePos(pos, dim, split)
		fillMask(m, [0] * (len(dim)), index, dim, True)
		m.writeEnd()
