import time
import pygame
import sys

# Initialize Pygame
pygame.init()

# Screen dimensions
SCREEN_WIDTH = 820
SCREEN_HEIGHT = 200

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)

# Rectangle dimensions and spacing
RECT_WIDTH = 80
RECT_HEIGHT = 160
SPACING = 20

# Initialize the screen
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Rectangle Toggle")

# Create rectangles
rectangles = []
colors = [RED] * 8  # Initialize all rectangles to red
for i in range(8):
    rect = pygame.Rect((RECT_WIDTH + SPACING) * i + SPACING, (SCREEN_HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT)
    rectangles.append(rect)

# Font
font = pygame.font.Font(None, 36)

# Function to toggle rectangle color
def toggle_color(index):
    if colors[index] == RED:
        colors[index] = GREEN
    else:
        colors[index] = RED


def calcul_somme():
    cnt, somme = 0, 0
    cnt = 0
    for i in range(len(colors)):
        if colors[i] == GREEN:
            cnt += 1
            somme +=i
    if cnt :
        somme = somme / cnt
    return somme,cnt
# Main loop
running = True

while running:
    cnt = 0
    somme = 0 
    green_count = colors.count(GREEN)
    red_count = len(colors) - green_count

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Left mouse button
                pos = pygame.mouse.get_pos()
                for i, rect in enumerate(rectangles):
                    if rect.collidepoint(pos):
                        toggle_color(i)
                        break

    # Draw rectangles
    screen.fill(WHITE)
    for i, rect in enumerate(rectangles):
        pygame.draw.rect(screen, colors[i], rect)

    somme,cnt = calcul_somme()
    # Render text
    green_text = font.render("somme: " + str(somme), True, BLACK)
    red_text = font.render("cnt: " + str(cnt), True, BLACK)
    screen.blit(green_text, (20, 20))
    screen.blit(red_text, (20, 60))

    pygame.display.flip()
    time.sleep(1/20)
# Quit Pygame
pygame.quit()
sys.exit()
