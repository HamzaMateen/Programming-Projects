import pygame 
import pymunk 
import math 
import pymunk.pygame_util 

import random
from typing import Tuple

# pymunk is unitless library, it is upto us to choose the units in the simulations 
pygame.init()

WIDTH, HEIGHT = 1000, 600
window = pygame.display.set_mode((WIDTH, HEIGHT))

def calculate_distance(p1, p2):
    return math.sqrt((p1[0] - p2[0])**2 + (p1[1] - p2[1])**2)
    

# the angle is in radians
def calculate_angle(p1, p2):
    return math.atan2(p2[1] - p1[1], p2[0] - p1[0])


def draw(space, window, draw_options, line : Tuple[Tuple[int, int], Tuple[int, int]]):
    window.fill("white")
    
    if line:
        pygame.draw.line(window, "red", line[0], line[1], 3)
    space.debug_draw(draw_options)
    pygame.display.update()
    
def create_ball(space, radius, mass, pos) -> pymunk.Circle:
    body = pymunk.Body(body_type=pymunk.Body.STATIC) 
    body.position = pos

    # (0, 0) is top-left corner of the 'window' object
    # attach a circle polygon to this body 

    circle = pymunk.Circle(body, radius) # radius = 50
    circle.mass = mass
    circle.color = [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]


    # without these props specified, the ball won't bounce off the surface.
    circle.elasticity = 0.9
    circle.friction = 0.4
    
    space.add(body, circle)
    return circle  

def create_square (space, side, mass):
    body = pymunk.Body()
    body.position = (WIDTH // 3, HEIGHT // 3)

    square = pymunk.Poly(body, [(0, 0), (0, side), (side, side), (side, 0)])
    square.mass = mass 

    square.color = (120, 120, 60, 90)
    space.add(body, square)

    return square  

def create_boundaries(space, width=WIDTH, height=HEIGHT):
    # this rectangles remain fixed in the window, and do not exhibit any motion upon gravitational pull

    rects = [
        [(width/2, height - 10), (width, 20)], # bottom
        [(width/2, 10), (width, 20)], # top
        [(10, height/2), (20, height)], # left
        [(width - 10, height/2), (20, height)], # right
    ]

    for pos, size in rects:
        body = pymunk.Body(body_type=pymunk.Body.STATIC)
        body.position = pos

        shape = pymunk.Poly.create_box(body, size)
        shape.color = (160, 160, 160, 255)

        shape.elasticity = 0.4
        shape.friction = 0.3

        space.add(body, shape) # global space objectS


# the idea is to launch the ball towards the mouse upon a click
# where the power with which the ball gets projectiled towards the mouse 
# is proportional to the distance between the ball and mouse.


# main event loop 
def run(window, width, height):
    run = True 
    clock = pygame.time.Clock()
    fps = 60

    space = pymunk.Space() # everythin in pymunk's physics is rendered and simulated here.
    space.gravity = (0, 981) # (x, y) directions 

    # gravity is +ve because in Pymunk, when we move DOWN the screen, the 'y' INCREASES.

    draw_options = pymunk.pygame_util.DrawOptions(window)

    create_boundaries(space)

    pressed_pos = None 
    ball = None 

    while run: 
        
        line = None
        # event checking loop 
        for event in pygame.event.get():
            if ball and pressed_pos:
                line = [pressed_pos, pygame.mouse.get_pos()]

            if event.type == pygame.QUIT: # triggered when red button clicked
                run = False 
                break 

            # here, if a large impulse if applied, the ball might escape out of the window
            # because the program is checking for the collision event every 1/fps seconds
            if event.type == pygame.MOUSEBUTTONDOWN:
                if not ball:
                    radius = random.randint(15, 75)

                    pressed_pos = pygame.mouse.get_pos()
                    ball = create_ball(space, radius, 10, pressed_pos)

                elif pressed_pos:
                    ball.body.body_type = pymunk.Body.DYNAMIC

                    angle = calculate_angle(*line)
                    force = calculate_distance(*line) * 100

                    x_dir_force = math.cos(angle) * force
                    y_dir_force = math.sin(angle) * force 

                    ball.body.apply_impulse_at_local_point((x_dir_force, y_dir_force), (0, 0))
                    pressed_pos = None         

                else:  
                    # remove ball from screen/window and from body       
                    space.remove(ball, ball.body)
                    ball = None
                    
        draw(space, window, draw_options, line)

        space.step(1/fps) 
        clock.tick(fps)

    pygame.quit()

if __name__ == "__main__":
    run(window, WIDTH, HEIGHT)


