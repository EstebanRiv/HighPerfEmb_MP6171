#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arm_neon.h>

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

    uint32_t n_bytes = heigth * width * 3;

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
        for(uint32_t i = 0; i<n_bytes-3; 3*i++)
        {



          uint32x4_t twoahead =  vaddq_u32(i,2);
          uint32x4_t oneahead =  vaddq_u32(i,1);
          int64x2_t sixtysixTimes_2 = vmull_s32 (buffer_in[twoahead], 66);
          int64x2_t onetwonineTimes_1 = vmull_s32(buffer_in[oneahead], 129);
          int64x2_t twentyfiveTimes = vmull_s32(buffer_in[i],25);
          uint32x2_t firstinternalSum =  vaddq_u32(vaddq_u32(sixtysixTimes_2  + onetwonineTimes_1) + vaddq_u32(twentyfiveTimes, 128));
          int32x2_t shiftleft_1 = vshl_n_s32 (firstinternalSum, 8);

          buffer_out[i]   = vaddq_u32(shiftleft_1, 16);                        // Y =  R*0.299000 + G*0.587000 + B*0.114000



          // int64x2_t thirtyeightTimes_2 = vmull_s32 (buffer_in[twoahead], 38);
          // int64x2_t seventyfourTimes_1 = vmull_s32 (buffer_in[oneahead], 74);
          // int64x2_t onetwelveTimes_1 = vmull_s32 (buffer_in[oneahead], 112);
          // int32x2_t negative = vmull_s32(thirtyeightTimes_2,-1);
          // int64x2_t onetwelveTimes = vmull_s32(buffer_in[i],112);
          // uint32x4_t SecondinternalSum =  vaddq_u32( vsubq_s32(negative, seventyfourTimes_1), vaddq_u32(onetwelveTimes, 128));
          // int32x2_t shiftleft_2 = vshl_n_s32 (SecondinternalSum, 8);
          // // // uint32x2_t index_2 = vadd_u32(i, n_bytes/3);

          // buffer_out[i, n_bytes/3] = vaddq_u32(shiftleft_2, 128;             // U = -R*0.168736 - G*0.331264 + B*0.500000 + 128



          // int64x2_t onetwelveTimes_2 = vmull_s32 (buffer_in[twoahead], 112);
          // int64x2_t ninetyfourTimes_1 = vmull_s32(buffer_in[oneahead], 94);
          // int32x4_t eighteenTimes = vmull_s32(buffer_in[i],18);
          // uint32x4_t ThirdinternalSum =  vsubq_s32( vsubq_s32(onetwelveTimes_2, ninetyfourTimes_1), vaddq_u32(eighteenTimes, 128));
          // int32x2_t shiftleft_3 = vshl_n_s32 (ThirdinternalSum, 8);
          // // // int64x2_t subIndex = vmull_s32(2,n_bytes/3);
          // // // uint32x2_t index_3 = vadd_u32(i, subIndex);


          // buffer_out[i, 2,n_bytes/3] = vaddq_u32(shiftleft_3, 128;)           // V =  R*0.500000 - G*0.418688 - B*0.081312 + 128



          //buffer_out[i]   = (( buffer_in[i+2]*66  + buffer_in[i+1]* 129 + buffer_in[i]*25  + 128) >> 8) + 16;                        // Y =  R*0.299000 + G*0.587000 + B*0.114000
          //buffer_out[i+(n_bytes/3)] = ((-buffer_in[i+2]*38  - buffer_in[i+1]* 74 + buffer_in[i]*112  + 128) >> 8) + 128;             // U = -R*0.168736 - G*0.331264 + B*0.500000 + 128
          //buffer_out[i+(2*n_bytes/3)] = (( buffer_in[i+2]*112 - buffer_in[i+1]* 94 - buffer_in[i]*18   + 128) >> 8) + 128;           // V =  R*0.500000 - G*0.418688 - B*0.081312 + 128
        }

    }

    write_ptr = fopen(output_image,"wb");
    fwrite(buffer_out,n_bytes,1,write_ptr);    

    fclose(ptr);
    fclose(write_ptr);
}