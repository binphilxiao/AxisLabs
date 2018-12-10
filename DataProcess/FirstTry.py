import os, stat
from six.moves import urllib
import zipfile

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

    def SetLocalPath(self, local):
        self.local = local
        print("Dir     \t", self.local)

    def Download(self):
        if (not os.path.isdir(self.local)):
            print("Makedir \t", self.local)
            os.makedirs(self.local, mode=0o777)
        else:
            print("Exists  \t" + self.local)
            
        if not os.path.isfile(self.local + '\\' + self.file):
            print("Download\t", self.file)
            urllib.request.urlretrieve(self.URL, os.path.join(self.local,self.file))
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
       
d = Download(DATA_ROOT, DATA_PATH, DATA_FILE)
d.SetFile(DATA_FILE)
d.SetPath(DATA_PATH)
d.SetRoot(DATA_ROOT)
d.SetLocalPath(LOCAL_PATH)
d.Download()
p = Unzip(LOCAL_PATH,DATA_FILE)
p.SetDir(LOCAL_PATH)
p.SetFile(DATA_FILE)
p.Unzip()
p.List()


