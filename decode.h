#ifndef DECODE_H
#define DECODE_H
#include "types.h"
typedef struct
{
    char *enc_filename; //to store encode file name
    char *dec_filename; //to store decode file name from commandline argument
    char dec_magicstring[2]; //to store magicstring
    char dec_ext[5]; //to store extention name
    int dec_ext_size;   //to store extention size
    int dec_secrte_filesize;//to store encoded message size
    FILE *fptr_enc_image;//encoded file pointer
    FILE *fptr_dec_txt; //decode file pointer
}Decinfo;
//function to read and validate the command line arguments
Status read_and_validate_decode_args(int argc,char *argv[],Decinfo *decinfo);
//function to do decoding
Status do_decode(Decinfo *decinfo);
//file to open the encoded file and the file to store the decoded data
Status dec_file_open(Decinfo *decinfo);
//to check the encoded file is bmp or not
Status Is_bmp(Decinfo *dceinfo);
//to decode magic string
Status dec_magicstring(Decinfo *decinfo);
//to decode extension size
Status dec_extsize(Decinfo *decinfo);
//to decode extension
Status dec_ext(Decinfo *decinfo);
//to decode file size
Status dec_filesize(Decinfo *decinfo);
//to decode secrete message
Status dec_message(Decinfo *dceinfo);
//decode image to data
Status image_to_data(char *data,int size,FILE *fptr_image);
//to decode 8 bytes to 1 bytes
Status bytes_to_bit(char *data,char *res);
//to decode integer from image
Status image_to_integer(int *t,FILE *fptr_image);


#endif