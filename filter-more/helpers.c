#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average_rgb = 0;
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue
            average_rgb =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            // Update pixel values
            image[i][j].rgbtRed = average_rgb;
            image[i][j].rgbtGreen = average_rgb;
            image[i][j].rgbtBlue = average_rgb;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int reflectRed, reflectGreen, reflectBlue;
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixels
            reflectRed = image[i][j].rgbtRed;
            reflectGreen = image[i][j].rgbtGreen;
            reflectBlue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;

            image[i][width - 1 - j].rgbtRed = reflectRed;
            image[i][width - 1 - j].rgbtGreen = reflectGreen;
            image[i][width - 1 - j].rgbtBlue = reflectBlue;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy of the image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Made this by brute forcing with switch case. Changed to for loops to make it more compact.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float count = 0.00;
            int blue = 0, green = 0, red = 0;
            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        blue += copy[k][l].rgbtBlue;
                        green += copy[k][l].rgbtGreen;
                        red += copy[k][l].rgbtRed;
                        count++;
                    }
                }
            }

            image[i][j].rgbtBlue = round(blue / count);
            image[i][j].rgbtGreen = round(green / count);
            image[i][j].rgbtRed = round(red / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE G[height][width];

    int GxR, GyR, GxG, GyG, GxB, GyB;

    // Initialize Gx and Gy metrix
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            GxR = GyR = GxG = GyG = GxB = GyB = 0;

            // Loop over 3x3 pixels
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    // Check if this pixel is at edge of the image
                    if ((i + h < 0 || i + h > height - 1) || (j + w < 0 || j + w > width - 1))
                        continue;

                    // Sum each channel value
                    GxR += image[i + h][j + w].rgbtRed * Gx[h + 1][w + 1];
                    GxG += image[i + h][j + w].rgbtGreen * Gx[h + 1][w + 1];
                    GxB += image[i + h][j + w].rgbtBlue * Gx[h + 1][w + 1];

                    GyR += image[i + h][j + w].rgbtRed * Gy[h + 1][w + 1];
                    GyG += image[i + h][j + w].rgbtGreen * Gy[h + 1][w + 1];
                    GyB += image[i + h][j + w].rgbtBlue * Gy[h + 1][w + 1];
                }
            }

            // Calculate every Gx and Gy value and store in temp
            G[i][j].rgbtRed = fmin(round(sqrt(GxR * GxR + GyR * GyR)), 255);
            G[i][j].rgbtGreen = fmin(round(sqrt(GxG * GxG + GyG * GyG)), 255);
            G[i][j].rgbtBlue = fmin(round(sqrt(GxB * GxB + GyB * GyB)), 255);
        }
    }

    // Ready to iterate whole image from temp to image[i][j]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = G[i][j];
        }
    }
}
