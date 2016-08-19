#include <stdio.h>

int main(int argc, char *argv[])
{
   FILE *fp;
   char ch;

   if(argc < 2)
   {
       fprintf(stderr, "Error: Arguments\n");
       return -1;
    }

   fp = fopen("/dev/gpio_write", "w");

   ch = argv[1][0];

   fprintf(fp, "%c", ch);

   fclose(fp);
}
