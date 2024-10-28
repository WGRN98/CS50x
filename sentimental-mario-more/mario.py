height = 0

while (height <= 0 or height > 8):
    try:
        height = int(input("Height: "))

    except ValueError:
        print("Please input a number!")

    else:
        if height > 8 or height < 1:
            print("Input a valid height between 1 and 8")

for i in range(height):
    for s in range(height - i - 1, 0, -1):
        print(" ", end="")

    for j in range(i + 1):
        print("#", end="")

    print("  ", end="")

    for j in range(i + 1):
        print("#", end="")

    print("")
