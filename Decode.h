#ifndef DECODE_H
#define DECODE_H

#include "types.h"  // Contains user-defined types
#include <stdio.h>  // Contains the definition for FILE
#include <string.h>

/* 
 * Structure to store information required for
 * decoding the secret file from the source Image.
 * Info about output and intermediate data is also stored.
 */
typedef struct _DecodeInfo
{
    //Name of the steo image file
    char stego_image_fname[100];    
    //File pointer for the stego image
    FILE *fptr_stego_image;        
    //length of magic string
    int magic_string_len;           
    //size of secret file extension
    int file_exten_size;            
    //placeholder for data
    int data;                       
    //magic string used in encoding
    char magic_string[100];         
    //size of the secret file
    int sec_file_size;              
    /* Stego Image Info */
    //output file name
    char output_fname[100];         
    //file pointer for output file
    FILE *fptr_output_file;         

} DecodeInfo;

/* Function prototypes */
OperationType check_operation_type(char *argv[]);
/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);
/* Get File pointers for input and output files */
Status opende_files(DecodeInfo *decInfo);
/* Skip bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image);
/* Read magic string length */
Status decode_magic_stringlen(DecodeInfo *decInfo);
/* Store Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);
/* Decode secret file extension length */
Status decode_secret_ext_len(DecodeInfo *decInfo);
/* Decode secret file extension */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo);
/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);
/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);
/* Decode byte to least significant bit */
Status decode_byte_to_lsb(char *data, char *image_buffer);
/* Decode byte to integer */
Status decode_byte_to_int(int *data, char *image_buffer);

#endif // DECODE_H
