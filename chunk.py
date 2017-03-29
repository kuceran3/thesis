import sys
import csv

def movePos(pos, dim):
	over = 1
	for i in range(1, len(dim)):
		pos[-i] += 1
		if (pos[-i] >= dim[- i - 1][1]):
			pos[-i] = 0
		else:
			break
	return pos

if (len(sys.argv) < 2):
	print("Usage: python chunk.py <INPUT_FILE>")
	exit()

with open(sys.argv[1], 'r') as csvF:
	with open(sys.argv[1].split('.')[0] + "_meta.csv", "w") as meta:
		cF = csv.reader(csvF, delimiter=',')

		header = next(cF)
		dim = []
		attr = []
		i = 0
		j = 0

		for item in header:
			#if ((item.split(":"))[1].find("dim", 0, len((item.split(":"))[1])) >= 0):
			if (item.find(":dim") >= 0):
				dim.append([(item.split(":"))[0], int((item.split(":"))[1][3:])])
				i += 1
			else:
				attr.append([(item.split(":"))[0], (item.split(":"))[1]])
				j += 1
		for d in dim:
			meta.write(d[0] + ":dim" + str(d[1]) + ',')
		for a in range(0, j):
			if (a < j - 1):
				meta.write(str(attr[a][0]) + ":" + str(attr[a][1]) + ",")
			else:
				meta.write(str(attr[a][0]) + ":" + str(attr[a][1]) + '\n')

		s = 1
		pos = [0] * (len(dim) - 1)
		for x in range(0, i - 1):
			#print(dim[x][1], type(dim[x][1]))
			s *= dim[x][1]
		#s = int(s)
		for x in range(0, s):
			fileNameNum = ''
			for p in pos:
				fileNameNum += '_' + str(p)
			fileName = sys.argv[1].split('.')[0] + fileNameNum + '.bin' 
			with open(fileName, 'wb') as out:
				#name = sys.argv[1].split('.')[0] + "_bin" + str(x) + ".bin"
				meta.write(fileName + '\n')
				count = x*s
				for row in cF:
					#print(row)
					#print(int(row[len(dim) - 1]).to_bytes(2, byteorder='little'))
					out.write(int(row[len(dim) - 1]).to_bytes(2, byteorder='little'))
					for i in range(len(row[len(dim):])):
						if (attr[i][1] == 'int'):
							it = int(row[len(dim) + i])
							#print(it)
							#print(it.to_bytes(2, byteorder='little'))
							#print(str(len(bytes(int(row[len(dim) + i])))))
							#rint(str(int(row[len(dim) + i])))
							out.write(it.to_bytes(2, byteorder='little'))
						#elif (attr[i][1] == 'float'):
						#	out.write(bytes(float(row[len(dim) + i])))
						else:
							#print("not ok")
							out.write(int(len(row[len(dim) + i]) + 1 ).to_bytes(2, byteorder='little') )
							out.write(bytes(row[len(dim) + i]).encoding('UTF-8'))
							out.write(0)
					count += 1
					if(count > x*s + int(dim[len(dim) - 1][1]) - 1):
						break;
				pos = movePos(pos, dim)
