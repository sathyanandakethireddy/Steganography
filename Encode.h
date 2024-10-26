#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    //Name for source image
    char *src_image_fname;
    //File pointer for source image
    FILE *fptr_src_image;
    uint sec_size;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    //Name of the secret file
    char *secret_fname;
    //File pointer for secret file
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    //Name for stego Image
    char *stego_image_fname;
    //File pointer for stego image
    FILE *fptr_stego_image;

} EncodeInfo;

/*
/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);
/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int args,char *argv[], EncodeInfo *encInfo);
/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);
/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);
/* check capacity */
Status check_capacity(char *magic_str,EncodeInfo *encInfo);
/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);
/* Get file size */
uint get_file_size(FILE *fptr);
/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);
// Encode Magic String Size
Status encode_magic_string_size(int magic_str_length,EncodeInfo *encInfo);
/* Encode Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);
/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);
//Encode Secret file extension
Status encode_secret_file_extn_size(int file_extn_size, EncodeInfo *encInfo);
/* Encode secret file size */
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo);
/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);
/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);
/* Encode a byte into LSB of image data array */
//Status encode_byte_to_lsb(char data, char *image_buffer);
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);
//Encode byte to lsb
Status encode_byte_to_lsb(char data, char *image_buffer);
//Encode int lsb
Status encode_int_to_lsb(int size,char *buffer);

#endif
