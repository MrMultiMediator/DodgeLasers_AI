import sys,os
limit = int(sys.argv[1])
counter = 0

g = open('mv.sh','w')

while counter < limit:
	g.write('mv p_data'+str(limit-counter)+'.txt p_data'+str(limit-counter+1)+'.txt\n')
	g.write('mv g_data'+str(limit-counter)+'.txt g_data'+str(limit-counter+1)+'.txt\n')

	counter += 1

g.write('mv p_data.txt p_data1.txt\n')
g.write('mv g_data.txt g_data1.txt\n')

g.close()
