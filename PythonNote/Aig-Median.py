# 题目：寻找中位数，允许元素重复出现
def medianPatrtion(mylist,nSmallest):
	lit=[elem for elem in mylist if elem<mylist[0]]
	equl=[elem for elem in mylist if elem==mylist[0]]
	grt=[elem for elem in mylist if elem>mylist[0]]
	
	if len(lit)==0 and len(grt)==0:
		return equl,0
	
	if len(lit)>=nSmallest:
		return medianPatrtion(lit,nSmallest)
	elif (len(lit)+len(equl)) >= nSmallest:
		return medianPatrtion(equl,nSmallest-len(lit))
	else:
		return medianPatrtion(grt,nSmallest-len(lit)-len(equl))

if __name__=='__main__':
	mylist=[11,2,3,4,5,5,6,6,17]
	medianlist,index=medianPatrtion(mylist,(len(mylist)+1)/2)
	print medianlist[index]






