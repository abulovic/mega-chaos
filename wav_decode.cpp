// http://bit.ly/1Je539P

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sndfile.h>

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3) {
        cout << "usage: " << argv[0] << " file.wav file.dat";
        exit(-1);
    }
    SNDFILE *sf;
    SF_INFO info;
    int num_channels;
    int num, num_items;
    int *buf;
    int f,sr,c;
    int i,j;
    FILE *out;
    
    /* Open the WAV file. */
    info.format = 0;
    sf = sf_open(argv[1], SFM_READ, &info);
    if (sf == NULL)
    {
        printf("Failed to open the file.\n");
        exit(-1);
    }
    /* Print some of the info, and figure out how much data to read. */
    f = info.frames;
    sr = info.samplerate;
    c = 1/*info.channels*/;
    printf("frames=%d\n",f);
    printf("samplerate=%d\n",sr);
    printf("channels=%d\n",c);
    num_items = f*c;
    printf("num_items=%d\n",num_items);
    /* Allocate space for the data to be read, then read it. */
    buf = (int *) malloc(num_items*sizeof(int));
    num = sf_read_int(sf,buf,num_items);
    sf_close(sf);
    printf("Read %d items\n",num);
    /* Write the data to filedata.out. */
    out = fopen(argv[2], "w");
    for (i = 0; i < num; i += c)
    {
        for (j = 0; j < c; ++j)
        {
            fprintf(out, "%f", (float) buf[i+j] / 4294967296.0f);
            if (j + 1 < c)
                fprintf(out, ",");
        }
        fprintf(out,",");
    }
    fclose(out);
    return 0;
}