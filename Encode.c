#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>

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
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
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
    	perror("fopen");
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
//Call the check_capacity function with arguments magic string and structure address encInfo
Status check_capacity(char *magic_str,EncodeInfo *encInfo)
{
   //store the src_image size in bmp_size
    long bmp_size = get_image_size_for_bmp(encInfo->fptr_src_image);
    //set fseek at the end ,pointer will point to last byte
    fseek(encInfo->fptr_secret, 0, SEEK_END);
    //store secret file size in secret_size variable
    long secret_size = ftell(encInfo->fptr_secret);
    encInfo->sec_size = secret_size;
    //point fptr_secret to beginning
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    //store magic string length as 4
    int magic_string_length=4;
    //store magic_string_size ,
    int magic_string_size=strlen(magic_str);
    //store file_exten_length as 4
    int file_exten_length=4;
    //Declare exten pointer to store address of .
    char *exten = strstr(encInfo->secret_fname, ".txt");
    //file_exten_size store size of exten
    int file_exten_size=strlen(exten);
    //declare secre_file_length variable and store its length
    int secret_file_length=4;
    //Declare total_bits ,which store sum of all legth,size,of magic string,exten ,file data,file size and including header 54 bytes
    int total_bits=(magic_string_length+magic_string_size+file_exten_length+file_exten_size+secret_file_length+secret_size)*8+54;
    //if bmp_size ,BMP image as more size than total_bits ,perform encoding,by returning e_su
    if(bmp_size > total_bits)
    {   printf("it is working\n");
     
        return e_success;
    }
    //else return not have enough capacity im BMP file
    else
    {
        printf("Error: Not enough capacity in the BMP file.\n");
        //close the pointer fptr_src_image
        fclose(encInfo->fptr_src_image);
        //close the pointer fptr_secret
        fclose(encInfo->fptr_secret);
        return e_failure;
    }

    
}
//Define do_encoding function
Status do_encoding(EncodeInfo *encInfo)
{
    //if files are open ,print files are open successfully
    if(open_files(encInfo) == e_success)
    {
        printf("Files are open successfully.\n");
    }
    //print failed to open files
    else
    {
        printf("Failed to open the files.\n");
        return e_failure;
    }
    //declare magic string
    char magic[50];
    printf("Enter the magic string:");
    //take magic string as user input
    scanf("%s",magic);
    // if capicity checked print Successfully checked the file capacity
    if(check_capacity(magic,encInfo) == e_success)
    {
        printf("Successfully checked the file capacity.\n");
    }
    //else printf failed to check the file capacity
    else
    {
        printf("Failed to check the file capacity.\n");
        return e_failure;
    }//if copy_bmp_header return e_success,print Bmp header copied successfully
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf("BMP header copied successfully.\n");
    }
    //print fialed to copy BMP header and return e_failure
    else
    {
        printf("Failed to copy BMP header.\n");
        return e_failure;
    }
    //if encode_magic_string_size returns e_success,print Sucessfully encode magic string size
    if(encode_magic_string_size(strlen(magic),encInfo)==e_success)
    {
        printf("Sucessfully encoded magic string size\n");
    }
    //else print Failed to encode magic string size
    else
    {
        printf("Failed to encode magic string size\n");
    }
    //if encode_magic_string ,returns e_success,print the below statement
    if(encode_magic_string(magic,encInfo)== e_success)
    {
       printf("sucessfully encode magic string\n");

    }
    //else print failed to encode magic string size
    else
    {
       printf("failed to  encode magic string\n");
    }
    //encode_secret_file_extn_size return e_success,print successfully encode_secret_file_extn_size
    if(encode_secret_file_extn_size(strlen(strstr(encInfo->secret_fname,".txt")),encInfo)==e_success)
    {
       printf("sucessfully encode_secret_file_extn_size\n");
    }
    //print failed to secreat file extension
    else
    {
       printf("failed to secreat file extension\n");
    }
    //if encode_secret_file_extn return e_success,print successfully encode_secret_file_extn
    if(encode_secret_file_extn(encInfo->secret_fname,encInfo)==e_success)
    {
       printf("sucessfully encode_secret_file_extn\n");
    }
    //print failed to encode_secret_file_extn
    else
    {
       printf("failed encode_secret_file_extn\n");
    }
    //if encode_secret_file_size returns e_success ,print sucessfully encode_secret_file_size
    if(encode_secret_file_size(encInfo->sec_size,encInfo)==e_success)
    {
        printf("Sucessfully encode_secret_file_size\n");
    }
    //print failedd to encode_secret_file_size
    else
    {
        printf("failed to encode_secret_file_size\n");
    }
    //encode_secret_file_data return e_success ,print successfully encode_secret_file data
    if(encode_secret_file_data(encInfo)==e_success)
    {
        printf("Sucessfully encode_secret_file_data\n");
    }
    //else print failed to encode_secret_file_data
    else
    {
        printf("failed to encode_secret_file_data\n");
    }
    //if copy_remaining_img_data returns e_success,print successfully copy_remaining_img_data
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        printf("Sucessfully copy_remaining_img_data\n");
    }
    //else print failed to copy_remaining_img_data
    else
    {
        printf("Failed to copy_remaining_img_data\n");
    }
    

   
}
//Define copy_bmp_header with passing file pointer of src_img,steg_img
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //declare array of character to store first 54 bytes of header
    char header[54];
    //rewind function point pointer to starting address
    rewind(fptr_src_image);
    //here it fread read 54 bytes form src_img  and store it header
    fread(header,sizeof(char),54,fptr_src_image);
    //fwrite write 54 bytes from headder to stego_bmp
    fwrite(header,sizeof(char),54,fptr_dest_image);
    //if all done return e_success
    return e_success;
}
// Encode the magic string size into the stego image
Status encode_magic_string_size(int magic_str_length,EncodeInfo *encInfo)
{
    // Create a buffer of size 32 bytes
   char buff[32];
   // Read 32 bytes from the source image
   fread(buff, 32, 1, encInfo->fptr_src_image); 
   // Encode the magic string length into the LSBs
   encode_int_to_lsb(magic_str_length, buff); 
   // Write the modified buffer to the stego image
   fwrite(buff, 32, 1, encInfo->fptr_stego_image);
   // Return success if everything is done
   return e_success; 
}
// Encode the magic string itself into the stego image
Status encode_magic_string(char *magic_string,EncodeInfo *encInfo)
{
    // Create a buffer of size 8 bytes
    char buff[8];
    // Loop over each character in the magic string
    for(int i=0;i<strlen(magic_string);i++)
    {
       // Read 8 bytes from the source image
        fread(buff,8,1,encInfo->fptr_src_image);
        // Encode the character into the LSBs of the buffer
        encode_byte_to_lsb(magic_string[i],buff);
        // Write the modified buffer to the stego image
        fwrite(buff,8,1,encInfo->fptr_stego_image);
    }
    // Return success if everything is done
    return e_success;
}
// Encode the file extension size into the stego image
Status encode_secret_file_extn_size(int file_extn_size,EncodeInfo *encInfo)
{
    // Create a buffer of size 32 bytes
    char buff[32];
    // Read 32 bytes from the source image
    fread(buff,32,1,encInfo->fptr_src_image);
    // Encode the file extension size into the LSBs of the buffer
    encode_int_to_lsb(file_extn_size,buff);
    // Write the modified buffer to the stego image
    fwrite(buff,32,1,encInfo->fptr_stego_image);
    // Return success if everything is done
    return e_success;
}
// Encode the file extension into the stego image
Status encode_secret_file_extn(char *file_extn,EncodeInfo *encInfo)
{
    // Find the file extension from the secret filename
    file_extn = strstr(encInfo->secret_fname, ".");
    // Get the length of the file extension
    int len = strlen(file_extn);
    // Create a buffer of size 8 bytes
    char buff[8];
    // Loop over each character in the file extension
    for (int i = 0; i < len; i++)
    {
        // Read 8 bytes from the source image
        fread(buff, sizeof(char), 8, encInfo->fptr_src_image);
        // Encode the character into the LSBs of the buffer
        encode_byte_to_lsb(file_extn[i], buff);
        // Write the modified buffer to the stego image
        fwrite(buff, sizeof(char), 8, encInfo->fptr_stego_image);
    }
    // Return success if everything is done
    return e_success;
}
// Encode the file size into the stego image
Status encode_secret_file_size(int file_size,EncodeInfo  *encInfo)
{
    // Create a buffer of size 32 bytes
    char buff[32];
    // Read 32 bytes from the source image
    fread(buff,32,1,encInfo->fptr_src_image);
     // Encode the file size into the LSBs of the buffer
    encode_int_to_lsb(file_size,buff);
    // Write the modified buffer to the stego image
    fwrite(buff,32,1,encInfo->fptr_stego_image);
    // Return success if everything is done
    return e_success;
}
//Define encode_secret_file_data,here encode the secret file data to stego_image
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    char buff[8];
     // Reset secret file pointer to the beginning
    rewind(encInfo->fptr_secret);
    // Read each byte of the secret file
    while((ch=fgetc(encInfo->fptr_secret))!=EOF)
    {
        // Read 8 bytes from the source image
        fread(buff,8,1,encInfo->fptr_src_image);
        // Encode the secret byte into the LSBs of the buffer
        encode_byte_to_lsb(ch,buff);
        // Write the modified buffer to the stego image
        fwrite(buff,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
//Define Encode_byte_to_lsb,
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        // Clear the LSB of image_buffer[i] and set it to the corresponding bit of the data
	    image_buffer[i]=(image_buffer[i]&0xFE)|((data & (1<<7-i))>>7-i);
    }
    return e_success;
}
//Define encode_int_to_lsb
Status encode_int_to_lsb(int size,char *buffer)
{
    int j=0;
    for(int i=31;i>=0;i--)
    {
        // Extract the i-th bit of size and store it in the buffer
        j=((uint)(size&(1<<i)))>>i;
        // Clear LSB of buffer byte
        buffer[31-i]=buffer[31-i]&(0XFFFFFFFE);
        // Set the LSB to the corresponding bit of the size
        buffer[31-i]=buffer[31-i] | j;
        j++;
    }
   
}
// Copy remaining image data from source image to destination image
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    // Read from source image and write to destination image byte by byte until EOF
    while( fread(&ch,1,1,fptr_src) )
    {
        // Write the byte to the destination image
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}
   

