#include <stdio.h>

int main(void)
{
   FILE *fp;
   char str[100];  

   fp = fopen("/dev/gpio_read", "r");
   fread(str, 1, 1, fp);
   if(str[0] == '1')
       printf("The button is pressed\n");
   else 
       printf("The button is not pressed\n");
   
   fclose(fp);
}
