
from random import choice
from string import ascii_uppercase

count = int(input())

print(''.join(choice(ascii_uppercase) for i in range(count)))
