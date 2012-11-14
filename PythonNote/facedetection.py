# -*- coding: utf-8 -*-
import cv
import pygame,time

if __name__ == "__main__":
	pygame.init()
	pygame.display.set_caption('FaceDetection') 
	screen = pygame.display.set_mode([620,485])
		
	image=cv.LoadImage('temp.jpg', cv.CV_LOAD_IMAGE_COLOR)
	hc = cv.Load("E:\\Software\\Notepad++\\PythonEnv\\haarcascade_frontalface_alt.xml")
	face = cv.HaarDetectObjects(image, hc, cv.CreateMemStorage(), 1.2,2, cv.CV_HAAR_DO_CANNY_PRUNING, (0,0) )
	for (x,y,w,h),n in face:
		# print("[(%d,%d) -> (%d,%d)]" % (x,y,x+w, y+h))
		cv.Rectangle(image,(int(x),int(y)),(int(x)+w,int(y)+h),(255,255,255),1,0)
	cv.SaveImage('temp_detec.jpg', image)
	
	#¼ÓÔØÍ¼Ïñ
	image = pygame.image.load('temp_detec.jpg')
	#´«ËÍ»­Ãæ
	screen.blit(image,[2, 2])

	#ÏÔÊ¾Í¼Ïñ
	pygame.display.flip()
	
	time.sleep(1000)