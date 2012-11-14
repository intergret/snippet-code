# -*- coding: utf-8 -*-
# 题目：有一个没有排序，元素个数为2N的正整数数组。要求把它分割为元素个数为N的两个数组，并使两个子数组的和最接近。

# 解法：
# f(k,i,onesum) 表示从前k个数里面选出i个数和为sum能否成立。
# f(k,i,onesum) = true; if(f(k-1,i-1,onesum-arr[i])==true || f(k-1,i,onesum)==true)

from collections import defaultdict

def SplitList(myList):
	isSumOk= defaultdict(dict)
	halfSum=sum(myList)/2
	N=len(myList)/2
	
	# isSumOk[i][onesum]
	for i in range(0,N+1,1):
		for onesum in range(0,halfSum+1,1):
			isSumOk[i][onesum]=False
	
	isSumOk[0][0]=True
	
	for k in range(1,2*N+1,1):
		for i in range(min(k,N),0,-1):
			for onesum in range(0,halfSum+1,1):
				print k,i,onesum
				if(onesum >= myList[k-1] and isSumOk[i-1][onesum-myList[k-1]]):
					isSumOk[i][onesum] = True;

	for oneSum in range(halfSum,0,-1):
		if isSumOk[N][oneSum]:
			print oneSum
			break

if __name__ == '__main__':
	myList=[100,2,3,8,9,1]
	SplitList(myList)