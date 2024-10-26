#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>
//prototype of check_operation_type
OperationType check_operation_type(char *argv[]);
//prototype get_image_size_for_bmp
uint get_image_size_for_bmp(FILE *fptr_image);
//prototype open_files
Status open_files(EncodeInfo *encInfo);
//prototype for read_and_validate_encode_args
Status  read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo);
//prototype for check_capacity
Status check_capacity(char *magic_str,EncodeInfo *encInfo);
//prototype foe copy_bmp_header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);
//prototype encode_magic_string_size
Status encode_magic_string_size(int magic_str_length,EncodeInfo *encInfo);
//prototype for int_to_lsb
Status int_to_lsb(int data, char *image_buffer);
//prototype for encode_byte_to_lsb
Status encode_byte_to_lsb(char data, char *image_buffer);
int main(int argc,char *argv[])
{
    //check_poertion_type for encoding
   if(check_operation_type(argv) == e_encode)
    {   
        EncodeInfo encInfo; 
        printf("Selected encoding\n");
         // Validate the arguments for encoding
        if(read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
        {
           // Successful validation message
           printf("File type \"-e\" reading is successful\n");
           // Call the encoding function
           do_encoding(&encInfo);
        }
        else
        {
           // Unsuccessful validation message
           printf("File type \"-e\" reading is unsuccessful\n");
        }
    }
    //check_operation_type for decoding
    if(check_operation_type(argv)==e_decode)
    {
        DecodeInfo decInfo;
        printf("Selected decoding\n");
        // Validate the arguments for decoding
        if(read_and_validate_decode_args(argv,&decInfo)==e_success)
        {
            // Successful validation message
            printf("File type \"-d\" reading is successful\n");
            // Call the decoding function
            do_decoding(&decInfo);
        }
        else
        {
            // Unsuccessful validation message
            printf("File type \"-d\",reading is unsuccessful\n");
        }
    }
     return 0;
}
//check_operation_type for either encoding or decoding based on command line arguments
OperationType check_operation_type(char *argv[])
{
    //if argv[1]=='-e',do encoding
    if(strcmp(argv[1],"-e")==0)
    {
        // Return encoding action
	    return e_encode;
    }
    //if argv[1]=='d',do decoding
    else if(strcmp(argv[1],"-d")==0)
    {
        // Return decoding action
	    return e_decode;
    }
    else
    {
         // Return unsupported action for any other input
	    return e_unsupported;
    }
}
//define read_and_validate_encode_args
Status  read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo)
{
    //if argc==4 or argc ==5 
if(argc == 4 || argc == 5) 
{
    
    char *add = strrchr(argv[2], '.');
    //check if file extent is .bmp
    if(add != NULL && strcmp(add, ".bmp") == 0) 
    {
        //assign argv[2] as src image
        encInfo->src_image_fname = argv[2];
        char *secr = strrchr(argv[3], '.');
        // Find the last occurrence of '.' in argv[3]
        if(secr != NULL && strcmp(secr, ".txt") == 0) 
        {
            // Assign argv[3] as the secret filename
            encInfo->secret_fname = argv[3];
            if(argv[4] != NULL) 
            {
                // Assign argv[4] as the stego image filename
                encInfo->stego_image_fname = argv[4];
            } 
            else 
            {
                // Default stego filename to 'output.bmp'
                encInfo->stego_image_fname = "output.bmp";
            }

            // Return success if all conditions are met
            return e_success;
        } 
        else 
        {
            // Error message for invalid secret file extension
            printf("Invalid secret file extension. Expected .txt\n");
            // Return failure if secret file extension is not '.txt'
            return e_failure;
        }
    } 
    else 
    {
        // Error message for invalid BMP file extension
        printf("Invalid bmp file name. Expected .bmp\n");
        // Return failure if source file extension is not '.bmp'
        return e_failure;
    }
} 
else 
{
    // Error message for incorrect number of arguments
    printf("Invalid number of arguments\n");
    // Return failure if the number of arguments is incorrect
    return e_failure;
}

}
