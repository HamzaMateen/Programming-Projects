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

        space.add(body, shape)



# main event loop 
def run(window, width, height):
    run = True 
    clock = pygame.time.Clock()
    fps = 60

    space = pymunk.Space() # everythin in pymunk's physics is rendered and simulated here.
    space.gravity = (0, 981) # (x, y) directions 

    # gravity is +ve because in Pymunk, when we move DOWN the screen, the 'y' INCREASES.

    draw_options = pymunk.pygame_util.DrawOptions(window)

    bounds = create_boundaries(space)

    ball = create_ball(space, 30, 40)
    # square = create_square(space, 100, 100)
    
    while run: 
        
        # event checking loop 
        for event in pygame.event.get():
            if event.type == pygame.QUIT: # triggered when red button clicked
                run = False 
                break 

            # here, if a large impulse if applied, the ball might escape out of the window
            # because the program is checking for the collision event every 1/fps seconds
            if event.type == pygame.MOUSEBUTTONDOWN:
                ball.body.apply_impulse_at_local_point((-50000, -600), (0, 0))

        draw(space, window, draw_options)

        space.step(1/fps) 
        clock.tick(fps)

    pygame.quit()

if __name__ == "__main__":
    run(window, WIDTH, HEIGHT)


