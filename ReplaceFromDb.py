#!/usr/bin/env python3
import os
import sys
import fileinput
import glob

class dataBase:
    def __init__(self, fileName):
        filedata=open(fileName, 'r+')
        self.replacement={}

        record=False;
        content=''
        for line in filedata.readlines():
            if line.find('END_RECORD') >= 0:
                record=False;
                self.replacement[key]=content
                content=''
            if record:
                content+=line
            if line.find('START_RECORD') >= 0:
                args = line.split(' ')
                key = args[1];
                print( "found entrie ",key )
                record=True;

        filedata.close()

class htmlGeneretaor:
    def __init__(self, Masterdb):
        self.contentout=''
        self.Masterdb = Masterdb

    def filterInputFile(self, fileName):
        self.contentout=''
        filedata=open(fileName, 'r+')
        for line in filedata.readlines():
            self.contentout+=line
            for key in self.Masterdb.replacement.keys():
                token='<!--@'+key.rstrip('\n')+'-->'
                if line.find(token) >= 0:
                    print("in ",fileName," ? for ",token)
                    self.contentout+=self.Masterdb.replacement[key]
        filedata.close()

    def writeout(self, fileName):
        filedata=open(fileName, 'w+')
        filedata.write(self.contentout)
        filedata.close()

destination='' #tdb
source='' #tbd
folder='Base/'
src=source+folder+'*.html'

databaseFile=source+folder+'master.dat'
db = dataBase(databaseFile)

for inputfile in glob.glob(src):
    if not os.path.isfile(inputfile):
        continue
    filename = os.path.basename(inputfile)
    outputPath=destination+folder+filename
    print(inputfile,"--->",outputPath)
    rep = htmlGeneretaor(db)
    rep.filterInputFile(inputfile)
    rep.writeout(outputPath)
