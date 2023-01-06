import pygame 
import pymunk 
import math 
import pymunk.pygame_util 

# pymunk is unitless library, it is upto us to choose the units in the simulations 

pygame.init()

WIDTH, HEIGHT = 1000, 600
window = pygame.display.set_mode((WIDTH, HEIGHT))

def draw(space, window, draw_options):
    window.fill("white")
    space.debug_draw(draw_options)

    pygame.display.update()
    
def create_ball(space, radius, mass):
    body = pymunk.Body()    
    body.position = (WIDTH/2, HEIGHT/2)

    # (0, 0) is top-left corner of the 'window' object
    # attach a circle polygon to this body 

    circle = pymunk.Circle(body, 50) # radius = 50
    circle.mass = mass
    circle.color = (166, 10, 7, 100)
    space.add(body, circle)
    
    return circle  

def create_boundaries(space, width, height):
    rects = [
        
    ]
# main event loop 
def run(window, width, height):
    run = True 
    clock = pygame.time.Clock()
    fps = 60

    space = pymunk.Space() # everythin in pymunk's physics is rendered and simulated here.
    space.gravity = (0, 981) # (x, y) directions 

    # gravity is +ve because in Pymunk, when we move DOWN the screen, the 'y' INCREASES.

    draw_options = pymunk.pygame_util.DrawOptions(window)

    ball = create_ball(space, 30, 40)

    while run: 
        # event checking loop 
        for event in pygame.event.get():
            if event.type == pygame.QUIT: # triggered when red button clicked
                run = False 
                break 

        draw(space, window, draw_options)
        space.step(1/fps) 
        clock.tick(fps)

    pygame.quit()

if __name__ == "__main__":
    run(window, WIDTH, HEIGHT)


