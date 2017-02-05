from os import listdir
from os.path import isfile, join
import os
mypath = "./dataset"
onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]
print onlyfiles
for i in onlyfiles:
        print "./app ./dataset/" + i
        os.system("./app ./dataset/" + i)
