import numpy as np
import cv2
import os

main_dir = '/home/rishabh/hacktheworld/Focus Area - Image Processing/code/t_2'
images = os.listdir(main_dir)
os.chdir('/home/rishabh/hacktheworld/Focus Area - Image Processing/code/tag')
for image in images:
	img = cv2.imread(main_dir+"/"+image,0)
	thresh = np.mean(img)
	print image
	_,img2 = cv2.threshold(img,120,255,cv2.THRESH_BINARY)
	cv2.imwrite(image[:-4]+'_intermediate1'+'.jpg', img2)
	rows,cols = img.shape
	#cv2.imwrite('gray_label2.jpg', img)
	kernel = np.ones((rows/4,1))
	closing = cv2.morphologyEx(img2, cv2.MORPH_CLOSE, kernel)
	cv2.imwrite(image[:-4]+'_intermediate2'+'.jpg', closing)
	#kernel = np.ones((rows,1))
	#closing = cv2.morphologyEx(img2, cv2.MORPH_CLOSE, kernel)
	#cv2.imwrite(image[:-4]+'_binary'+'.jpg', closing)
	print closing.shape
	closed = np.sum(closing/255,axis=0)
	start = 0
	end = 0
	count = 0
	thresh = int(0.9*rows)
	centroid = 0
	for i in xrange(0,cols):
		if (closed[i] >= rows):
			end = end+1
		else:
			if (start != end) and (end - start > 100):
				lb = img[:,start:end]
				_,lb = cv2.threshold(lb,140,255,cv2.THRESH_BINARY)
				centroid = (end+start)/2
				cv2.imwrite(image[:-4]+'_'+str(count)+'_'+str(centroid)+'.jpg', lb)
				#print start,end
				count = count+1
			start = i+1
			end = i+1
			
