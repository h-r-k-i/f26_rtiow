

/*
 * Original code in https://www.a1k0n.net/2011/07/20/donut-math.html 
 * 
 * Credits to Andy Sloane (github.com/a1k0n)
 * 
 * Extracted and refactored the code to be readable in order to study and comprehend it from the original 2016 
 * I resolved warnings, added some missing type declarations, added reasonable newlines.
 * Tested in Ubuntu 20.04
 * To compile it you need to link the C mathematical library with the -lm option.
 * 
 * (shell commands)
 * sudo apt install -y gcc
 * gcc donut.c -lm -o donut
 * chmod u+x donut
 * ./donut
 */


#include <stdio.h>
#include <string.h>

int k;
double sin(), cos();

int main()
{
  float A = 0, B = 0, i, j, z[1760];
  char b[1760];
  printf("\x1b[2J");
  for (;;)
  {
    memset(b, 32, 1760);
    memset(z, 0, 7040);

    for (j = 0; 6.28 > j; j += 0.07)
      for(i = 0; 6.28 > i; i += 0.02)
      {
        float c = sin(i); 
        float d = cos(j);
        float e = sin(A);
        float f = sin(j);
        float g = cos(A);
        float h = d + 2;
        float D = 1 / (c * h * e + f * g + 5);
        float l = cos (i);
        float m = cos(B); 
        float n = sin(B);
        float t = c * h * g - f * e;
        int x = 40 + 30 * D * (l * h * m - t * n);
        int y = 12 + 15 * D * (l * h * n + t * m);
        int o = x + 80 * y;
        int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
        if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o])
        {
          z[o] = D;;;
          b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
        }
      }
    printf("\x1b[H");
    for (k = 0; 1761 > k; k++)
      putchar(k%80 ? b[k] : 10);
    A += 0.04;
    B += 0.02;
  }
}