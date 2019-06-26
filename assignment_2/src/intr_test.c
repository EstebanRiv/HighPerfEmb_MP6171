#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

// Output formats
//#define YUV_420
//#define YUV_444

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
    signed char buffer_y[n_pixels];
    signed char buffer_u[n_pixels];
    signed char buffer_v[n_pixels];
    signed char buffer_out[n_bytes];

    // read input RGB file
    ptr = fopen(input_image,"rb");
    fread(buffer_in,n_bytes,1,ptr);

    if (ptr != NULL)
    {
        // Input format: RGB24, low memory-->|R G B|R G B|R G B|....|R G B|R G B|R G B|<--high memory
        // Output format: YUV444 plannar, low memory->|Y Y Y Y..|U U U U..|V V V V..|<--high memory
        
        // get YUV components
        for(int i = 0; i<n_pixels; i++)
        {
            //   YUV 4:4:4 Planar
            /*
            buffer_out[i] = (( buffer_in[(i*3)]*66  + buffer_in[(i*3)+1]* 129 + buffer_in[(i*3)+2]*25   + 128) >> 8) + 16;      
            buffer_out[n_pixels+i] = ((-buffer_in[(i*3)]*38  - buffer_in[(i*3)+1]* 74  + buffer_in[(i*3)+2]*112  + 128) >> 8) + 128;     
            buffer_out[2*n_pixels+i] = (( buffer_in[(i*3)]*112 - buffer_in[(i*3)+1]* 94  - buffer_in[(i*3)+2]*18   + 128) >> 8) + 128;
            */     
        }
    }

    write_ptr = fopen(output_image,"wb");
    fwrite(buffer_out,n_bytes,1,write_ptr);    

    fclose(ptr);
    fclose(write_ptr);
}

