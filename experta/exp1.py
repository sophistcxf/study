from experta import *
from enum import Enum

class Nationality(Enum):
    englishman = 0
    spaniard = 1
    ukrainian = 2
    norwegian = 3
    japanese = 4


class Color(Enum):
    red = 0
    green = 1
    ivory = 2
    yellow = 3
    blue = 4


class Pet(Enum):
    dog = 0
    snails = 1
    fox = 2
    horse = 3
    zebra = 4


class Drink(Enum):
    water = 0
    coffee = 1
    milk = 2
    orange_juice = 3
    tea = 4


class Smokes(Enum):
    old_golds = 0
    kools = 1
    chesterfields = 2
    lucky_strikes = 3
    parliaments = 4

f1 = Fact(Nationality.englishman, MATCH.n1)
print(f1)

Fact(Color.red, MATCH.c1 & MATCH.n1)

# The Spaniard owns the dog.
Fact(Nationality.spaniard, MATCH.n2 & ~MATCH.n1)
Fact(Pet.dog, MATCH.p1 & MATCH.n2)

