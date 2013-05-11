#encoding=utf-8
import time
from collections import defaultdict
from operator import itemgetter

# Data in BC.txt:
# a	b
# a	h
# b	c
# b	h
# h	i
# h	g
# g	i
# g	f
# c	f
# c	i
# c	d
# d	f
# d	e
# f	e

class Graph:
	def __init__(self):
		self.Graph = defaultdict(set)
		self.NodesNum = 0
	
	def MakeLink(self,filename,separator):
		with open(filename,'r') as graphfile:
			for line in graphfile:
				nodeA,nodeB = line.strip().split(separator)
				self.Graph[nodeA].add(nodeB)
				self.Graph[nodeB].add(nodeA)
		self.NodesNum = len(self.Graph)
		
	def BetweennessCentrality(self):
		betweenness = dict.fromkeys(self.Graph,0.0)
		for s in self.Graph:
			# 1. compute the length and number of shortest paths from node s
			S = []
			P = {}
			for v in self.Graph:
				P[v]=[]
			Sigma = dict.fromkeys(self.Graph,0.0)
			Sigma[s] = 1.0
			D = {}
			D[s] = 0
			Q = [s]
			# use BFS to find single source shortest paths
			while Q:
				v = Q.pop(0)
				S.append(v)
				Dv = D[v]
				for w in self.Graph[v]:
					# w found for the first time?
					if w not in D:
						Q.append(w)
						D[w] = D[v] + 1
					# shortest path to w via v
					if D[w] == D[v] + 1:
						Sigma[w] += Sigma[v]
						P[w].append(v)
			
			# 2. sum all pair-dependencies of node s
			delta = dict.fromkeys(self.Graph,0.0)
			# S returns vertices in order of non-increasing distance from s
			while S:
				w = S.pop()
				coeff = (1.0+delta[w])/Sigma[w]
				for v in P[w]:
					delta[v] += Sigma[v]*coeff
				if w != s:
					betweenness[w] += delta[w]
		
		scale = 1.0/((self.NodesNum-1)*(self.NodesNum-2))
		for v in betweenness:
			betweenness[v] *= scale
			
		betweenness = [(node,bc) for node,bc in betweenness.iteritems()]
		betweenness = sorted(betweenness,key=itemgetter(1),reverse=True)
		return betweenness

if __name__=='__main__':
	separator = '\t'
	file = 'C:\\Users\\Administrator\\Desktop\\BC.txt'
	
	begin = time.time()
	myGraph = Graph()
	myGraph.MakeLink(file,separator)
	print myGraph.BetweennessCentrality()
	
	print 'Time:',time.time()-begin,' seconds'