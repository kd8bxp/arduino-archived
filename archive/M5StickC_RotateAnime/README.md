# M5StickC_RotateAnime  
Rotate animation for M5StickC  

## Files  
- M5StickC_Anime  
	- M5StickC_Anime.ino: Main program for Arduino  
	- img.c: Image data  
- rotateImage: Sample image  
- dir2array2byte.py: Python script to convert from bmp/png data to img.c  

## If you set original Image  
1. Prepare 7pcs of 80x80 bmp or png image files.(-45/-30/-15/0//15/30/45 degree)  
2. Put the image files in rotateImage directory.  
3. Run dir2array2byte.py to convert from the image files to img.c.  
4. Put img.c in Arduino project and write to M5StickC.  
