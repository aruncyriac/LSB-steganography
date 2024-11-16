#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
Status read_and_validate_decode_args(int argc,char *argv[],Decinfo *decinfo)
{
   
//function to read and validate the command line arguments
    if(argc == 4 || argc == 3)
    {
        if(strstr(argv[2],".bmp") == NULL)
        {     //checking argv[2] is in .bmp format
            //printf("\n%s\n",argv[2]);
            printf("\nERROR:Encoded file is not .bmp format\n\n");
            return e_failure;
        }
        if((strstr(argv[2],".txt") == NULL) && (argc == 4))
        {
            return e_failure;
        }
        {
            //copy the command line arguments to the structure
            decinfo->enc_filename = argv[2];
            decinfo->dec_filename = argv[3];
            return e_success;

        }
    }
    else 
    {
        return e_failure;
    }
    if(argc == 3)
    {
        decinfo->enc_filename = argv[2];
        decinfo->dec_filename = "text";
       
        return e_success;
    }
}
Status dec_file_open(Decinfo *decinfo)
{
    //funtion to open the .bmp file
    decinfo->fptr_enc_image = fopen(decinfo->enc_filename,"r");
    if(decinfo->fptr_enc_image == NULL)
    {
        return e_failure;
    }
    //setting the file pointer to the 54 postion excluding the header
    fseek(decinfo->fptr_enc_image,54,SEEK_SET);
    printf("INFO:Encoded File open sucessfully..\n\n");
    return e_success;
}
Status dec_magicstring(Decinfo *decinfo)
{
    //function to decode the magic string
    if(image_to_data(decinfo->dec_magicstring,2,decinfo->fptr_enc_image) == e_success)
    {
        //if decoding the magic string is successfull validate the magic string
        if(strstr(decinfo->dec_magicstring,"#*") == NULL)
        {
            printf("ERROR:Magic string not matched\n\n");
            return e_failure;
        }
        
        return e_success;
    }
}
Status dec_extsize(Decinfo *decinfo)
{
    //to decode ext size
    if(image_to_integer(&decinfo->dec_ext_size,decinfo->fptr_enc_image) == e_success)
    {
        
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status dec_ext(Decinfo *decinfo)
{
    //to decode the extention
    if(image_to_data(decinfo->dec_ext,decinfo->dec_ext_size,decinfo->fptr_enc_image) == e_success)
    {
        
        //if decoding the exten sucessful join the extention with dec_filename
        strcat(decinfo->dec_filename,decinfo->dec_ext);
        //open the file to store the decoding value
       
        decinfo->fptr_dec_txt = fopen(decinfo->dec_filename,"w");
        //file error handling
        if(decinfo->fptr_dec_txt == NULL)
        {
            printf("%s",decinfo->dec_ext);
            return e_failure;
        }
        printf("\nINFO:Decodeing file open sucessfully\n\n");
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status dec_filesize(Decinfo *decinfo)
{
    //function to decode the file size
    if(image_to_integer(&decinfo->dec_secrte_filesize,decinfo->fptr_enc_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status dec_message(Decinfo *dceinfo)
{
    /*char temp[8],data;
    for(int i = 0; i < dceinfo->dec_secrte_filesize;i++)
    {
        fread(temp,8,1,dceinfo->fptr_enc_image);
        if(bytes_to_bit(temp,&data) == e_success)
        {
            printf("%c",data);
            fprintf(dceinfo->fptr_dec_txt,"%c",data);
        }
        else
        {
            return e_failure;
        }*/

       //function to decode the secret massage
       //decalration of array of size of filesize
       //+1 is for '\0'
       char data[dceinfo->dec_secrte_filesize + 1];
       //decode the massage to data
       if(image_to_data(data,dceinfo->dec_secrte_filesize,dceinfo->fptr_enc_image) == e_success)
       {
            //if success write the decoded data to the new file
            data[dceinfo->dec_secrte_filesize] = '\0';//adding the null character to the character array
            //entering the decoded message into new file
            fprintf(dceinfo->fptr_dec_txt,"%s",data);
            return e_success;
       }
       else
       {
            return e_failure;
       }
    
}
Status do_decode(Decinfo *decinfo)
{   //to do decoding
    if(dec_file_open(decinfo) == e_success)
    {   //if file open successfull
        //printf("\nINFO:Files open sucessfully");
        if(dec_magicstring(decinfo) == e_success)
        {  
            printf("INFO:Decoded Magic String\n\n");
             //decoding magic string is successfull
            if(dec_extsize(decinfo) == e_success)
            {   
                printf("INFO:Decoded Extn size\n");
                //decoding ext size is successfull
                if(dec_ext(decinfo) == e_success)
                {   
                    printf("INFO:Decoded Extn\n\n");
                    //decoding ext is successfull
                    if(dec_filesize(decinfo) == e_success)
                    {   //decoding file size is successfull
                        printf("INFO:Decoded size\n\n");
                        if(dec_message(decinfo) == e_success)
                        {
                            printf("INFO:Decoded Message \n\n");
                            //decoding message is successfull
                            return e_success;
                        }
                        else
                        {
                            printf("ERRORDecoding screte message Failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("ERROR:Decoding file size Failed\n");
                        return e_failure;

                    }
                }
                else
                {
                    printf("ERROR:Dcoding extention failed\n");
                    return e_failure;
                }
            }
            else
            {
                printf("ERROR:Decoding extension size failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("ERROR:Decoding magic string failed\n");
            return e_failure;
        }

    }
    else
    {
        printf("ERROR:Failed to open files\n");
        return e_failure;
    }
}
Status image_to_data(char *data,int size,FILE *fptr_image)
{   //function to decode image to data
    char temp[8];
    int i;
    for(i = 0;i < size;i++)
    {   //read 1byte from the image
        fread(temp,8,1,fptr_image);
        //decode the lsb bit 1 byte
        if(bytes_to_bit(temp,&data[i]) == e_failure)
        {
            printf("\nDecode unsucessfull");
            return e_failure;
        }
    }
   return e_success;
}
Status bytes_to_bit(char *data,char *res)
{   //function to decode 8bytes to 1 bytes
    char temp = 0;
    for(int i = 0;i < 8;i++)
    {//getting the lsb bits
        if(data[i] & 1)
        {//copy it to temp
            temp = (temp << 1) | 1;
        }
        else
        {
            temp = (temp << 1) & (~1);
        }
    }
    *res = temp;
    return e_success;
}
Status image_to_integer(int *t,FILE *fptr_image)
{//decoding the image to innteger
    int temp = 0;
    //to store store 32 bit
    char data[32];
    //read 32 bytes
    fread(data,32,1,fptr_image);
    for(int i = 0;i < 32;i++)
    {//geting the lsb bit and storing it in temp
        if(data[i] & 1)
        {
            temp = (temp << 1) | 1;
        }
        else
        {
            temp = (temp << 1) & (~1);
        }
    }
    *t = temp;
    return e_success;
}