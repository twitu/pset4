/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
 
 /*#include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <string.h>

char sign1 = {0xff, 0xd8, 0xff};
char chunk [512] = {1};
char* name;
int count;
FILE* raw = fopen("card.raw", "r");

int main (void)
{
    count++;
    sprintf(name, "%03d.jpg", count);
    printf("%s", name);
    return 0;
}*/
/**
 * M. Adan
 *
 * Computer Science 50
 * PSet 4
 *
 * resize.c
 */


#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(void)
{   
    // open the file programmatically with fopen
    FILE* infile = fopen("card.raw", "r");
    
    // check file permission
    if (infile == NULL)
    {
        fclose(infile); 
        printf("file opening error\n");
        return 1;
    }

    // counter
    int counter = 0; 
   
    // a buffer of 512 bytes
    BYTE buffer[512];
   
    // three-digit decimal number from 000 on up
    char lname[10]; 
   
    // temp file output
    FILE* temp = NULL; 
    
    // until end of file is reached
    while (!feof(infile))
    {
       
        // check first couple of sequence of the jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            // if already exist, then close file
            if (temp != NULL)
            {
                fclose(temp);
                
            }
            
            // label the files with three digits
            sprintf(lname, "%03d.jpg", counter);
            
            // next file in line
            temp = fopen(lname, "w");
            
            // bump the counter up
            counter++;
            
            // new file with buffer
            fwrite(buffer, sizeof(buffer), 1, temp);
        }
        else if (counter > 0)
        {
            // put jpg into temp
            fwrite(buffer, sizeof(buffer), 1, temp);            
            
        }
      
        fread(buffer, sizeof(buffer), 1, infile);
        
    }
  
    
    // close the file
    fclose(infile);

    // that's all folks
    return 0;
}