# Author: Marco Simoes
# Adapted from Java's implementation of Rui Pedro Paiva
# Teoria da Informacao, LEI, 2022

import sys
from huffmantree import HuffmanTree
import numpy as np


class GZIPHeader:
    ''' class for reading and storing GZIP header fields '''

    ID1 = ID2 = CM = FLG = XFL = OS = 0
    MTIME = []
    lenMTIME = 4
    mTime = 0

    # bits 0, 1, 2, 3 and 4, respectively (remaining 3 bits: reserved)
    FLG_FTEXT = FLG_FHCRC = FLG_FEXTRA = FLG_FNAME = FLG_FCOMMENT = 0   
    
    # FLG_FTEXT --> ignored (usually 0)
    # if FLG_FEXTRA == 1
    XLEN, extraField = [], []
    lenXLEN = 2
    
    # if FLG_FNAME == 1
    fName = ''  # ends when a byte with value 0 is read
    
    # if FLG_FCOMMENT == 1
    fComment = ''   # ends when a byte with value 0 is read
        
    # if FLG_HCRC == 1
    HCRC = []
        
        
    
    def read(self, f):
        ''' reads and processes the Huffman header from file. Returns 0 if no error, -1 otherwise '''

        # ID 1 and 2: fixed values
        self.ID1 = f.read(1)[0]  
        if self.ID1 != 0x1f: return -1 # error in the header
            
        self.ID2 = f.read(1)[0]
        if self.ID2 != 0x8b: return -1 # error in the header
        
        # CM - Compression Method: must be the value 8 for deflate
        self.CM = f.read(1)[0]
        if self.CM != 0x08: return -1 # error in the header
                    
        # Flags
        self.FLG = f.read(1)[0]
        
        # MTIME
        self.MTIME = [0]*self.lenMTIME
        self.mTime = 0
        for i in range(self.lenMTIME):
            self.MTIME[i] = f.read(1)[0]
            self.mTime += self.MTIME[i] << (8 * i)                 
                        
        # XFL (not processed...)
        self.XFL = f.read(1)[0]
        
        # OS (not processed...)
        self.OS = f.read(1)[0]
        
        # --- Check Flags
        self.FLG_FTEXT = self.FLG & 0x01
        self.FLG_FHCRC = (self.FLG & 0x02) >> 1
        self.FLG_FEXTRA = (self.FLG & 0x04) >> 2
        self.FLG_FNAME = (self.FLG & 0x08) >> 3
        self.FLG_FCOMMENT = (self.FLG & 0x10) >> 4
                    
        # FLG_EXTRA
        if self.FLG_FEXTRA == 1:
            # read 2 bytes XLEN + XLEN bytes de extra field
            # 1st byte: LSB, 2nd: MSB
            self.XLEN = [0]*self.lenXLEN
            self.XLEN[0] = f.read(1)[0]
            self.XLEN[1] = f.read(1)[0]
            self.xlen = self.XLEN[1] << 8 + self.XLEN[0]
            
            # read extraField and ignore its values
            self.extraField = f.read(self.xlen)
        
        def read_str_until_0(f):
            s = ''
            while True:
                c = f.read(1)[0]
                if c == 0: 
                    return s
                s += chr(c)
        
        # FLG_FNAME
        if self.FLG_FNAME == 1:
            self.fName = read_str_until_0(f)
        
        # FLG_FCOMMENT
        if self.FLG_FCOMMENT == 1:
            self.fComment = read_str_until_0(f)
        
        # FLG_FHCRC (not processed...)
        if self.FLG_FHCRC == 1:
            self.HCRC = f.read(2)
            
        return 0
            



class GZIP:
    ''' class for GZIP decompressing file (if compressed with deflate) '''

    gzh = None
    gzFile = ''
    fileSize = origFileSize = -1
    numBlocks = 0
    f = None
    

    bits_buffer = 0
    available_bits = 0        

    
    def __init__(self, filename):
        self.gzFile = filename
        self.f = open(filename, 'rb')
        self.f.seek(0,2)
        self.fileSize = self.f.tell()
        self.f.seek(0)

        
    def comps(self,HCLEN):
        array = []
        array = np.zeros(19)
        array = array.astype("uint16")
        ordem = [16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15]
        for i in range(HCLEN+4):
            array[ordem[i]] = self.readBits(3)
        print(array)
        return array
    
    def count(self,comp,n):
        count = np.zeros(n)
        count = count.astype("uint16")
        for i in range(len(count)):
            for j in range(len(comp)):
                if comp[j] == i:
                    count[i]+=1
        print(count)
        return count
    
    def huffman(self,comp,count,n):
        cod = 0
        count[0] = 0
        huffman_array = np.zeros(n)
        huffman_array = huffman_array.astype("uint16")
        for i in range(1,len(count)):
            cod += count[i-1]
            if(i>2):
                cod = np.left_shift(cod,1)
                huffman_array[i] = cod
        print(huffman_array)
        hft = HuffmanTree()
        i = 0
        for j in comp:
            if(j!=0):
                huffmancode = (format(huffman_array[j],'b')).zfill(j)
                hft.addNode(huffmancode,i,True) 
                huffman_array[j]+=1
            i+=1
        return hft
    def search(self,valor,hft):
        tam = np.zeros(valor)
        tam = tam.astype("uint16")
        terminate = False
        i = 0
        
        while(i<valor):
            while not terminate:
                nextBit = str(self.readBits(1))
            
                pos = hft.nextNode(nextBit)
                        
                if pos != -2:
                    terminate = True
            
            if(pos < 16):
                tam[i] = pos
                i+=1
            if(pos==16):
                n = 3 + self.readBits(2)
                rep = tam[i-1]
                for k in range(n):
                    tam[i] = rep
                    i+=1
            if(pos==17):
                n = 3 + self.readBits(3)
                for k in range(n):
                    tam[i] = 0
                    i+=1
            if(pos==18):
                n = 11 + self.readBits(7)
                for k in range(n):
                    tam[i] = 0
                    i+=1
            
            terminate = False
            hft.resetCurNode()
        return tam

    def calcula_bits(self,start_pos,i,pos):
        cur_pos = start_pos
        bits=0 
        count = 0 
        length = 0 
        while(cur_pos!=pos):
            cur_pos+=1 
            length += (2**bits)
            count+=1 
            if(bits == 0 and count >=i*2)or(bits!=0 and count >=i):
                bits+=1 
                count = 0 
        if(bits>0):
            length += self.readBits(bits)
        return length
    def descomprimir(self,hft_HLIT, hft_DIST):
        array = np.array([])
        array = array.astype("uint16")
        terminate_LIT = False
        terminate_DIST = False
        notFinished = True
        i=0
        length=0
        while (notFinished):
            while not terminate_LIT:
                nextBit = str(self.readBits(1))
                pos = hft_HLIT.nextNode(nextBit)
                if pos != -2:
                    terminate_LIT = True
                    if (pos<256):
                        array = np.append(array,[pos],axis = 0)
                        i+= 1
                    if (pos==256):
                        notFinished=False
                    if (pos>256):
                        length = 3 + self.calcula_bits(257,4,pos)
                        while not terminate_DIST:
                            nextBit = str(self.readBits(1))
                            next_pos = hft_DIST.nextNode(nextBit) 
                            if next_pos != -2:
                                terminate_DIST = True
                        hft_DIST.resetCurNode() 
                        terminate_DIST = False
                        distance = 1 + self.calcula_bits(0,2,next_pos)
                        for i in range(length):
                            array = np.append(array,[array[-distance]],axis=0)
            terminate_LIT = False 
            hft_HLIT.resetCurNode()
        return array  
          
    
        
    def decompress(self):
        ''' main function for decompressing the gzip file with deflate algorithm '''
        
        numBlocks = 0

        # get original file size: size of file before compression
        origFileSize = self.getOrigFileSize()
        print(origFileSize)
        
        # read GZIP header
        error = self.getHeader()
        if error != 0:
            print('Formato invalido!')
            return
        
        # show filename read from GZIP header
        print(self.gzh.fName)
        
        
        # MAIN LOOP - decode block by block
        BFINAL = 0    
        while not BFINAL == 1:    
            
            BFINAL = self.readBits(1)
                            
            BTYPE = self.readBits(2)        
            
            if BTYPE != 2:
                print('Error: Block %d not coded with Huffman Dynamic coding' % (numBlocks+1))
                return

            
                                
            #--- STUDENTS --- ADD CODE HERE
            
            HLIT = self.readBits(5)
            HDIST = self.readBits(5)
            HCLEN = self.readBits(4)
            
            print(HLIT)
            print(HDIST)
            print(HCLEN)
            
            comp = []
            comp = gz.comps(HCLEN)
            bl_count = gz.count(comp,6)
            
            hft = gz.huffman(comp,bl_count,len(bl_count))
            
            lit = gz.search(HLIT + 257,hft)
            lit_count =gz.count(lit,11)
            print(lit)
            
            dist = gz.search(HDIST +1,hft)
            dist_count = gz.count(dist,10)
            print(dist)
            
            hft_HLIT = gz.huffman(lit, lit_count,len(lit_count))
            hft_HDIST = gz.huffman(dist, dist_count,len(dist_count))
            
            #ex7 e 8
            char_array = np.char.mod('%c', gz.descomprimir(hft_HLIT,hft_HDIST)).tolist()
            string = ''
            for i in char_array:
                string += i
            print(string)
            with open(self.gzh.fName,'w') as f:
                f.write(string)  
              
            numBlocks += 1

            # update number of blocks read
        
        # close file            
        
        self.f.close()    
        print("End: %d block(s) analyzed." % numBlocks)
        return HCLEN


    
    def getOrigFileSize(self):
        ''' reads file size of original file (before compression) - ISIZE '''
        
        # saves current position of file pointer
        fp = self.f.tell()
        
        # jumps to end-4 position
        self.f.seek(self.fileSize-4)
        
        # reads the last 4 bytes (LITTLE ENDIAN)
        sz = 0
        for i in range(4): 
            sz += self.f.read(1)[0] << (8*i)
        
        # restores file pointer to its original position
        self.f.seek(fp)
        
        return sz        
    

    
    def getHeader(self):  
        ''' reads GZIP header'''

        self.gzh = GZIPHeader()
        header_error = self.gzh.read(self.f)
        return header_error
        

    def readBits(self, n, keep=False):
        ''' reads n bits from bits_buffer. if keep = True, leaves bits in the buffer for future accesses '''

        while n > self.available_bits:
            self.bits_buffer = self.f.read(1)[0] << self.available_bits | self.bits_buffer
            self.available_bits += 8
        
        mask = (2**n)-1
        value = self.bits_buffer & mask

        if not keep:
            self.bits_buffer >>= n
            self.available_bits -= n

        return value

    


if __name__ == '__main__':

    # gets filename from command line if provided
    fileName = "FAQ.txt.gz"
    if len(sys.argv) > 1:
        fileName = sys.argv[1]            

    # decompress file
    gz = GZIP(fileName)
    gz.decompress()
  