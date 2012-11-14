import os

def findstrToFind(dirName,strToFind):
	[findstrToFindInDir(os.path.join(dirName, f),strToFind) for f in os.listdir(dirName) if os.path.isdir(os.path.join(dirName, f))]
	[findstrToFindInFile(os.path.join(dirName, f),strToFind) for f in os.listdir(dirName) if os.path.isfile(os.path.join(dirName, f))]
		
def findstrToFindInDir(dirName,strToFind):
	findstrToFind(dirName,strToFind)

def findstrToFindInFile(dirName,strToFind):
	for eachline in open(dirName,'r'):
		if eachline.count(strToFind)>0:
			print dirName+':'+eachline,

if __name__ == '__main__':
	dirName = 'D:\\Git\\GitWorkSpace'
	strToFind = 'hello'
	findstrToFind(dirName,strToFind)
	