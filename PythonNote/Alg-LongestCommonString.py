# -*- coding: utf-8 -*-
# 题目：求两字符串的最初公共子序列

martix=[]

def computeLCS(strX,strY):
	global martix
	for time in range(len(strY)+1):
		martix.append([0]*(len(strX)+1))

	for oneStrIdx in range(len(strY)):
		for anotherStrIdx in range(len(strX)):
			if strY[oneStrIdx]==strX[anotherStrIdx]:
				martix[oneStrIdx+1][anotherStrIdx+1]=martix[oneStrIdx][anotherStrIdx]+1
			else:
				martix[oneStrIdx+1][anotherStrIdx+1]=max(martix[oneStrIdx+1][anotherStrIdx],martix[oneStrIdx][anotherStrIdx+1])


def getAllLCS(OneLCS,pointY,pointX):
	global martix
	tempLCS=[element for element in OneLCS]
	if martix[pointY][pointX]!=0:
		if strY[pointY-1]==strX[pointX-1]:
			tempLCS.append(strX[pointX-1])
			getAllLCS(tempLCS,pointY-1,pointX-1)
		else:
			if martix[pointY][pointX]==martix[pointY][pointX-1]:
				getAllLCS(tempLCS,pointY,pointX-1)
			if martix[pointY][pointX]==martix[pointY-1][pointX]:
				getAllLCS(tempLCS,pointY-1,pointX)
	else:
		OneLCS.reverse()
		print OneLCS

if __name__=='__main__':
	strX='abcbdab'
	strY='bdcaba'
	# strX='inthebegining'
	# strY='allthingsarelost'
	
	computeLCS(strX,strY)
	getAllLCS([],len(strY),len(strX))
	