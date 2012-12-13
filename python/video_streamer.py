import pygame 
import sys
import pygame.camera 
from pygame.locals import *


pygame.init()
pygame.camera.init()

scrn_size = (640, 480)
screen = pygame.display.set_mode(scrn_size)

cam = pygame.camera.Camera('/dev/video0', scrn_size)
cam.start()

while 1:
    img = cam.get_image()
    screen.blit(img, (0, 0))
    pygame.display.update()
    for ev in pygame.event.get():
	if ev.type == pygame.QUIT:
	    sys.exit()

    

 
