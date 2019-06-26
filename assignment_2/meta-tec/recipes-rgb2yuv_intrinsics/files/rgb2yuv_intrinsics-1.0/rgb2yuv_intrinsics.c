#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <arm_neon.h>

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

    unsigned char buffer_in[n_bytes];
    signed char buffer_out[n_bytes];

    // read input RGB file
    ptr = fopen(input_image,"rb");
    fread(buffer_in,n_bytes,1,ptr);

    // rgb_x3x16_tmp.val[0] -> R
    // rgb_x3x16_tmp.val[1] -> G
    // rgb_x3x16_tmp.val[2] -> B
    uint8x16x3_t rgb_x3x16_tmp;

    // yuv_x3x16_tmp.val[0] -> Y
    // yuv_x3x16_tmp.val[1] -> U
    // yuv_x3x16_tmp.val[2] -> V
    uint8x16x3_t yuv_x3x16_tmp;

    // pointer to input RGB image
    uint8_t *rgb_ptr = buffer_in;
    uint8_t *yuv_ptr = buffer_out;

    if (ptr != NULL)
    {
        // Input format: RGB24, low memory-->|R G B|R G B|R G B|....|R G B|R G B|R G B|<--high memory
        // Output format: YUV444 plannar, low memory->|Y Y Y Y..|U U U U..|V V V V..|<--high memory
        
        // get YUV components
        for(int i = 0; i<n_pixels/16; i++)
        {
            //   YUV 4:4:4 Planar
            /*
            buffer_out[i] = (( buffer_in[(i*3)]*66  + buffer_in[(i*3)+1]* 129 + buffer_in[(i*3)+2]*25   + 128) >> 8) + 16;      
            buffer_out[n_pixels+i] = ((-buffer_in[(i*3)]*38  - buffer_in[(i*3)+1]* 74  + buffer_in[(i*3)+2]*112  + 128) >> 8) + 128;     
            buffer_out[2*n_pixels+i] = (( buffer_in[(i*3)]*112 - buffer_in[(i*3)+1]* 94  - buffer_in[(i*3)+2]*18   + 128) >> 8) + 128;
            */  

            // get rgb for next 16 pixels
            rgb_x3x16_tmp = vld3q_u8(rgb_ptr+i*16*3);

            // get RGB high and low vector sectors in 16bit uints (needed to handle multiplications)
            uint8x8_t high_r = vget_high_u8(rgb_x3x16_tmp.val[0]);
            uint8x8_t low_r = vget_low_u8(rgb_x3x16_tmp.val[0]);
            uint8x8_t high_g = vget_high_u8(rgb_x3x16_tmp.val[1]);
            uint8x8_t low_g = vget_low_u8(rgb_x3x16_tmp.val[1]);
            uint8x8_t high_b = vget_high_u8(rgb_x3x16_tmp.val[2]);
            uint8x8_t low_b = vget_low_u8(rgb_x3x16_tmp.val[2]);

            int16x8_t signed_high_r = vreinterpretq_s16_u16(vaddl_u8(high_r, vdup_n_u8(0)));
            int16x8_t signed_low_r = vreinterpretq_s16_u16(vaddl_u8(low_r, vdup_n_u8(0)));
            int16x8_t signed_high_g = vreinterpretq_s16_u16(vaddl_u8(high_g, vdup_n_u8(0)));
            int16x8_t signed_low_g = vreinterpretq_s16_u16(vaddl_u8(low_g, vdup_n_u8(0)));
            int16x8_t signed_high_b = vreinterpretq_s16_u16(vaddl_u8(high_b, vdup_n_u8(0)));
            int16x8_t signed_low_b = vreinterpretq_s16_u16(vaddl_u8(low_b, vdup_n_u8(0)));

            // coeficients for Y
            uint8x8_t coef1 = vdup_n_u8(66);
            uint8x8_t coef2 = vdup_n_u8(129);
            uint8x8_t coef3 = vdup_n_u8(25);

            // compute Y
            uint16x8_t high_y = vmull_u8(high_r, coef1);
            high_y = vmlal_u8(high_y, high_g, coef2);
            high_y = vmlal_u8(high_y, high_b, coef3);
            uint16x8_t low_y  = vmull_u8(low_r, coef1);
            low_y = vmlal_u8(low_y, low_g, coef2);
            low_y = vmlal_u8(low_y, low_b, coef3);

            high_y = vaddq_u16(high_y, vdupq_n_u16(128));
            low_y = vaddq_u16(low_y, vdupq_n_u16(128));

            uint8x16_t y = vcombine_u8(vqshrn_n_u16(low_y, 8), vqshrn_n_u16(high_y, 8));
            yuv_x3x16_tmp.val[0] = y;

            // coeficients for U
            int16x8_t scoef1 = vdupq_n_s16(-38);
            int16x8_t scoef2 = vdupq_n_s16(-74);
            int16x8_t scoef3 = vdupq_n_s16(-112);

            // compute U
            int16x8_t high_u = vmulq_s16(signed_high_r, scoef1);
            high_u = vmlaq_s16(high_u, signed_high_g, scoef2);
            high_u = vmlaq_s16(high_u, signed_high_b, scoef3);
            int16x8_t low_u = vmulq_s16(signed_low_r, scoef1);
            low_u = vmlaq_s16(low_u, signed_low_g, scoef2);
            low_u = vmlaq_s16(low_u, signed_low_b, scoef3);

            high_u = vaddq_s16(high_u, vdupq_n_s16(128));
            low_u = vaddq_s16(low_u, vdupq_n_s16(128));

            int8x16_t u = vcombine_s8(vqshrn_n_s16(low_u, 8), vqshrn_n_s16(high_u, 8));
            u = vaddq_s8(u, vdupq_n_s8(128));
            yuv_x3x16_tmp.val[1] = vreinterpretq_u8_s8(u);

            // coeficients for V
            scoef1 = vdupq_n_s16(112);
            scoef2 = vdupq_n_s16(-94);
            scoef3 = vdupq_n_s16(-18);

            // compute V
            int16x8_t high_v = vmulq_s16(signed_high_r, scoef1);
            high_v = vmlaq_s16(high_v, signed_high_g, scoef2);
            high_v = vmlaq_s16(high_v, signed_high_b, scoef3);
            int16x8_t low_v = vmulq_s16(signed_low_r, scoef1);
            low_v = vmlaq_s16(low_v, signed_low_g, scoef2);
            low_v = vmlaq_s16(low_v, signed_low_b, scoef3);

            high_v = vaddq_s16(high_v, vdupq_n_s16(128));
            low_v = vaddq_s16(low_v, vdupq_n_s16(128));

            int8x16_t v = vcombine_s8(vqshrn_n_s16(low_v, 8), vqshrn_n_s16(high_v, 8));
            v = vaddq_s8(v, vdupq_n_s8(128));
            yuv_x3x16_tmp.val[2] = vreinterpretq_u8_s8(v);;

            // store
            vst3q_u8(yuv_ptr, yuv_x3x16_tmp);
            yuv_ptr += 3*16;
        }
    }

    write_ptr = fopen(output_image,"wb");
    fwrite(buffer_out,n_bytes,1,write_ptr);    

    fclose(ptr);
    fclose(write_ptr);
}

