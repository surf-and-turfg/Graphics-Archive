import pygame
import math
import numpy
import time
pygame.init()

pygame.display.set_caption('Quick Start')
window_surface = pygame.display.set_mode((800, 800))

background = pygame.Surface((800, 800))
background.fill(pygame.Color('#000000'))
gravcons = 6.67430*math.pow(10,-4)
is_running = True
class planet:
  def __init__(self):
    self.pos = numpy.array((400,400))
    self.radius = 50
    self.mass = 1000000 #kg
  def draw(self):
    pygame.draw.circle(background, (255,255,255),tuple(self.pos), self.radius)

p1 = planet()
planets = []
planets.append(p1)
class rocket:
  def __init__(self):
    self.velocityx = 0
    self.velocityy = 0
    self.velocity = math.sqrt(self.velocityx*self.velocityx + self.velocityy*self.velocityy)
    self.mass = 1000 #kg
    self.kineticE = 0.5 * self.mass * self.velocity*self.velocity
    self.potentialE = 0
    self.rot = math.radians(15)
    self.pos = numpy.array((700,100))
  def gravity(self):
    for i in planets:
      self.dis = i.pos - self.pos
      self.distan = self.dis[0] * self.dis[0] + self.dis[1] * self.dis[1]
      self.angle = math.asin(self.dis[1]/math.sqrt(self.distan))
      self.pull = (gravcons * i.mass)/(self.distan)
      self.velocityx += self.pull * math.cos(self.angle) * (self.dis[0]/abs(self.dis[0]))
      self.velocityy += self.pull * math.sin(self.angle)
  def draw(self):
    self.pos = numpy.array((self.pos[0]+self.velocityx,self.pos[1]+self.velocityy))
    self.transform = numpy.array(((math.cos(self.rot),-math.sin(self.rot)),
                      (math.sin(self.rot),math.cos(self.rot))))
    self.p1 = numpy.array((0,15))
    self.p2 = numpy.array((10,-15))
    self.p3 = numpy.array((-10,-15))
    self.p1 = self.p1.dot(self.transform)
    self.p2 = self.p2.dot(self.transform)
    self.p3 = self.p3.dot(self.transform)
    self.p1 += self.pos
    self.p2 += self.pos
    self.p3 += self.pos
    pygame.draw.polygon(background,(255,255,255),(tuple(self.p1), tuple(self.p2), tuple(self.p3)))
  def thrust(self):
    self.thrustmag = 20
    self.accel = numpy.array((self.thrustmag/self.mass,0))
    self.transform = numpy.array(((math.cos(self.rot),-math.sin(self.rot)),
                      (math.sin(self.rot),math.cos(self.rot))))
    self.accel = self.accel.dot(self.transform)
    self.velocityx += -self.accel[1]
    self.velocityy += self.accel[0]
    self.kineticE = 0.5 * self.mass * self.velocity*self.velocity
   

r1 = rocket()
rotleft = False
rotright = False
thruster = False
def move():
  if rotleft == True:
    r1.rot -= math.radians(1)
  if rotright == True:
    r1.rot += math.radians(1)
  if thruster == True:
    r1.thrust()
while is_running:
  window_surface.blit(background, (0, 0))
  background.fill(pygame.Color('#000000'))
  for event in pygame.event.get():
    if event.type == pygame.QUIT:
      is_running = False
    if event.type == pygame.KEYDOWN:
      if event.key == pygame.K_a:
        rotright = True
      if event.key == pygame.K_d:
        rotleft = True
      if event.key == pygame.K_w:
        thruster = True
    if event.type == pygame.KEYUP:
      if event.key == pygame.K_a:
        rotright = False
      if event.key == pygame.K_d:
        rotleft = False
      if event.key == pygame.K_w:
        thruster = False
  r1.draw()
  p1.draw()
  r1.gravity()
  move()
  pygame.display.update()
  time.sleep(0.01)
