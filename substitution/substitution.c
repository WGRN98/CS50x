#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// From uppercase to lowercase is 32

int main(int argc, string argv[])
{
    int i, n;
    // Get the key from command-line argument
    string key = argv[1];

    // Check if the correct number of command-line arguments is provided
    if (argc != 2)
    {
        printf("Invalid key, input 1 alphabetic key!");
        return 1;
    }

    // Check if the key length is not equal to 26
    if (strlen(argv[1]) != 26)
    {
        printf("ERROR, key must have 26 letters");
        return 1;
    }

    // Check if the character is not an alphabetic character or if it has already been visited
    bool visited[26] = {false};
    for (i = 0, n = strlen(key); i < n; i++)
    {
        char c = toupper(key[i]);

        // Check if the character is not an alphabetic character or if it has already been visited
        if (!isalpha(c) || visited[c - 'A'])
        {
            printf("Invalid Key!\n");
            return 1;
        }

        // Mark the character as visited
        visited[c - 'A'] = true;
    }

    // Convert the key to uppercase
    for (i = 0; i < 26; i++)
    {
        if (isalpha(argv[1][i]))
        {
            if (isupper(argv[1][i])) // Uppercase
                key[i] = argv[1][i];
            else // Lowercase
                key[i] = toupper(argv[1][i]);
        }
        else
        {
            printf("Invalid key!\n");
            return 1;
        }
    }

    string plain_text = get_string("plaintext: ");
    n = strlen(plain_text);
    char cipher_text[n + 1];

    // Encrypt plaintext
    for (i = 0; i < n; i++)
    {
        if (isalpha(plain_text[i]))
        {
            char shift = islower(plain_text[i]) ? 'a' : 'A';
            int index = plain_text[i] - shift;

            if (isupper(plain_text[i]))
                cipher_text[i] = key[index];
            if (islower(plain_text[i]))
                cipher_text[i] = tolower(key[index]);
        }
        else
        {
            // Non-alphabetic characters remain unchanged
            cipher_text[i] = plain_text[i];
        }
    }

    cipher_text[n] = '\0';

    printf("ciphertext: %s\n", cipher_text);
}
