import os
	
def traverseDir(dirName):
	[traverseInDir(os.path.join(dirName,d)) for d in os.listdir(dirName) if os.path.isdir(os.path.join(dirName,d))]
	[traverseInFile(os.path.join(dirName,f)) for f in os.listdir(dirName) if os.path.isfile(os.path.join(dirName,f))]	
	
def traverseInDir(dirName):
	traverseDir(dirName)

def traverseInFile(fileName):
	print fileName+'\t'+str(os.path.getsize(fileName))

if __name__ == '__main__':
	dirName = 'D:\\Git\\GitWorkSpace'
	traverseDir(dirName)