/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdint.h>
#include <stdio.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(void)
{   
    // open the file programmatically with fopen
    FILE* inputfile = fopen("card.raw", "r");
    
    // check file permission
    if (inputfile == NULL)
    {
        fclose(inputfile); 
        printf("file opening error\n");
        return 1;
    }
    
    int counter = 0; 
    
    // a buffer of 512 bytes
    BYTE buffer[512];
   
    // three-digit decimal number from 000 on up
    char name[10]; 
   
    // temp file output
    FILE* temp = NULL; 
    
    // until end of file is reached
    while (!feof(inputfile))
    {
       
        // check for jpg signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            // if already exist, then close file
            if (temp != NULL)
            {
                fclose(temp);
            }
            
            // label the files with three digits
            sprintf(name, "%03d.jpg", counter);
            
            // next file in line
            temp = fopen(name, "w");
            
            // new file with buffer
            fwrite(buffer, sizeof(buffer), 1, temp);
            
            // bump the counter up
            counter++;
        }
        else if (counter > 0)
        {
            // put jpg into temp
            fwrite(buffer, sizeof(buffer), 1, temp);            
            
        }
      
        fread(buffer, sizeof(buffer), 1, inputfile);
        
    }
    fclose(inputfile);
    
    return 0;
}