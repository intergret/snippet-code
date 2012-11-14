# -*- coding: utf-8 -*-
# 题目：设p1=(x1, y1), p2=(x2, y2), …, pn=(xn, yn)是平面上n个点构成的集合S，设计算法找出集合S中距离最近的点对。
import math,sys,random

# 解法一：
# 已知集合S中有n个点，一共可以组成n(n-1)/2对点对，蛮力法就是对这n(n-1)/2对点对逐对进行距离计算，通过循环求得点集中的最近点对.
def nearestPoint1(Points):
	nearestDistance=sys.maxint
	nearestPointA=[]
	nearestPointB=[]
	for onePoint in Points:
		for anotherPoint in Points:
			if onePoint != anotherPoint:
				distance=math.sqrt(math.pow((onePoint[0]-anotherPoint[0]),2)+math.pow((onePoint[1]-anotherPoint[1]),2))
				if distance<nearestDistance:
					nearestDistance=distance
					nearestPointA=onePoint
					nearestPointB=anotherPoint

	print 'F',nearestPointA,'T',nearestPointB,'=',nearestDistance


# 解法二：
# 算法描述：已知集合S中有n个点，分治法的思想就是将S进行拆分，分为2部分求最近点对。算法每次选择一条垂线L，将S拆分左右两部分为SL和SR，L一般取点集S中所有点的中间点的x坐标来划分，这样可以保证SL和SR中的点数目各为n/2
# 依次找出这两部分中的最小点对距离：δL和δR，记SL和SR中最小点对距离δ = min（δL，δR），
# 以L为中线，δ为半宽划分一个长带，最小距离对还有可能存在于SL和SR的交界处，p点和q点分别位于SL和SR的虚线范围内。p点不必与所有的q点计算距离，只需计算与满足关系式(q[x]-p[x])<=2*δ and |q[y]-p[y]|<=δ的q点的距离。
def nearestPoint2(Points):
	if len(Points)==1:return sys.maxint
	if len(Points)==2:return math.sqrt(math.pow((Points[0][0]-Points[1][0]),2)+math.pow((Points[0][1]-Points[1][1]),2))
	
	DivideX=sum([onePoint[0] for onePoint in Points])/len(Points)
	leftPoints=[onePoint for onePoint in Points if onePoint[0]<=DivideX]
	rightPoints=[onePoint for onePoint in Points if onePoint[0]>DivideX]
	
	leftNearestDistance=nearestPoint2(leftPoints)
	rightNearestDistance=nearestPoint2(rightPoints)
	
	MDist=min(leftNearestDistance,rightNearestDistance)
	leftMidPoints=[onePoint for onePoint in leftPoints if (DivideX-onePoint[0])<=MDist]
	rightMidPoints=[onePoint for onePoint in rightPoints if (onePoint[0]-DivideX)<=MDist]
	
	midNearestDistance=sys.maxint
	for oneleftPoint in leftMidPoints:
		partRightPoints=[onerightPoint for onerightPoint in rightMidPoints if (onerightPoint[0]-oneleftPoint[0])<=2*MDist and abs(onerightPoint[1]-oneleftPoint[1])<=MDist]
		for onerightPoint in partRightPoints:
			distance=math.sqrt(math.pow((oneleftPoint[0]-onerightPoint[0]),2)+math.pow((oneleftPoint[1]-onerightPoint[1]),2))
			if distance<midNearestDistance:
				midNearestDistance=distance
		
	return min(midNearestDistance,MDist)

if __name__=='__main__':	
	# 取30个点，x,y坐标范围都在1-100之间
	Points=[[random.randint(1,100),random.randint(1,100)] for one in range(30)]

	nearestPoint1(Points)
	print '*'*20
	Points=sorted(Points,key=lambda x:(x[0],x[1]),reverse=False)
	print nearestPoint2(Points)