/*
Name :Arun Cyriac
Date: 28-10-2024
Discription:
LSB Steganography is indeed a technique for concealing sensitive data within an ordinary-looking object, making it 
appear unremarkable to anyone who comes across it. Unlike cryptology, which focuses on encrypting messages to 
protect their content, steganography aims to prevent detection of the information itself. By embedding data in 
a way that goes unnoticed, steganography effectively reduces the likelihood of unauthorized access or decryption,
as there is no indication that any hidden information exists. This subtlety is what distinguishes it from
traditional methods of securing data.Here we are encoding in .bmp file.

In this project we pass '-e' as command line argument for encoding and '-d' for decoding along with the other file.
For encoding the command line argument should be in './a.out -e [source image] [secret data file] [destination file]'.
For decoding the command line argument should be in './a.out -e [source image] [secret data file]'.
*/
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
int main(int argc, char *argv[])
{
    char dummyfile_name[20] = "text";
    //argc =  5 including a.out
    //1check whether in encode(-e) or decode(-d)
    if(argc == 1)
    {
        printf("INFO: Please pass valid arguments.\n\nINFO: Encoding minimun 4 arguments.\nUsage:./a.out -e source image file secret_data_file [Destination_image_file]\nINFO: Decoding Minimum 3 arguments.\nUsage: /a.out -d encoded_image [output_file_name]\n");
        return 0;
    }
    OperationType op_type = check_operation_type(argv[1],argc);
    if(argc == 2)
    {
        return 0;
    }

    //1 check op_type e_code
    //2 if yes-> start encode,no ->goto step 3
    //3 check op_type e_dcode
    //4 if yes decode,no step 5
    //5 print error message
    EncodeInfo encInfo;
    Decinfo decinfo;
    decinfo.fptr_dec_txt = NULL;
    /*Status sat = read_and_validate_encode_args(argv, &encInfo);
    if(sat == e_failure)
    {
        //invlaid file formats
        printf("Invalid file format");
        return e_failure;
    }
    else
    {
        //file name are in correct format
        //printf("\n%s\t %s\t %s",encInfo.src_image_fname,encInfo.secret_fname,encInfo.stego_image_fname);
        sat = do_encoding(&encInfo);

    }*/
    
    if(op_type == e_encode)
    {
        //if encoding selected
        
        //to validate and copy the command line arguments to the structure
        if(argc == 3)
        {
             printf("INFO: Please pass valid arguments for Encoding.\nUsage:./a.out -e source image file secret_data_file [Destination_image_file]\n");
             return 0;
        }
        if(read_and_validate_encode_args(argv,argc, &encInfo) == e_failure)
        {
            printf("INFO: Please pass valid arguments for Encoding.\nUsage:./a.out -e source image file secret_data_file [Destination_image_file]\n");
            return 0;
        }
        printf("\nINFO:Encode selected\n");
        if(do_encoding(&encInfo) == e_success)
        {//if encoding is successfull
            printf("\n------------------------------\n");
            printf("\nINFO:Encoding is successfull\n");
            printf("\n------------------------------\n");
        }
        //closing all opened file
        fclose(encInfo.fptr_src_image);
        fclose(encInfo.fptr_secret);
        fclose(encInfo.fptr_stego_image);

    }
    else if(op_type == e_decode)
    {//if decoding is selected
        printf("INFO:Decode selected\n\n");
        if(read_and_validate_decode_args(argc,argv,&decinfo) == e_success)
        {//if validation and copying command line arguments successfull
            if(argc == 3)
            {
                decinfo.dec_filename = dummyfile_name;
            }
            if(do_decode(&decinfo) == e_success)
            {//if decoding is successfull
                printf("\n----------------------------\n");
                printf("\nINFO:Decoding is sucessfull\n");
                printf("\n-----------------------------\n");
                
            }
            else 
            {
                printf("ERROR:Failed to decode.\n");
            }
            //closing all open files
            fclose(decinfo.fptr_enc_image);
            fclose(decinfo.fptr_dec_txt);
        }
        else
        {
            
            printf("\nERROR:Invalid file type\n");
        
        }
    }
    else
    {
        
        return 0;
    }
    

}
OperationType check_operation_type(char *argv,int argc)
{
    //1 compare argv with -e
    //2 if yes  ->return e_encode,no - goto step 3
    //3 compare argv with -d
    //4 if yes->return e_decode,no goto step 5
    //5 return e_unsupported
    if(strcmp(argv,"-e") == 0)
    {
        if(argc == 2)
        {
            printf("INFO: Please pass valid arguments for Encoding.\nUsage:./a.out -e source image file secret_data_file [Destination_image_file]\n");
        }
        //printf("encoded sel");
        return e_encode;
    }
    else if(strcmp(argv,"-d") == 0)
    {
        if(argc == 2)
        {
            printf("INFO: Please pass valid arguments for Decoding.\nUsage: ./a.out -d encoded_image [output_file_name]\n");
        }
        //printf("decoded sel");
        return e_decode;
    }
    else
    {
        printf("\nERROR:Unsupported operation type.\n\n");
        printf("INFO: Please pass valid arguments.\n\nINFO: Encoding minimun 4 arguments.\nUsage:./a.out -e source image file secret_data_file [Destination_image_file]\n\nINFO: Decoding Minimum 3 arguments.\nUsage: /a.out -d encoded_image [output_file_name]\n");
        //printf("%s",argv);
        return e_unsupported;
    }
}