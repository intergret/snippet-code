#encoding=utf-8
import networkx,heapq,sys
from matplotlib import pyplot
from collections import defaultdict,OrderedDict
from numpy  import array

# Data in graphdata.txt:
# a	b	4
# a	h	8
# b	c	8
# b	h	11
# h	i	7
# h	g	1
# g	i	6
# g	f	2
# c	f	4
# c	i	2
# c	d	7
# d	f	14
# d	e	9
# f	e	10

def Edge(): return defaultdict(Edge)

class Graph:
	def __init__(self):
		self.Link = Edge()
		self.FileName = ''
		self.Separator = ''
	
	def MakeLink(self,filename,separator):
		self.FileName = filename
		self.Separator = separator
		graphfile = open(filename,'r')
		for line in graphfile:
			items = line.split(separator)
			self.Link[items[0]][items[1]] = int(items[2])
			self.Link[items[1]][items[0]] = int(items[2])
		graphfile.close()
		
	def LocalClusteringCoefficient(self,node):
		neighbors = self.Link[node]
		if len(neighbors) <= 1: return 0
		links = 0
		for j in neighbors:
			for k in neighbors:
				if j in self.Link[k]:
					links += 0.5
		return 2.0*links/(len(neighbors)*(len(neighbors)-1))
	
	def AverageClusteringCoefficient(self):
		total = 0.0
		for node in self.Link.keys():
			total += self.LocalClusteringCoefficient(node)
		return total/len(self.Link.keys())
		
	def DeepFirstSearch(self,start):
		visitedNodes = []
		todoList = [start]
		while todoList:
			visit = todoList.pop(0)
			if visit not in visitedNodes:
				visitedNodes.append(visit)
				todoList = self.Link[visit].keys() + todoList
		return visitedNodes
		
	def BreadthFirstSearch(self,start):
		visitedNodes = []
		todoList = [start]
		while todoList:
			visit = todoList.pop(0)
			if visit not in visitedNodes:
				visitedNodes.append(visit)
				todoList = todoList + self.Link[visit].keys()
		return visitedNodes
		
	def ListAllComponent(self):
		allComponent = []
		visited = {}
		for node in self.Link.iterkeys():
			if node not in visited:
				oneComponent = self.MakeComponent(node,visited)
				allComponent.append(oneComponent)
		return allComponent
	
	def CheckConnection(self,node1,node2):
		return True if node2 in self.MakeComponent(node1,{}) else False
	
	def MakeComponent(self,node,visited):
		visited[node] = True
		component = [node]
		for neighbor in self.Link[node]:
			if neighbor not in visited:
				component += self.MakeComponent(neighbor,visited)
		return component
			
	def MinimumSpanningTree_Kruskal(self,start):
		graphEdges = [line.strip('\n').split(self.Separator) for line in open(self.FileName,'r')]
		
		nodeSet = {}
		for idx,node in enumerate(self.MakeComponent(start,{})):
			nodeSet[node] = idx
		
		edgeNumber = 0; totalEdgeNumber = len(nodeSet)-1
		for oneEdge in sorted(graphEdges,key=lambda x:int(x[2]),reverse=False):
			if edgeNumber == totalEdgeNumber: break
			nodeA,nodeB,cost = oneEdge
			if nodeA in nodeSet and nodeSet[nodeA] != nodeSet[nodeB]:
				nodeBSet = nodeSet[nodeB]
				for node in nodeSet.keys():
					if nodeSet[node] == nodeBSet:
						nodeSet[node] = nodeSet[nodeA]
				print nodeA,nodeB,cost
				edgeNumber += 1
	
	def MinimumSpanningTree_Prim(self,start):
		expandNode = set(self.MakeComponent(start,{}))
		distFromTreeSoFar = {}.fromkeys(expandNode,sys.maxint); distFromTreeSoFar[start] = 0
		linkToNode = {}.fromkeys(expandNode,'');linkToNode[start] = start
		
		while expandNode:
			# Find the closest dist node
			closestNode = ''; shortestdistance = sys.maxint;
			for node,dist in distFromTreeSoFar.iteritems():
				if node in expandNode and dist < shortestdistance:
					closestNode,shortestdistance = node,dist
			expandNode.remove(closestNode)
			
			print linkToNode[closestNode],closestNode,shortestdistance
			
			for neighbor in self.Link[closestNode].iterkeys():
				recomputedist = self.Link[closestNode][neighbor]
				if recomputedist < distFromTreeSoFar[neighbor]:
					distFromTreeSoFar[neighbor] = recomputedist
					linkToNode[neighbor] = closestNode

	def ShortestPathOne2One(self,start,end):
		pathFromStart = {}
		pathFromStart[start] = [start]
		todoList = [start]
		while todoList:
			current = todoList.pop(0)
			for neighbor in self.Link[current]:
				if neighbor not in pathFromStart:
					pathFromStart[neighbor] = pathFromStart[current] + [neighbor]
					if neighbor == end:
						return pathFromStart[end]
					todoList.append(neighbor)
		return []
	
	def Centrality(self,node):
		path2All = self.ShortestPathOne2All(node)
		# The average of the distances of all the reachable nodes
		return float(sum([len(path)-1 for path in path2All.itervalues()]))/len(path2All)
	
	def SingleSourceShortestPath_Dijkstra(self,start):
		expandNode = set(self.MakeComponent(start,{}))
		distFromSourceSoFar = {}.fromkeys(expandNode,sys.maxint); distFromSourceSoFar[start] = 0
		
		while expandNode:
			# Find the closest dist node
			closestNode = ''; shortestdistance = sys.maxint;
			for node,dist in distFromSourceSoFar.iteritems():
				if node in expandNode and dist < shortestdistance:
					closestNode,shortestdistance = node,dist
			expandNode.remove(closestNode)
			
			for neighbor in self.Link[closestNode].iterkeys():
				recomputedist = distFromSourceSoFar[closestNode] + self.Link[closestNode][neighbor]
				if recomputedist < distFromSourceSoFar[neighbor]:
					distFromSourceSoFar[neighbor] = recomputedist
		
		for node in distFromSourceSoFar:
			print start,node,distFromSourceSoFar[node]
	
	def AllpairsShortestPaths_MatrixMultiplication(self,start):
		nodeIdx = {}; idxNode = {}; 
		for idx,node in enumerate(self.MakeComponent(start,{})):
			nodeIdx[node] = idx; idxNode[idx] = node
		matrixSize = len(nodeIdx)
		
		MaxInt = 1000
		nodeMatrix = array([[MaxInt]*matrixSize]*matrixSize)
		for node in nodeIdx.iterkeys():
			nodeMatrix[nodeIdx[node]][nodeIdx[node]] = 0
		for line in open(self.FileName,'r'):
			nodeA,nodeB,cost = line.strip('\n').split(self.Separator)
			if nodeA in nodeIdx:
				nodeMatrix[nodeIdx[nodeA]][nodeIdx[nodeB]] = int(cost)
				nodeMatrix[nodeIdx[nodeB]][nodeIdx[nodeA]] = int(cost)
			
		result = array([[0]*matrixSize]*matrixSize)
		for i in xrange(matrixSize):
			for j in xrange(matrixSize):
				result[i][j] = nodeMatrix[i][j]
		
		for itertime in xrange(2,matrixSize):
			for i in xrange(matrixSize):
				for j in xrange(matrixSize):
					if i==j:
						result[i][j] = 0
						continue
					result[i][j] = MaxInt
					for k in xrange(matrixSize):
						result[i][j] = min(result[i][j],result[i][k]+nodeMatrix[k][j])
		
		for i in xrange(matrixSize):
			for j in xrange(matrixSize):
				if result[i][j] != MaxInt:
					print idxNode[i],idxNode[j],result[i][j]
		
	def ShortestPathOne2All(self,start):
		pathFromStart = {}
		pathFromStart[start] = [start]
		todoList = [start]
		while todoList:
			current = todoList.pop(0)
			for neighbor in self.Link[current]:
				if neighbor not in pathFromStart:
					pathFromStart[neighbor] = pathFromStart[current] + [neighbor]
					todoList.append(neighbor)
		return pathFromStart
	
	def NDegreeNode(self,start,n):
		pathFromStart = {}
		pathFromStart[start] = [start]
		pathLenFromStart = {}
		pathLenFromStart[start] = 0
		todoList = [start]
		while todoList:
			current = todoList.pop(0)
			for neighbor in self.Link[current]:
				if neighbor not in pathFromStart:
					pathFromStart[neighbor] = pathFromStart[current] + [neighbor]
					pathLenFromStart[neighbor] = pathLenFromStart[current] + 1
					if pathLenFromStart[neighbor] <= n+1:
						todoList.append(neighbor)
		
		for node in pathFromStart.keys():
			if len(pathFromStart[node]) != n+1:
				del pathFromStart[node]
		return pathFromStart
		
	def Draw(self):
		G = networkx.Graph()
		nodes = self.Link.keys()
		edges = [(node,neighbor) for node in nodes for neighbor in self.Link[node]]
		G.add_edges_from(edges)
		networkx.draw(G)
		pyplot.show()
		

if __name__=='__main__':
	separator = '\t'
	filename = 'C:\\Users\\Administrator\\Desktop\\graphdata.txt'
	resultfilename = 'C:\\Users\\Administrator\\Desktop\\result.txt'
	
	myGraph = Graph()
	myGraph.MakeLink(filename,separator)
	
	print 'LocalClusteringCoefficient',myGraph.LocalClusteringCoefficient('a')
	print 'AverageClusteringCoefficient',myGraph.AverageClusteringCoefficient()
	print 'DeepFirstSearch',myGraph.DeepFirstSearch('a')
	print 'BreadthFirstSearch',myGraph.BreadthFirstSearch('a')
	print 'ShortestPathOne2One',myGraph.ShortestPathOne2One('a','d')
	print 'ShortestPathOne2All',myGraph.ShortestPathOne2All('a')
	print 'NDegreeNode',myGraph.NDegreeNode('a',3).keys()
	print 'ListAllComponent',myGraph.ListAllComponent()
	print 'CheckConnection',myGraph.CheckConnection('a','f')
	print 'Centrality',myGraph.Centrality('c')
	
	myGraph.MinimumSpanningTree_Kruskal('a')
	myGraph.AllpairsShortestPaths_MatrixMultiplication('a')
	myGraph.MinimumSpanningTree_Prim('a')
	myGraph.SingleSourceShortestPath_Dijkstra('a')
	# myGraph.Draw()
	
	