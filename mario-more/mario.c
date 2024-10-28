#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int i = 1, j, s, x;

    while (i == 1)
    {
        x = get_int("How high do you want the pyramid? (Between 1 and 8) ");
        if (x > 8 || x < 1)
        {
            printf("Invalid mumber entered, please select a number between 1 and 8\n");
        }
        else if (x > 0 && x < 9)
        {
            i = 0;
        }
    }

    for (i = 0; i < x; i++)
    {
        for (s = x - 1; s > j; s--)
        {
            printf(" ");
        }

        for (j = 0; j <= i; j++)
        {

            printf("#");
        }

        printf("  ");

        for (j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
