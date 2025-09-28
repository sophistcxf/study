import math

def ptop_length(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    sx = math.cos((y1+y2)*0.01745329252/2)
    return math.sqrt(dx * dx * sx + dy * dy) * 111195.0

print ptop_length(118.671997,36.819508,118.749413,36.882271)
