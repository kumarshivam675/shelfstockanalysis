import os
import cv2

main_dir = '/home/rishabh/hacktheworld/Focus Area - Image Processing/code/t9'
os.chdir(main_dir)
files = os.listdir(main_dir)

count=0

for f in files:
	if (f[-4:] == '.jpg'):
		img = cv2.imread(f)
		cv2.imwrite('/home/rishabh/hacktheworld/Focus Area - Image Processing/code/t10/'+str(count)+'/'+f, img)
		count = count + 1
		if (count > 200):
			count = 0
