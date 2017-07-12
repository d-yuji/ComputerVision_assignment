#!/usr/bin/python
# -*- coding: utf-8 -*-

# ディレクトリの作成プログラム

from PIL import Image
from pylab import *
import imtools
import cv2
import os
import pickle
import shutil
from scipy.cluster.vq import *

dataurl = "../../../imagedata/newdata/newdata/"
dirlist = os.listdir(dataurl)
output = "../dataset/"

for url in dirlist :
  print(url)
  if os.path.isdir(dataurl+url) :
    if os.path.exists(output+url) != True :
        os.mkdir(output+url)
