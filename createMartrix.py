import random

i = 300 ;j = 150 ;k = 500

mf = open("M.data",'w+')
for r in range(1,i+1):
	for c in range(1,j+1):
		mf.write('M#'+str(r)+"#"+str(c)+"#"+str(random.randint(0,100))+"\n");
		
for r in range(1,j+1):
	for c in range(1,k+1):
		mf.write('N#'+str(r)+"#"+str(c)+"#"+str(random.randint(0,100))+"\n");

mf.close()
		
	
