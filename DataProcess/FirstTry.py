import os, stat
from six.moves import urllib

DATA_ROOT = "http://archive.ics.uci.edu/"
DATA_PATH = "ml/machine-learning-databases/00360/"
DATA_FILE = "AirQualityUCI.zip"
LOCAL_PATH = os.path.join(os.getcwd(),"Data")

class DataSet:
    def __init__(self, root, path, file):
        self.root = root
        self.path = path
        self.file = file
        self.URL = self.root + self.path + self.file
        print(self.URL)
        
    def GetURL(self):
        return self.URL

    def SetRoot(self, root):
        self.root = root
        self.URL = self.root + self.path + self.file
        print(self.URL)

    def SetPath(self, path):
        self.path = path
        self.URL = self.root + self.path + self.file
        print(self.URL)

    def SetFile(self, file):
        self.file = file
        self.URL = self.root + self.path + self.file
        print(self.URL)

    def SetURL(self, root, path, file):
        self.root = root
        self.path = path
        self.file = file
        self.URL = self.root + self.path + self.file
        print(self.URL)

    def SetLocalPath(self, local):
        self.local = local
        print(self.local)

    def IsAvailable(self):
        return 0

    def Download(self):
        os.mkdir(self.local)
        urllib.request.urlretrieve(self.URL, self.local)
        
       
d = DataSet(DATA_ROOT, DATA_PATH, DATA_FILE)
d.SetFile(DATA_FILE)
d.SetPath(DATA_PATH)
d.SetRoot(DATA_ROOT)
d.SetLocalPath(LOCAL_PATH)
d.Download()


