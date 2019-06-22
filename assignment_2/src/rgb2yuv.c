#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printHelp (void);
void printAuthorInfo (void);
void argumentParser (int argc, char **argv);
void rgb2yuv (char *input_image, char *output_image);

char *cvalue = NULL;
char *rgbFile = NULL;
char *yuvFile = NULL;

/* */
int main (int argc, char **argv)
{
    // parse argumetns
    argumentParser(argc, argv);

    if ((rgbFile != NULL) && (yuvFile != NULL))
    {
       rgb2yuv(rgbFile, yuvFile);
    }
}

/* */
void argumentParser(int argc, char **argv)
{
    int c;

    while ((c = getopt (argc, argv, "ahi:o:")) != -1)
    {
        switch (c)
        {
            case 'a':
                printAuthorInfo();
            break;
            case 'h':
                printHelp();
            break;
            case 'o':
                yuvFile = optarg;
            break;
            case 'i':
                rgbFile = optarg;
            break;                                    
            default:
                printf ("Error, no parameters found!!");
            break;
        }
    }
}

/* */
void printHelp()
{
    char *helpString = 
    "Usage\n./rgb2yuv [ -i RGBfile ] [ -o YUVfile ] [ -h ] [ -a ]\n-i RGBfile specifies the RGB file to be converted.\n-o YUVfile specifies the output file name.\n-a displays the information of the author of the program.\n-h displays the usage message to let the user know how to execute the application.";

    printf("%s\n", helpString);
}

/* */
void printAuthorInfo()
{
    char *AuthorInfoString = 
    "Instituto Tecnologico de Costa Rica\nEscuela de Ingenieria Electronica\nMP6171 - Sistemas Empotrados de Alto Desempeno\nGrupo 2:\n    Fernando Paris\n    Esteban Rivera\n    Kevin Viquez";

    printf("%s\n", AuthorInfoString);
}

/* */
void rgb2yuv(char *input_image, char *output_image)
{
    FILE *ptr;
    FILE *write_ptr;

    printf("In file: %s\n", input_image);
    printf("Out file: %s\n", output_image);

    int heigth = 640;
    int width = 480;

    int n_bytes = heigth * width * 3;

    unsigned char buffer_in[n_bytes];
    unsigned char buffer_out[n_bytes];

    // read input RGB file
    ptr = fopen(input_image,"rb");
    fread(buffer_in,n_bytes,1,ptr);

    if (ptr != NULL)
    {
        // Input format: RGB24, low memory-->|B G R|B G R|B G R|....|B G R|B G R|B G R|<--high memory
        // Output format: YUV444 packed, low memory->|Y Y Y Y Y..|U U U U U...|V V V V V|<--high memory
           
        // i=B, i+1=G, i+2=R
        for(int i = 0; i<n_bytes-3; 3*i++)
        {            
            buffer_out[i]   = (( buffer_in[i+2]*66  + buffer_in[i+1]* 129 + buffer_in[i]*25  + 128) >> 8) + 16;                        // Y =  R*0.299000 + G*0.587000 + B*0.114000
            buffer_out[i+(n_bytes/3)] = ((-buffer_in[i+2]*38  - buffer_in[i+1]* 74 + buffer_in[i]*112  + 128) >> 8) + 128;             // U = -R*0.168736 - G*0.331264 + B*0.500000 + 128
            buffer_out[i+(2*n_bytes/3)] = (( buffer_in[i+2]*112 - buffer_in[i+1]* 94 - buffer_in[i]*18   + 128) >> 8) + 128;           // V =  R*0.500000 - G*0.418688 - B*0.081312 + 128
        }
         
    }

    write_ptr = fopen(output_image,"wb");
    fwrite(buffer_out,n_bytes,1,write_ptr);    

    fclose(ptr);
    fclose(write_ptr);
}

