import random


chars = "abcdefghijklmnopqrstuvwxyz"
chars += "".join([c.upper() for c in chars])

# print(chars)

with open("names.txt", "w") as names:
    for i in range(20000):
        names.write("".join(random.choice(chars) for _ in range(1000)))
        names.write("\n")

# x = "".join(random.choice(chars) for _ in range(1000))
