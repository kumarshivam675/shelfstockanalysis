#import Image
from PIL import Image
import pytesseract
import os

for i in xrange(0,201):
	print i
	main_dir = '/home/rishabh/hacktheworld/Focus Area - Image Processing/code/t10/'+str(i)
	os.chdir(main_dir)
	files = os.listdir(main_dir)

	for f in files:
		if (f[-4:] == '.jpg'):
			#print f
			a = pytesseract.image_to_string(Image.open(main_dir+'/'+f))
			if len(a):
				target = open(main_dir+'/'+f[:-4]+'.txt', 'w')
				target.write(a)
				target.close()
			else:
				os.remove(f)
