#coding:utf-8
import sys
from collections import defaultdict
import os
from sys import stderr

'''
multiplication of two sparse matrix stored in libsvm format.
output is in the format of (row_id, col_id, value)
'''

def read_right():
    '''
    read the inital query/sku matrix
    '''
    for line in sys.stdin:
        elems = line.strip().split('\t')
        row_id = elems[0]
        tuples = [ (x.split(':')[0], x.split(':')[1]) for x in elems[1:] ]
        for tuple in tuples:
            print row_id+'\t'+'right'+'\002'+tuple[0]+'\002'+tuple[1]
             
def read_left():
    '''
    read the inital Q matrix
    '''
    for line in sys.stdin:
        elems = line.strip().split('\t')
        row_id = elems[0]
        tuples = [ (x.split(':')[0], x.split(':')[1]) for x in elems[1:] ]
        for tuple in tuples:
            print tuple[0]+'\t'+'left'+'\002'+row_id+'\002'+tuple[1]


def mapper1():
    '''
    mapreduce mapper in STEP 1
    in Z=M*N, Zij=Mix*Nxj
    usr x as key in shuffle
    '''
    file_path = os.environ["map_input_file"]
    if 'left' in file_path:
        read_left()
    elif 'right' in file_path:
        read_right()
    
def reducer1():
    '''
    mapreduce reducer in STEP 1
    for the same x,
    calculate Mix*Nxj for every i,j
    '''
    last_key = ""
    left_list, right_list = [],[]
    for line in sys.stdin:
        key, value=line.strip().split('\t')
        values = value.split('\002')
        if key == last_key:
            if values[0]=='left':
                left_list.append( (values[1],values[2]) )
            elif values[0]=='right':
                right_list.append( (values[1],values[2]) )
        else:
            for left in left_list:
                for right in right_list:
                    print left[0]+'\002'+right[0] +'\t'+ str(float(left[1]) * float(right[1]))
            if values[0]=='left':
                left_list, right_list = [(values[1],values[2])], []
            elif values[0]=='right':
                left_list, right_list = [], [(values[1],values[2])]
            last_key = key 
    if key==last_key:
        for left in left_list:
            for right in right_list:
                print left[0]+'\002'+right[0] +'\t'+ str(float(left[1]) * float(right[1]))

def mapper2():
    '''
    mapreduce mapper in STEP2
    '''
    for line in sys.stdin:
        print line.strip()

            
def reducer2():
    '''
    mapreduce reducer in STEP2
    sum the result of M*N
    '''
    last_key, sum = "", 0.0
    for line in sys.stdin:
        key, value = line.strip().split('\t')
        if key == last_key:
            sum += float(value)
        else:
            if sum > 0.0:
                print last_key +'\t'+ str(sum)
            last_key, sum = key, float(value)
    if key == last_key and sum > 0.0:
        print key +'\t'+ str(sum)
        

if __name__ == "__main__":
    try:
	    if sys.argv[1]=='mapper1':
	        mapper1()
	    elif sys.argv[1]=='reducer1':
	        reducer1()
	    elif sys.argv[1]=='mapper2':
	        mapper2()
	    elif sys.argv[1]=='reducer2':
	        reducer2()
    except Exception, e:
        exstr = traceback.format_exc()
        print exstr
