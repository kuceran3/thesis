import sys

# in.csv (starting Dimenions) (Dimensions Size) (attributes 1/0)
with open(sys.argv[1], 'r') as data, open(sys.argv[1].split('.')[0] + '_patternBig.csv', 'w') as pattern:
	header = data.readline()[:-1].split(',')
	dims = 0
	attr = 0
	print(header)
	for h in header:
		if ('dim' in h):
			dims += 1
		else:
			attr += 1

	start = list(map(int, sys.argv[2:2+dims]))
	size = list(map(int, sys.argv[2+dims:2+(2*dims)]))
	attrs = list(map(int, sys.argv[2+(2*dims):2 + (2*dims) + attr]))

	print(start)
	print(size)
	print(attrs)
	for i in range(len(size)):
		if (size[i] > 1):
			pattern.write(header[i].split('dim')[0] + 'dim' + str(size[i]) + ',')
	for i in range(attr - 1):
		if (attrs[i] >= 1):
			pattern.write(header[dims + i] + ',')
	if (attrs[-1] >= 1):
		pattern.write(header[-1] + '\n')

	prev = -1
	dN = [0] * dims
	while (True):
		line = data.readline()
		if (not line):
			break
		line = list(map(int, line[:-1].split(',')))

		if (line[0] > prev):
			print(line)
			prev = line[0]
		t = True
		for i in range(dims):
			if (line[i] < start[i] or line[i] > (start[i] + size[i] - 1)):
				t = False
				break
		if (not t):
			continue
		
		for i in range(dims):
			if (size[i] > 1):
				pattern.write(str(dN[i]) + ',')
				#pattern.write(str(line[i]) + ',')
		for i in range(dims - 1, -1, -1):
			dN[i] += 1
			if (dN[i] >= size[i]):
				dN[i] = 0
			else:
				break

		for i in range(attr - 1):
			if (attrs[i] >= 1):
				pattern.write(str(line[dims + i]) + ',')
		if (attrs[-1] >= 1):
			pattern.write(str(line[-1]) + '\n')
				

