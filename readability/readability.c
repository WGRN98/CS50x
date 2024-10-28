#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
void print_grade(float index);

int main(void)
{
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    float L = (float) letters / (float) words * 100;
    float S = (float) sentences / (float) words * 100;

    float index = 0.0588 * L - 0.296 * S - 15.8;
    index = round(index);

    print_grade(index);
}

int count_letters(string text)
{
    // Return the number of letters in text
    int count = 0;
    int len = strlen(text);

    for (int i = 0; i < len; i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }
    }

    return count;
}

int count_words(string text)
{
    // Return the number of words in text
    int count = 0;
    int len = strlen(text);

    for (int i = 0; i < len; i++)
    {
        if (text[i] == ' ' || text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            if (i + 1 < len && (text[i + 1] == ' ' || text[i + 1] == '.' || text[i + 1] == '!' ||
                                text[i + 1] == '?'))
            {
                continue;
            }

            count++;
        }
    }
    return count;
}

int count_sentences(string text)
{
    // Return the number of sentences in text
    int count = 0;
    int len = strlen(text);

    for (int i = 0; i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            if (i + 1 < len && (text[i + 1] == '.' || text[i + 1] == '!' || text[i + 1] == '?'))
            {
                continue;
            }

            count++;
        }
    }
    return count;
}

void print_grade(float index)
{
    printf("%f\n", index);
    // Print the grade
    if (index < 1)
        printf("Before Grade 1\n");

    else if (index < 2)
        printf("Grade 1\n");

    else if (index < 3)
        printf("Grade 2\n");

    else if (index < 4)
        printf("Grade 3\n");

    else if (index < 5)
        printf("Grade 4\n");

    else if (index < 6)
        printf("Grade 5\n");

    else if (index < 7)
        printf("Grade 6\n");

    else if (index < 8)
        printf("Grade 7\n");

    else if (index < 9)
        printf("Grade 8\n");

    else if (index < 10)
        printf("Grade 9\n");

    else if (index < 11)
        printf("Grade 10\n");

    else if (index < 12)
        printf("Grade 11\n");

    else if (index < 13)
        printf("Grade 12\n");

    else if (index < 14)
        printf("Grade 13\n");

    else if (index < 15)
        printf("Grade 14\n");

    else if (index < 16)
        printf("Grade 15\n");

    else
        printf("Grade 16+\n");
}
