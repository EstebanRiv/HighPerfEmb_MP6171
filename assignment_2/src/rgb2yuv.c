#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

void printHelp (void);
void printAuthorInfo (void);
void argumentParser (int argc, char **argv);
void rgb2yuv (char *input_image, char *output_image);

char *cvalue = NULL;
char *rgbFile = NULL;
char *yuvFile = NULL;

struct timeval start, end;
double t;

/* */
int main (int argc, char **argv)
{
    // get start time to init "profiler"
    gettimeofday(&start, NULL);

    // parse argumetns
    argumentParser(argc, argv);

    if ((rgbFile != NULL) && (yuvFile != NULL))
    {
       rgb2yuv(rgbFile, yuvFile);
    }

    // get end time
    gettimeofday(&end, NULL);
    t = (double) ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000000.0;

    printf("time: %f\n",t);
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
    int n_pixels = heigth * width;
    int index = 0;

    unsigned char buffer_in[n_bytes];
    unsigned char buffer_y[n_pixels];
    unsigned char buffer_u[n_pixels];
    unsigned char buffer_v[n_pixels];
    unsigned char buffer_out[n_bytes];

    // read input RGB file
    ptr = fopen(input_image,"rb");
    fread(buffer_in,n_bytes,1,ptr);

    if (ptr != NULL)
    {
        // Input format: RGB24, low memory-->|B G R|B G R|B G R|....|B G R|B G R|B G R|<--high memory
        // Output format: YUV444 planar, low memory->|Y Y Y Y Y..|U V U V U V U V U V...|<--high memory
           
        printf("r: %i, g: %i, b: %i\n", buffer_in[5],buffer_in[4],buffer_in[3]) ;  

        // get YUV components
        for(int i = 0; i<n_pixels; i++)
        {         
            buffer_y[i] =  buffer_in[(i*3)+2]*0.299   + buffer_in[(i*3)+1]* 0.587   + buffer_in[(i*3)]*0.114;            // Y =  R*0.299000 + G*0.587000 + B*0.114000
            buffer_u[i] = -buffer_in[(i*3)+2]*0.1687  - buffer_in[(i*3)+1]* 0.3313  + buffer_in[(i*3)]*0.5   + 128;        // U = -R*0.168736 - G*0.331264 + B*0.500000 + 128
            buffer_v[i] =  buffer_in[(i*3)+2]*0.5     - buffer_in[(i*3)+1]* 0.4187  - buffer_in[(i*3)]*0.813 + 128;       // V =  R*0.500000 - G*0.418688 - B*0.081312 + 128
        }

        // wrtie yuv file
        for(int i = 0; i<n_bytes/3; i++)
        {         
            buffer_out[i] = buffer_y[i];
            buffer_out[(n_bytes/3)+i] = buffer_u[i];
            buffer_out[2*(n_bytes/3)+i] = buffer_v[i];
        }        
         
    }

    write_ptr = fopen(output_image,"wb");
    fwrite(buffer_out,n_bytes,1,write_ptr);    

    fclose(ptr);
    fclose(write_ptr);
}

