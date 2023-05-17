
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h> /* for the function ntohs() */
#include "jpeg_reader.c"
#include "bitstream.c"
int main(int argc,char** argv){
    /* If there is no image.jpeg input : USAGE ERROR*/
    if (argc!=2){
        fprintf(stderr,"Usage: %s fichier.jpeg\n",argv[0]);
        return EXIT_FAILURE;
    }
    /* Check if the image is in jpeg or jpg format */
    size_t len = strlen(argv[1]);
    if (len < 5) {
        /* the string is too short to end with ".jpeg" or ".jpg" */
        return EXIT_FAILURE;
    }
    /* Return Error in this case */
    const char* suffix1 = argv[1] + len - 4;
    const char* suffix2 = argv[1] + len - 5;
    if (!(strcmp(suffix2, ".jpeg") == 0 || strcmp(suffix1, ".jpg") == 0)){
        fprintf(stderr,"Wrong Format: PLEASE INSERT JPEG IMAGE\n ");
        return EXIT_FAILURE;
    }
    FILE *jpeg_image= fopen(argv[1],"r");
    struct HEADER* header=calloc(1,sizeof(struct HEADER));
    extract_header(header,jpeg_image);
    uint8_t pos;
    fread(&pos,sizeof(uint8_t),1,jpeg_image);
    /*printf("path should be 111111110 and is %s \n",header->dhts->dht_table[0]->tree->right->right->right->right->right->right->right->right->left->c);
    printf("symbol should be b and is %x \n",header->dhts->dht_table[0]->tree->right->right->right->right->right->right->right->right->left->S);
    printf("f %x \n",(0xfe)>>4);
    printf("e %x \n",(0xfe)&(0x0f));*/
    /*for (int i = 0; i < 16; i++) {
    printf(",%d ", header->dhts->dht_table[1]->symbols_number[i]);}*/

    create_stream(jpeg_image);
    
    /*for (int i = 0; i < header->dhts->dht_table[1]->symbols_number_total; i++) {
    printf("path :%s \n", header->dhts->dht_table[1]->paths[i]);}*/
    fclose(jpeg_image);
    free_header(header);
    return EXIT_SUCCESS;
    //free MCU_lis;
}