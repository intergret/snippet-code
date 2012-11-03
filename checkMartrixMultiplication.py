import os
os.system("rm M_10_j.data")
os.system("rm N_j_95.data")

os.system("grep ^M#10# M.data >> M_10_j.data")
os.system("grep ^N#[0-9]*#95 M.data >> N_j_95.data")

m10j = [0]*151
nj95 = [0]*151
mf = open("M_10_j.data",'r')
for onel in mf:
	splititem = onel.split("#")
	m10j [int(splititem[2])] = int(splititem[3])
mf.close()

nf = open("N_j_95.data",'r')
for onel in nf:
	splititem = onel.split("#")
	nj95 [int(splititem[1])] = int(splititem[3])
nf.close()

sum = 0;
for j in range(1,151):
	sum += m10j[j] * nj95[j]

print "[10,95] should be:" + str(sum)

print "compute with MR is:" + os.popen("grep ^10#95 Result.data").read()