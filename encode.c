#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET); //file pointer to 18 pos

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image); //read 4 bytes
    printf("\nINFO:Width = %u\n\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image); //read next 4 bytes
    printf("INFO:Hight = %u\n\n", height);

    // Return image capacity
    return width * height * 3; //width*height is in pixel so multipy with 3
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");//prints actual error message
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status read_and_validate_encode_args(char *argv[],int argc, EncodeInfo *encInfo)
{
    //1.check .bmp passed .txt passed .bmp passed in order in argv
    //2.if yes->
    //store all arguments into respective structure memner and return e_succuss ,
    //no ->return e_failure
    if(strstr(argv[2],".bmp") == NULL)
    {
        //first argv not in .bmp format
        return e_failure;
    }
    if(strstr(argv[3],".txt") == NULL)
    {
        
        //second argv not in .txt
        return e_failure;
    }

    if(argc == 4)
    {
       
        encInfo->src_image_fname = argv[2];
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file,strstr(encInfo->secret_fname,"."));
        encInfo->stego_image_fname = "Output.bmp";
        return e_success;
        
    }
    if(argc == 5)
    {
        if(strstr(argv[4],".bmp") == NULL)
        {
            return e_failure;
        }
     
        //copy command line arguments to the structure
        encInfo->src_image_fname = argv[2];
        encInfo->secret_fname = argv[3];
        encInfo->stego_image_fname = argv[4];

        //printf("%s",encInfo->extn_secret_file);
        return e_success;

    }
    

}
uint get_file_size(FILE *fptr)
{
    //Function to return the size of the secret file
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}
Status check_capacity(EncodeInfo *encInfo)
{
   
    //1.magic string here #*
    //2.encode secrete file size
    //3.encode extention type
    //4.size secrete massege
    //5.secrete massage
    //6.capacity = 16 + 32 + (size of exetention * 8) + 32 + (size of data *8) + 54 + EOF;e
    //algoritham
    //1.find the size of image file
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    //2.find the extension type and its size
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if(encInfo->image_capacity > (16 + 32 + encInfo->size_secret_file*8 + 32 +encInfo->size_secret_file + 1) ) //+1 for EOF
    {
        printf("INFO:Secret file size is %ld\n\n",encInfo->size_secret_file);
        return e_success;
    }
    else
    {
        return e_failure;
    }
    //3.find the size of the secrete data
    //4check if image size is greater than the capacity,yes->return success,no->return failure.

}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //1.use rewind funtion change the file pointer to start
    rewind(fptr_src_image);
    char temp[54];
    if((fread(temp,1,54,fptr_src_image) == 54 ) && (fwrite(temp,1,54,fptr_dest_image) == 54))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
    //2.read 54 bytes of data from source image
    //return e_success or e_failaure using the return type of fread and fwrite
    //3.write the data to destination
}

//call encode magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    //encoding data into bytes
    if(encode_data_to_image(magic_string, 2, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
    
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
      //encoding data into bytes
    if(encode_data_to_image(file_extn,4,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_secret); //set secret file pointer to the begining
    char data[(int) encInfo->size_secret_file]; //decalation of tempory storage
	fread(data, sizeof(char), (uint) encInfo->size_secret_file, encInfo->fptr_secret); //read the bytes from .bmp file and store it in data
    //printf("%s",data);
	/*if (ferror(encInfo->fptr_secret))
	{
		fprintf(stderr,"Error: While reading the data to secret file\n");
		clearerr(encInfo->fptr_secret);
		return e_failure;
	}*/
   
      //encoding data into bytes
    if(encode_data_to_image(data,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
    {
        
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    //function to copy the remaining bytes to the new .bmp
    char ch;
    while(fread(&ch,1,1,fptr_src)>0)
    {
        if(fwrite(&ch,1,1,fptr_dest) == 0)
        {
            return e_failure;
        }
    
    }
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    //1.open files
    if(open_files(encInfo) == e_success)
    {
        printf("\nINFO:All files are open successfully\n");
        //2.we need call check capacity function to check size is with encode image
        if(check_capacity(encInfo) == e_success)
        {
            printf("INFO:Capacity checked\n\n");
            //if success copy the header to the new file
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
            {
                printf("INFO:Header file copied to new file\n\n");
                //if success encode magic string
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("INFO:Magic String encoded successfully\n\n");
                    //if success ext size
                    if(encode_secret_file_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
                    {
                        printf("INFO:Extension size encoded successfully\n\n");
                        //if success encode ext
                        if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                        {
                            printf("INFO:Extention encoded successfully\n\n");
                            //if success encode secret message size
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
                            {
                                printf("INFO:Secret file size encoded\n\n");
                                //if success encode message
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("INFO:Message size encoded\n\n");
                                    //if success encode copy the remaining bytes to
                                    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf("INFO:Copied remaining data\n");
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("\nERROR:Copying the remaining data failed\n");
                                    }
                                }
                                else
                                {
                                    printf("\nERROR:Encoding secrete data failed\n");
                                }
                            }
                            else
                            {
                                printf("\nERROR:Encoding file type failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("\nERROR:Encoding secret file extn failed\n");
                            return e_failure;
                        }

                    }
                    else
                    {
                        printf("\nERROR:Enoding failed at encode extention file type size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("\nERROR:Encode Fails at coping the magic string\n");
                    return e_failure;
                }

            }
            else
            {
                printf("\nERROR:Encoding Failed in Coping the header part\n");
                return e_failure;
            }
        }
        else
        {
            printf("\nError:Secrete Code Exceeds the Encoding Limit\n");
            return e_failure;
        }
        
    }
    else
    {
        printf("\nError:File not open\nS");
        return e_failure;
    }


}
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //to encode data to lsb of the .bmp bytes
    char temp[8];
    for(int i = 0;i < size;i++)
    {
        //read 8 bytes from the image
        fread(temp,8,1,fptr_src_image);
        //encode bits to lsb of bytes
        if(encode_byte_to_lsb(data[i], temp) == e_success)
        {
            //if success write the encoded data into new file
            fwrite(temp, 8, 1,fptr_stego_image);
        }
        else
        {
            return e_failure;
        }
    }
    return e_success;
    //read 8 bytes of data from sourse file and store it in array
    //pass to encode_bytes_to_lsb
    //write the encoded data to destnatin file.
    //repeat for length of the data

}
Status encode_secret_file_size(uint size,EncodeInfo *encInfo)
{
    //encode integer to the image
    char temp[32];
    int n = 31;
    //read 32 bytes from .bmp file
    fread(temp,32,1,encInfo->fptr_src_image);
    for(int i = 0; i < 32; i++)
    {
        //encode the bits to lsb of bytes
        temp[i] = temp[i] & (~1);
        if(size & (1<<n))
        {
            temp[i] = temp[i] | 1;
        }
        n--;
    }
    if(n == -1)
    {
        //if all bits is encode write the encoded data to the new .bmp file
        fwrite(temp,32,1,encInfo->fptr_stego_image);
        return e_success;
    }
    else
    {
        return e_failure;
    }



}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    //encoding the bits to bytes
    int n = 7;
    for(int i = 0; i < 8;i++)
    {
        //put the bits to lsb of the bit
        image_buffer[i] = image_buffer[i] & (~1);
        if(data & (1<<n))
        {
            image_buffer[i] = image_buffer[i] | 1;
        }
        n--;
    }
    if(n == -1)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
    //do encoding
    //clear lsb of arr[i]
    //get one bit from data
    //put it in lsb

    //repeat this process for 8 times
}

