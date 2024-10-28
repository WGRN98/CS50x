import math

while (True):
    try:
        name = input("Card number:")
        number = int(name)
    except ValueError:
        print("Please input a number!")
    else:
        break

number1 = number
number2 = int(number / 10)
sum1 = sum2 = tmp1 = tmp2 = 0

while (number1 != 0):
    sum1 = int(sum1 + (number1 % 10))
    number1 = int(number1 / 100)

while (number2 != 0):
    if int((number2 % 10) * 2) > 9:
        tmp1 = int((int((number2 % 10)) * 2) - 10)
        tmp2 = int((int((number2 % 10)) * 2) / 10)
    else:
        tmp1 = int(int((number2 % 10)) * 2)
    sum2 = int(sum2 + tmp1 + tmp2)
    tmp1 = tmp2 = 0
    number2 = int(number2 / 100)

sum = int((sum1 + sum2) % 10)

if (sum != 0):
    print("INVALID")
    exit

elif ((int(number / pow(10, 12)) == 4 or int(number / pow(10, 15)) == 4) and (len(str(number)) == 13 or len(str(number)) == 16)):
    print("VISA")
    exit

elif ((int(number / pow(10, 14)) == 51 or int(number / pow(10, 14)) == 52 or int(number / pow(10, 14)) == 53 or int(number / pow(10, 14)) == 54 or int(number / pow(10, 14)) == 55) and len(str(number)) == 16):
    print("MASTERCARD")
    exit

elif ((int(number / pow(10, 13)) == 37 or int(number / pow(10, 13)) == 34) and len(str(number)) == 15):
    print("AMEX")
    exit

else:
    print("INVALID")
