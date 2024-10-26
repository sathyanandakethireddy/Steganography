#include <stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
// Function to validate decoding arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    int i=0;
     // Count the number of arguments
    for(i=0;argv[i]!=0;i++);
    // Check if sufficient arguments are passed
    if(i<3)
    {
        printf("Invalid arguments are passed:\n");
        return e_failure;
    }
    // Check if the file is a .bmp file
    if(!(strstr(argv[2],".bmp")))
    {
        printf("Invalid file\n");
        printf("Pass file with extension.bmp:");
        return e_failure;
    }
    // Set default output filename if not provided
    if(argv[3]==NULL)
    {
        strcpy(decInfo->output_fname,"output");
    }
    // Set output filename if provided
    if(argv[3]!=NULL)
    {
        strcpy(decInfo->output_fname,argv[4]);
    }
    // Copy the stego image filename
    strcpy(decInfo->stego_image_fname,argv[2]);
    printf("Successufly validate decoding arguments\n");
   
    return e_success;
}
// Function to open the stego image file
Status opende_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Return failure if file couldn't be opened
    if(decInfo->fptr_stego_image == NULL)
    {
        return e_failure;
    }
    return e_success;
}
// Main decoding function
Status do_decoding(DecodeInfo *decInfo)
{
    // Open stego image file
    if(opende_files(decInfo) == e_failure)
    {
        printf("Error found\n");
        return 0;
    }
    // Skip the BMP header
   if(skip_bmp_header(decInfo->fptr_stego_image))
   {
       printf("Error found\n");
       return 0;
   }
   printf("Skipping bmp header is success\n");
   // Decode the magic string length
   if(decode_magic_stringlen(decInfo))
   {
       printf("Error\n");
       return 0;
   }
   printf("Decoding magic string length is success\n");
   // Decode the magic string
  if(decode_magic_string(decInfo->magic_string,decInfo))
  {
      printf("Error found \n");
      return 0;
  } 
  printf("Decoding magic string is success\n");
   // Ask user to input the secret code
  char str[100];
  printf("Enter the secret code ");
  scanf(" %[^\n]",str);
  // Compare the input string with the magic string
  if(strcmp(str,decInfo->magic_string))
  {
      printf("Enter valid string for decode:");
      return 0;
  }
    // Decode the secret file extension length
  if(decode_secret_ext_len(decInfo))
  {
      printf("Error found\n");
      return 0;
  }
  printf("Decoding secret length is success\n");
  // Decode the secret file extension
  char file_extn[100];
   if(decode_secret_file_extn(file_extn,decInfo))
   {
       printf("Error found");
       return 0;
   }
   printf("Decode secret file extension is done\n");
    // Append the file extension to the output file name
   strcat(decInfo->output_fname,file_extn);
    decInfo->fptr_output_file = fopen(decInfo->output_fname, "w");
    // Do Error handling
    if (decInfo->fptr_output_file == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

        return e_failure;
    }
     // Decode the secret file size
   if(decode_secret_file_size(decInfo))
   {
       printf("Error found");
       return 0;
   }
   //printf("%d",decInfo->sec_file_size);
  if(decode_secret_file_data(decInfo))
  {
      printf("Error found");
      return 0;
  }

}
// Function to skip the BMP header
Status skip_bmp_header(FILE *fptr_stego_image)
{
    // Move file pointer to skip the header
    fseek(fptr_stego_image,54,SEEK_SET);
    return e_success;
}
// Function to decode the magic string length
Status decode_magic_stringlen(DecodeInfo *decInfo)
{
    char magic_len[32];
    // Read 32 bits for the length
    fread(magic_len,32,1,decInfo->fptr_stego_image);
   // Decode the length 
    decode_byte_to_int(&decInfo->magic_string_len,magic_len);
    return e_success;
}
// Function to decode 32-bit integer from image data
Status decode_byte_to_int(int *data, char *image_buffer)
{
    int mask;
    *data=0;
    // Loop through each bit to decode the integer
    for(int i=0;i<32;i++)
    {
     mask=image_buffer[i]&1;
     *data=*data+(mask*((1<<(31-i))));
    }
}
// Function to decode a byte from LSB (Least Significant Bit)
Status decode_byte_to_lsb(char *data, char *image_buffer)
{
    char mask;
    int sum=0;
     // Loop through 8 bits to decode a character
    for(int i=0;i<8;i++)
    {
     mask=image_buffer[i]&1;
     sum=sum+(mask*((1<<(7-i))));
    }
    *data=(char)sum;
}
// Function to decode the magic string
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo)
{
    int j=decInfo->magic_string_len;
    char buffer[8];
    char ch;
    int i=0;
     // Read and decode each character of the magic string
    for(i=0;i<j;i++)
    {
    fread(buffer,8,1,decInfo->fptr_stego_image);
    decode_byte_to_lsb(&ch,buffer);
    magic_string[i]=ch;
    }
     // Null terminate the string
    magic_string[i]=0;
    return e_success;
}
// Function to decode the secret file extension length
Status decode_secret_ext_len(DecodeInfo *decInfo)
{
    char file_ext_len[32];
    // Read 32 bits for the extension length
    fread(file_ext_len,32,1,decInfo->fptr_stego_image);
    // Decode the length
    decode_byte_to_int(&decInfo->file_exten_size,file_ext_len);
    return e_success;
}
// Function to decode the secret file extension
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
    int j=8*decInfo->file_exten_size;
    char buffer[8];
    char ch;
    int i=0;
     // Decode each character of the file extension
    for(i=0;i<j;i++)
    {
    fread(buffer,8,1,decInfo->fptr_stego_image);
    decode_byte_to_lsb(&ch,buffer);
    file_extn[i]=ch;
    }
    // Null terminate the file extension
    file_extn[i]=0;
    return e_success;
}
// Function to decode the secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char file_data_len[32];
    // Move file pointer to secret file size location
    fseek(decInfo->fptr_stego_image,166,SEEK_SET);
    // Read the size
    fread(file_data_len,32,1,decInfo->fptr_stego_image);
    // Decode the file size
    decode_byte_to_int(&decInfo->sec_file_size,file_data_len);
    return e_success;
}
// Function to decode the secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{

    int j=decInfo->sec_file_size;
    // Move file pointer to the data location
    fseek(decInfo->fptr_stego_image,198,SEEK_SET);
    char buffer[8];
    char ch;
    int i=0;
    // Read and decode each byte of the file data
    for(i=0;i<j;i++)
    {
    fread(buffer,8,1,decInfo->fptr_stego_image);
    decode_byte_to_lsb(&ch,buffer);
    putc(ch,decInfo->fptr_output_file);
    }
    return e_success;
}
