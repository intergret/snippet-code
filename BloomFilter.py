#coding:utf-8
from BitVector import BitVector 
from random import Random 
from GeneralHashFunctions import *


class BloomFilter(object): 
    def __init__(self, n=None, m=None, k=None, p=None, bits=None ): 
        self.m = m 
        if k > 4 or k < 1: 
                raise Exception('Must specify value of k between 1 and 4') 
        self.k = k 
        if bits: 
                self.bits = bits 
        else: 
                self.bits = BitVector( size=m ) 
        self.rand = Random() 
        self.hashes = [] 
        self.hashes.append(RSHash) 
        self.hashes.append(JSHash) 
        self.hashes.append(PJWHash) 
        self.hashes.append(DJBHash) 

        # switch between hashing techniques 
        self._indexes = self._rand_indexes 
        #self._indexes = self._hash_indexes 

    def __contains__(self, key): 
        for i in self._indexes(key): 
                if not self.bits[i]: 
                        return False     
        return True 

    def add(self, key): 
        dupe = True 
        bits = [] 
        for i in self._indexes(key): 
                if dupe and not self.bits[i]: 
                        dupe = False 
                self.bits[i] = 1 
                bits.append(i) 
        return dupe 

    def __and__(self, filter): 
        if (self.k != filter.k) or (self.m != filter.m): 
                raise Exception('Must use bloom filters created with equal k / m paramters for bitwise AND') 
        return BloomFilter(m=self.m,k=self.k,bits=(self.bits & filter.bits)) 

    def __or__(self, filter): 
        if (self.k != filter.k) or (self.m != filter.m): 
                raise Exception('Must use bloom filters created with equal k / m paramters for bitwise OR') 
        return BloomFilter(m=self.m,k=self.k,bits=(self.bits | filter.bits)) 

    def _hash_indexes(self,key): 
        ret = [] 
        for i in range(self.k): 
                ret.append(self.hashes[i](key) % self.m) 
        return ret 

    def _rand_indexes(self,key): 
        self.rand.seed(hash(key)) 
        ret = [] 
        for i in range(self.k): 
                ret.append(self.rand.randint(0,self.m-1)) 
        return ret 

if __name__ == '__main__': 
    e = BloomFilter(m=100, k=4) 
    e.add('one') 
    e.add('two') 
    e.add('three') 
    e.add('four') 
    e.add('five') 
    e.add('ÄãºÃ')        

    f = BloomFilter(m=100, k=4) 
    f.add('three') 
    f.add('four') 
    f.add('five') 
    f.add('six') 
    f.add('seven') 
    f.add('eight') 
    f.add('nine') 
    f.add("ten")         

    # test check for dupe on add 
    assert not f.add('eleven') 
    assert f.add('eleven') 

    # test membership operations 
    assert 'ten' in f 
    assert 'one' in e 
    assert 'ÄãºÃ' in e
    assert 'ten' not in e 
    assert 'one' not in f          

    # test set based operations 
    union = f | e 
    intersection = f & e 

    assert 'ten' in union 
    assert 'one' in union 
    assert 'three' in intersection 
    assert 'ten' not in intersection 
    assert 'one' not in intersection