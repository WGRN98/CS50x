#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int firstDigit(long int n)
{
    // Remove last digit from number
    // till only one digit is left
    while (n >= 10)
        n /= 10;
    return n;
}

int numDigits(long int n)
{
    int d = 0;
    do
    {
        n /= 10;
        d++;
    }
    while (n != 0);

    return d;
}

int main(void)
{
    int sum1 = 0, sum2 = 0, digit, tmp1 = 0, tmp2 = 0;
    long int number = get_long("Card Number:");
    long int number1 = number, number2 = number;

    while (number1 != 0) // sum of digits not multiplied by 2
    {
        sum1 = sum1 + (number1 % 10);
        number1 = number1 / 100;
    }

    number2 = number2 / 10;
    while (number2 != 0) // sum of digits multiplied by 2
    {
        if (((number2 % 10) * 2) > 9)
        {
            tmp1 = ((number2 % 10) * 2) - 10;
            tmp2 = ((number2 % 10) * 2) / 10;
        }
        else
            tmp1 = (number2 % 10) * 2;
        sum2 = sum2 + tmp1 + tmp2;
        tmp1 = 0;
        tmp2 = 0;
        number2 = number2 / 100;
    }

    if ((sum1 + sum2) % 10 != 0)
    {
        printf("INVALID\n");
        exit(0);
    }
    else if (firstDigit(number) == 4 && (numDigits(number) == 13 || numDigits(number) == 16))
    {
        printf("VISA\n");
        exit(0);
    }
    else if ((number / (long int) pow(10, 14) == 51 || number / (long int) pow(10, 14) == 52 ||
              number / (long int) pow(10, 14) == 53 || number / (long int) pow(10, 14) == 54 ||
              number / (long int) pow(10, 14) == 55) &&
             numDigits(number) == 16)
    {
        printf("MASTERCARD\n");
        exit(0);
    }
    else if ((number / (long int) pow(10, 13) == 37 || number / (long int) pow(10, 13) == 34) &&
             numDigits(number) == 15)
    {
        printf("AMEX\n");
        exit(0);
    }
    else
    {
        printf("INVALID\n");
    }
}
