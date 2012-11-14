# -*- coding: utf-8 -*-
# 题目：输入一个整形数组，数组里有正数也有负数。数组中连续的一个或多个整数组成一个子数组，每个子数组都有一个和。求所有子数组的和的最大值。

# 解法一： 
# 最直接的解法当然是穷举遍历了，把所有的子数组列出来，然后计算和。
# 复杂度可以简单的想出来：设置两个变量i和j为子数组边界，这两个变量都要遍历整个数组，然后还需要一个游标k，来遍历整个子数组以求和。所以总的复杂度是O（n^3）。

def maxSubSum1(MyList):
	ListLength=len(MyList)
	sum=max=0
	for Lindex in range(ListLength): 
		for Rindex in range(Lindex,ListLength):
			sum=0
			for index in range(Lindex,Rindex+1): 
				sum+=MyList[index]
			print 'F',Lindex,'T',Rindex,'=',sum
			
			if sum>max:
				max=sum
	
	print max


# 解法一改进版：
# 仔细琢磨就会发现，其实不需要再使用k去遍历子数组，因为每次j移动都会产生新的子数组，所以只要在每次j移动时进行一下比较，就不会把最大值漏掉。所以只有i和j移动，复杂度降低到O（n^2）。
def maxSubSum2(MyList):
	ListLength=len(MyList)
	max=0
	for Lindex in range(ListLength):
		sum=0
		for Rindex in range(Lindex,ListLength): 
			sum+=MyList[Rindex]
			
			print 'F',Lindex,'T',Rindex,'=',sum
			
			if sum>max:
				max=sum
	
	print max


# 解法二：分治算法
# 跟二分查找的思想相似，我们可以分情况讨论这个问题是不是符合二分查找的条件。
# 情况1.这个满足最大和的子数组全部在本数组的左半部或者右半部。例如：左半部A[i]……A[n/2-1]或者右半部A[n/2]……A[j]。这种情况下可以直接使用递归调用。
# 情况2.满足最大和的子数组跨过了本数组的中间点。例如：A[i]……A[n/2-1] A[n/2]……A[j]连续。则这种情况下只要在左半部寻找以A[n/2-1]结尾，在右半部寻找以A[n/2]开头的两个满足最大和的连续数组，并求和即可。由于这个已知起点，只需要一个游标即可，所以复杂度是2*O（n/2）=O（n）。
# 综合以上两种情况，满足分治算法递归式：T（n）=2T（n/2）+O（n）=O（n*logn）。

def maxSubSum3(MyList,Left,Right):
	if Left==Right:
		return MyList[Left] if MyList[Left]>0 else 0
	
	Center = (Left+Right)/2;   
	LeftMaxSum = maxSubSum3(MyList,Left,Center)
	RightMaxSum = maxSubSum3(MyList,Center+1,Right) 
	
	LeftPartSum=MaxLeftPartSum=0
	for Lindex in range(Center,Left-1,-1):
		LeftPartSum+=MyList[Lindex]
		if LeftPartSum > MaxLeftPartSum: MaxLeftPartSum=LeftPartSum 

	RightPartSum=MaxRightPartSum=0
	for Rindex in range(Center+1,Right+1,1):
		RightPartSum+=MyList[Rindex]
		if RightPartSum > MaxRightPartSum:MaxRightPartSum=RightPartSum
	
	print 'F',Left,'T',Right,'=',max(LeftMaxSum,RightMaxSum,MaxLeftPartSum+MaxRightPartSum)
	return max(LeftMaxSum,RightMaxSum,MaxLeftPartSum+MaxRightPartSum)

# 解法三：
# 这个从A[0]到A[n-1]的最大和子数组问题分解成：
# 所求子数组中包含A[0]。如果不包含A[1]，则A[0]自己满足条件，此时Max（A[0]……A[n-1]）=A[0]。如果包含A[1]，则Max（A[0]……A[n-1]）=A[0]+Max（A[1]……A[n-1]）。
# 所求子数组中不包含A[0]。Max（A[0]……A[n-1]）=Max（A[1]……A[n-1]）。
# 最终结果取以上三者的最大值即可，即Max（A[0]……A[n-1]）=max（ A[0], A[0]+Max（A[1]……A[n-1]）, Max（A[1]……A[n-1]））。

def maxSubSum4(MyList,ListLength):
	NIncludeLargest=MyList[ListLength-1]
	NAllLargest=MyList[ListLength-1]
	for index in range(ListLength-2,-1,-1):
		NIncludeLargest=max(MyList[index],MyList[index]+NIncludeLargest)
		NAllLargest=max(NIncludeLargest,NAllLargest)
		print 'F',index,'T',ListLength-1,'=',NAllLargest
	
	return NAllLargest

if __name__ == '__main__':
	# maxSubSum1([1,-2,3,10,-4,7,2,-5])
	# maxSubSum1([-100,-2,-3,10,-4,-7,-2,1005])
	# print '*'*20
	# maxSubSum2([1,-2,3,10,-4,7,2,-5])
	# maxSubSum2([-100,-2,-3,10,-4,-7,-2,1005])
	# print '*'*20
	# print maxSubSum3([1,-2,3,10,-4,7,2,-5],0,len([1,-2,3,10,-4,7,2,-5])-1)
	# print maxSubSum3([-100,-2,-3,10,-4,-7,-2,1005],0,len([-100,-2,-3,10,-4,-7,-2,1005])-1)
	# print '*'*20
	print maxSubSum4([1,-2,3,10,-4,7,2,-5],len([1,-2,3,10,-4,7,2,-5]))
	print maxSubSum4([-100,-2,-3,10,-4,-7,-2,1005],len([-100,-2,-3,10,-4,-7,-2,1005]))