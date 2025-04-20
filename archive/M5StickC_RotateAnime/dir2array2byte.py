# -*- coding: utf-8 -*-

from PIL import Image
import numpy as np
import glob

#print('フォルダ名')
#dirname = input('>> ')
dirname = 'rotateImage'

files = glob.glob(dirname+'\*')
#最初の画像を開く
img0 = Image.open(files[0])
width0, height0 = img0.size

#ファイル名を記述
path_w = 'img.c'
with open(path_w, mode='w') as f:
    for file in files:
        print(file)
        f.write('//'+file+'\n')

    f.write('const unsigned short img[{}][{}] = '.format(len(files),width0*height0))
    f.write('{\n')

    for i,file in enumerate(files):
        print(file)
        img = Image.open(file)
        f.write('\t{\n')
        for y in range(height0):
            f.write('\t')
            for x in range(width0):
                img_pixels = img.getpixel((x,y))
                #565カラーに変換
                R = img_pixels[0]>>3
                G = img_pixels[1]>>2
                B = img_pixels[2]>>3
                img_data = int(R*2048+G*32+B)
                f.write(format(img_data,'#06x'))
                #最終列の時
                if x == width0-1:
                    #最終行の時
                    if y == height0-1:
                        f.write('\n\t}')
                    else:
                        f.write(',\n')

                else:
                    f.write(',')
        if i < len(files)-1:
            f.write(',')
    f.write('};')
