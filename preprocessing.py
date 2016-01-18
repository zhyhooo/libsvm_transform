import sys



def sortByIndex(filename_in, filename_out):
    '''
        sort features of one sample by their indice.
    '''

    fin = open(filename_in, 'r')
    fout = open(filename_out, 'w')

    for line in fin:
        elements = line.strip().split()

        label = elements[0]
        sorted_features = sorted( elements[1:], key=lambda x: int(x.split(':')[0]) )
        print >>fout, label, ' '.join(sorted_features)
    fin.close()
    fout.close()
 

def filterByIndex(filename_in, filename_out, filename_index, flag='whitelist'):
    '''
        filter features by their indice,
        use a feature ID whilelist or blacklist depending on 'flag'.
    '''
    indice = set()
    findex = open(filename_index, 'r')
    for line in findex:
        indice.add( line.strip() )
    findex.close()

    fin = open(filename_in, 'r')
    fout = open(filename_out, 'w')

    if flag == 'whitelist' or flag == 'w':
        for line in fin:
            elements = line.strip().split()

            label = elements[0]
            features = [ x for x in elements[1:] if x.split(':')[0] in indice ]
            print >>fout, label, ' '.join(features)
    elif flag == 'blacklist' or flag == 'b':
        for line in fin:
            elements = line.strip().split()

            label = elements[0]
            features = [ x for x in elements[1:] if x.split(':')[0] not in indice ]
            print >>fout, label, ' '.join(features)
    else:
        print "flag should be either 'w' or 'b'. "
        
    fin.close()
    fout.close()


def replaceFeatureByIndex(filename_in, filename_out, findex, fvalue):
    '''
        replace feature of a specific index with another constant value.
    '''
    fin = open(filename_in, 'r')
    fout = open(filename_out, 'w')

    symbol = ' '+str(findex)+':'
    for line in fin:
        if symbol in line:
           idx1 = line.find(symbol)
           idx2 = line.find(' ', idx1+len(symbol))
           print >>fout, ( line[:idx1] + symbol + str(fvalue) + line[idx2:] ).strip()
        else:
            print >>fout, line.strip()

    fin.close()
    fout.close()

        

