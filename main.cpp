#include <stdio.h>
#include <stdlib.h>

#include "float24.h"

double diff (const double input)
{
  float24_u var24 (input);

  double d = input - var24;

  return d;
}

// demonstrates float24_t usage.

void showFP24unsigned()
{
    unsigned char buffer [90]; // this will hold 30 float24_u values each taking 3 bytes.

    int cnt = 0;

    // this part prints difference between double and corresponding float24_u
    // and writes float24_u to a buffer (3 bytes each) 

    for (double f = M_PI; cnt < 30; f+=100)
    {
      printf ("(%f)\t diff : %.12lf\n", f, diff (f));

      float24_u value = f;

      memcpy (buffer + cnt * 3, value.data.buffer, 3);
      cnt ++;
    }

    // this part reads data for float24_u from the buffer.

    for (int i=0; i < cnt; i++)
    {
      float24_u var;

      memcpy (var.data.buffer, buffer + i * 3, 3);

      printf ("%d\t %f\n", i, float(var));
    }
}


void showFP24signed ()
{
    unsigned char buffer [90]; // this will hold 30 float24_u values each taking 3 bytes.

    int cnt = 0;

    // this part prints difference between double and corresponding float24_u
    // and writes float24_u to a buffer (3 bytes each) 

    for (double f = M_PI; cnt < 30; f += 100)
    {
      int sign = cnt & 1 ? 1 : -1;

      printf ("(%.12f)\t diff : %.12lf\n", sign * f, diff (sign * f));

      float24_s value = sign * f;

      memcpy (buffer + cnt * 3, value.data.buffer, 3);
      cnt ++;
    }

    // this part reads data for float24_u from the buffer.

    for (int i=0; i < cnt; i++)
    {
      float24_s var;

      memcpy (var.data.buffer, buffer + i * 3, 3);

      printf ("%d\t %f\n", i, float(var));
    }
}


int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf ("Usage: %s -(u|s)\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (0 == strcmp(argv[1], "-u"))
      showFP24unsigned ();

  else 
      showFP24signed ();

  return EXIT_SUCCESS;
}

