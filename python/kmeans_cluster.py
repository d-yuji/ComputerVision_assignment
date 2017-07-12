#!/usr/bin/python
# -*- coding: utf-8 -*-

# 実践コンピュータビジョン 6.1.3 画像のクラスタリングを参考にしています
# https://www.oreilly.co.jp/pub/9784873116075/index.html

from PIL import Image
from pylab import *
import imtools
import cv2
import os
import pickle
from scipy.cluster.vq import *

# 画像のリストを得る
#imlist = imtools.get_imlist('selected_fontimages/')

dataurl = "../dataset/" # 前処理を行った画像データへのパス
dirlist = os.listdir(dataurl)
output = "../extractData/means/" # クラスタリングされた結果の平均画像を保存するパス 
pkl_path = "../extractData/pca/" # クラスタリングに使う主成分分析の結果へのパス

for url in dirlist :
  print(url)
  if os.path.isdir(dataurl+url) :
    if os.path.exists(output+url) != True :
        os.mkdir(output+url)

    imlist = imtools.get_imlist(dataurl+url)
    imnbr = len(imlist)

    # モデルのファイルを読み込む
    with open(pkl_path+url+"_pca.pkl",'rb') as f:
      immean = pickle.load(f)
      V = pickle.load(f)

    # 平板化した画像を格納する行列を作る
    immatrix = array([array(Image.open(im)).flatten() for im in imlist],'f')

    # 第40主成分までを射影する
    immean = immean.flatten()
    projected = array([dot(V[:40],immatrix[i]-immean) for i in range(imnbr)])

    # k平均法
    projected = whiten(projected)
    centroids,distortion = kmeans(projected,8)

    code,distance = vq(projected,centroids)

    # クラスタを描画する
    for k in range(8):
      ind = where(code==k)[0]
      figure()
      gray()
      size = 32,32
      meanimage = np.zeros(size, dtype=np.float32)
      meanimage.fill(255)
      for i in range(len(ind)):
        tempImage = immatrix[ind[i]].reshape((32,32))
        meanimage = cv2.addWeighted(meanimage, float(i)/len(ind), tempImage, 1.0/len(ind), 7.0)
      
      cv2.imwrite(output+url+"/"+"mean"+str(k)+".jpg",meanimage)
      '''
      for i in range(minimum(len(ind),40)):
        subplot(4,10,i+1)
        imshow(immatrix[ind[i]].reshape((32,32)))
        axis('off')
    show()
    '''



