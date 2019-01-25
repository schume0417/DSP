#coding=Big5

if __name__ == '__main__':
	map = {}
	file = open('Big5-ZhuYin.map','r',encoding = 'Big5hkscs')  #Big5-ZhuYin.map
	out = open('ZhuYin-Big5.map', 'w', encoding='Big5hkscs')
	for line in file:
		a = line.split(' ')
		c = a[0]
		a[1].strip()
		zhuyin = a[1].split('/')
		zhuyin[-1] = zhuyin[-1][:-1]
		#print(zhuyin)
		#map[c] = [c]
		for i in zhuyin:
			b = i[0]
			if b not in map:
                            map[b] = [c]
			else:
				map[b].append(c)
	#print(map)
	for i in map:
		map[i] = list(set(map[i])) 
	#print(map)
	for i in map:
		out.write(i+' ')
		for j in map[i]:
			out.write(j+' ')
		out.write('\n')
		for j in map[i]:
			out.write(j+' '+j+'\n')

