import os, stat
from six.moves import urllib
import zipfile
import csv

DATA_ROOT = "http://archive.ics.uci.edu/"
DATA_PATH = "ml/machine-learning-databases/00360/"
DATA_FILE = "AirQualityUCI.zip"
LOCAL_PATH = os.path.join(os.getcwd(),"Data")

class Download:
    def __init__(self, root, path, file):
        self.root = root
        self.path = path
        self.file = file
        self.URL = self.root + self.path + self.file
        print("Init    \t", self.URL)

    def SetURL(self, root, path, file):
        self.root = root
        self.path = path
        self.file = file
        self.URL = self.root + self.path + self.file
        print("URL     \t", self.URL)
        
    def GetURL(self):
        return self.URL

    def SetRoot(self, root):
        self.root = root
        self.URL = self.root + self.path + self.file
        print("Root    \t", self.root)

    def SetPath(self, path):
        self.path = path
        self.URL = self.root + self.path + self.file
        print("Path    \t", self.path)

    def SetFile(self, file):
        self.file = file
        self.URL = self.root + self.path + self.file
        print("File    \t", self.file)

    def SetDir(self, loc):
        self.loc = loc
        print("Dir     \t", self.loc)

    def GetDir(self):
        return self.loc

    def Download(self):
        if (not os.path.isdir(self.loc)):
            print("Makedir \t", self.loc)
            os.makedirs(self.loc, mode=0o777)
        else:
            print("Exists  \t", self.loc)
            
        if not os.path.isfile(self.loc + '\\' + self.file):
            print("Download\t", self.file)
            urllib.request.urlretrieve(self.URL, os.path.join(self.loc,self.file))
            os.path.join(os.getcwd(),"Data")
        else:
            print("Exists  \t", self.file)


class Unzip:
    def __init__(self, loc, file):
        self.loc = loc
        self.file = file
        self.dest = os.path.join(self.loc,self.file)
        print("Init    \t", self.dest)

    def SetDest(self, loc, file):
        self.loc = loc
        self.file = file
        self.dest = os.path.join(self.loc,self.file)
        print("Dest    \t", self.dest)
        
    def GetDest(self):
        return self.Dest

    def SetDir(self, loc):
        self.loc = loc
        self.dest = os.path.join(self.loc,self.file)
        print("Dir     \t", self.loc)

    def GetDir(self):
        return self.loc

    def SetFile(self, file):
        self.file = file
        self.dest = os.path.join(self.loc,self.file)
        print("File    \t", self.file)

    def Unzip(self):
        self.type = "zip"
        if (self.type == "zip"):
            print("Type    \t .zip")
            zip = zipfile.ZipFile(os.path.join(self.loc,self.file))
            zip.extractall(self.loc)
            print("Unzip   \t" , os.path.join(self.loc,self.file))
        elif (self.type == "tar"):
            print("Type    \t .tar")
        elif (self.type == "cvs"):
            print("Type    \t .cvs")
    def List(self):
        print("List    \t")
        for x in os.listdir(self.loc):
            print("        	",x)

class Parse:
    def __init__(self, loc, file):
        self.loc = loc
        self.file = file
        self.dest = os.path.join(self.loc,self.file)
        print("Init    \t", self.dest)
        
    def SetFile(self, file):
        self.file = file
        self.dest = os.path.join(self.loc,self.file)
        print("File    \t", self.file)

    def Parse(self):
        print("Parse   \t", self.dest)
        with open(os.path.join(self.loc,self.file), mode='r') as self.infile:
            self.d = dict(filter(None, csv.reader(self.infile)))
        print(self.d)
       
d = Download(DATA_ROOT, DATA_PATH, DATA_FILE)
d.SetFile(DATA_FILE)
d.SetPath(DATA_PATH)
d.SetRoot(DATA_ROOT)
d.SetDir(LOCAL_PATH)
d.Download()
u = Unzip(LOCAL_PATH,DATA_FILE)
u.SetDir(d.GetDir())
u.SetFile(DATA_FILE)
u.Unzip()
u.List()
f= input("# Please input a file name: ")
p = Parse(u.GetDir(), f)
p.SetFile(f)
p.Parse()





