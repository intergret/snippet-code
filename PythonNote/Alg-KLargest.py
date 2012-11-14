# -*- coding: utf-8 -*-
# 题目：寻找N个数中最大的K个数

import math,heapq,random
from timeit import Timer

# 解法一：	   
# 寻找N个数中最大的K个数，本质上就是寻找最大的K个数中最小的那个，也就是第K大的数。可以使用二分搜索的策略来寻找N个数中的第K大的数。对于一个给定的数p，可以在O（N）的时间复杂度内找出所有不小于p的数。假如N个数中最大的数为Vmax，最小的数为Vmin，那么这N个数中的第K大数一定在区间[Vmin, Vmax]之间。那么，可以在这个区间内二分搜索N个数中的第K大数p。
def KLargest1(myList,N):
	maxElement=max(myList)
	minElement=min(myList)
	while ((maxElement-minElement)>0.5):
		midElement=minElement+(maxElement-minElement)*0.5
		numK=len([element for element in myList if element >= midElement])
		if numK>=N:
			minElement=midElement
		else:
			maxElement=midElement
	
	kLargest=[oneElement for oneElement in myList if oneElement>minElement]
	# print kLargest


# 解法二：维护一个元素个数为K的最小堆，遍历一遍数组后即获得最大的K个数
# The heapq Module:http://docs.python.org/library/heapq.html
def KLargest2(myList,N):
	kLargest=myList[0:N]
	heapq.heapify(kLargest)
	for index in range(N,len(myList)):
		if myList[index]>kLargest[0]:
			heapq.heapreplace(kLargest,myList[index])
	# print kLargest


# 解法三：
# 如果所有N个数都是正整数，且它们的取值范围不太大，可以考虑申请空间，记录每个整数出现的次数，然后再从大到小取最大的K个。比如，所有整数都在（0, MAXN）区间中的话，利用一个数组count[MAXN]来记录每个整数出现的个数（count[i]表示整数i在所有整数中出现的个数）。我们只需要扫描一遍就可以得到count数组。然后，寻找第K大的元素
def KLargest3(myList,N):
	maxElement=max(myList)
	countElement=[0 for index in range(maxElement+1)]
	for eachElement in myList:
		countElement[eachElement]+=1
	
	sunCount=0;kLargest=[]
	for eachElement in range(maxElement,0,-1):
		sunCount+=countElement[eachElement]
		if sunCount<=N:
			kLargest.extend([eachElement]*countElement[eachElement])
		else:
			break
	# print kLargest
	

if __name__=='__main__':
	myList=[random.randint(1,100000) for one in range(1000)]
	# KLargest1(myList,5)
	# KLargest2(myList,5)
	# KLargest3(myList,5)
	
	# 测时三次，每次执行函数10000遍。看三次中最久的时间。由于数组中的数取值范围太大，解法二完胜
	t1=Timer("KLargest1(myList,5)","from __main__ import *")
	print max(t1.repeat(3,10000))
	
	t2=Timer("KLargest2(myList,5)","from __main__ import *")
	print max(t2.repeat(3,10000))
	
	t3=Timer("KLargest3(myList,5)","from __main__ import *")
	print max(t3.repeat(3,10000))