import cv2
import numpy as np
from matplotlib import pyplot as plt

tmpl_count = 21
tmpl_frmt = "tmpl"
initial_img = cv2.imread('scene.png')
img = cv2.imread('scene.png',0)

for i in range(tmpl_count):
    img2 = img.copy()
    tmpl_name = tmpl_frmt + str(i+1) + ".png"
    print tmpl_name
    template = cv2.imread(tmpl_name,0)
    w, h = template.shape[::-1]
    # All the 6 methods for comparison in a list
    methods = ['cv2.TM_CCORR_NORMED']
    for meth in methods:
        img = img2.copy()
        method = eval(meth)
        # Apply template Matching
        res = cv2.matchTemplate(img,template,method)
        min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
        # If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum
        if method in [cv2.TM_SQDIFF, cv2.TM_SQDIFF_NORMED]:
            top_left = min_loc
        else:
            top_left = max_loc
        bottom_right = (top_left[0] + w, top_left[1] + h)
        cv2.rectangle(initial_img,top_left, bottom_right, 255, 2)

cv2.imwrite("final.png", initial_img)
