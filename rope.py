import pygame
import time as t
import math

pygame.init()


pygame.display.set_caption('Quick Start')

window_surface = pygame.display.set_mode((640, 640))


background = pygame.Surface((640, 640))

background.fill(pygame.Color('#000000'))


is_running = True

gravity = 9.8

class ball:
    def __init__(self,posx,posy,velx,vely):
        self.posx = posx
        self.posy = posy
        self.velx = velx
        self.vely = -vely
        self.time = 0
        self.posyold = posy
        self.accely = gravity
        self.accelx = 0
    def prop_fwd(self):
        self.time = self.time + 0.01
        self.posx += self.velx*0.01
        self.vely = self.vely + gravity*self.time
        self.posy += self.vely*0.01
        #self.posy = self.posyold + -self.vely*self.time + ((self.time**2)*gravity)/2
        print(str(self.posy)+", "+str(self.time))
        pygame.draw.circle(background, (156,78,200),(self.posx,self.posy),15)
    def verlet(self):
        self.time = 0.01
        self.posycopy = self.posy
        self.posy = 2*self.posy - self.posyold + (self.time**2)*self.accely
        self.posyold = self.posycopy
        pygame.draw.circle(background, (156,78,200),(self.posx,self.posy),15)
    def draw(self):
        pygame.draw.circle(background, (156,78,200),(self.posx,self.posy),15)
        

ball1 = ball(320,320,0,20)

rope=[]

x = 0
y = 0
desired = 10
balls = []
tobedeleted = []
while is_running:
     for event in pygame.event.get():
         if event.type == pygame.QUIT:
             is_running = False
         elif event.type == pygame.MOUSEBUTTONDOWN:
             #uncomment for original shit
            # x = pygame.mouse.get_pos()[0]
            # y = pygame.mouse.get_pos()[1]
            
            #creating rope
            for i in range(0,60):
                rope.append(ball(pygame.mouse.get_pos()[0],pygame.mouse.get_pos()[1]+i*10,0,0))

         elif event.type == pygame.MOUSEBUTTONUP:
             #uncomment this too
             #balls.append(ball(x,y,(pygame.mouse.get_pos()[0]-x),(pygame.mouse.get_pos()[1]-y)/-1))
             rope.clear()
     window_surface.blit(background, (0, 0))
     background.fill((0,0,0))
     for i in range(0,len(balls)):
         balls[i].verlet()
         if balls[i].posx>700 or balls[i].posx<-100 or balls[i].posy<-100 or balls[i].posy>700:
             tobedeleted.append(i)
     for i in tobedeleted:
         balls.pop(i)



     #actual rope shit
     for i in rope:
         i.verlet()
     
     for i in range(0,len(rope)):
         if i == 0:
             distancey = pygame.mouse.get_pos()[1]-rope[i].posy
             distancex = pygame.mouse.get_pos()[0]-rope[i].posx
             deltady = desired - distancey
             deltadx = 0 - distancex
             rope[i].posx -= deltadx
             rope[i].posy -= deltady
         else:
             distancey = rope[i-1].posy-rope[i].posy
             distancex = rope[i-1].posx-rope[i].posx
             deltady = desired - distancey
             deltadx = 0 - distancex
             rope[i].posx -= deltadx/2
             rope[i].posy -= deltady/2
             rope[i-1].posx += deltadx/2
             rope[i-1].posy += deltady/2
         rope[i].draw()
     tobedeleted.clear()
     t.sleep(0.01)
     pygame.display.update()
