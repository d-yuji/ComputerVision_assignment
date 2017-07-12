#!/usr/bin/python
# -*- coding: utf-8 -*-

# 実践コンピュータビジョン 1.3.6 主成分を参考にしています
# https://www.oreilly.co.jp/pub/9784873116075/index.html


from PIL import Image
from numpy import *
from pylab import *
import os
import pca
import imtools

dataurl = "../dataset/" #主成分析分をする画像データへのパス
dirlist = os.listdir(dataurl)
output = "../extractData/pca/" # 主成分分析の結果を保存するパス

for url in dirlist:
	print(url)
	if os.path.isdir(dataurl+url) : #ディレクトリが存在したら
		imlist = imtools.get_imlist(dataurl+url) # 画像を取得

		im = array(Image.open(imlist[0])) # サイズを得るため画像を1つ開く
		m,n = im.shape[0:2] # 画像のサイズを得る
		imnbr = len(imlist) # 画像数の得る

		# すべての平板化画像を格納する行列を作る
		immatrix = array([array(Image.open(im)).flatten() for im in imlist],'f') 

		# 主成分分析を実行する
		V,S,immean = pca.pca(immatrix) 

		# 画像を表示する（平均と、最初の7つの主成分）
		"""
		figure() 
		gray() 
		subplot(2,4,1) 
		imshow(immean.reshape(m,n)) 
		for i in range(7):
		  subplot(2,4,i+2)
		  imshow(V[i].reshape(m,n))
		show()
		"""
		import pickle
		# 平均と主成分を保存する
		f = open(output+url+"_pca.pkl", 'wb') 
		pickle.dump(immean,f) 
		pickle.dump(V,f) 
		f.close()
