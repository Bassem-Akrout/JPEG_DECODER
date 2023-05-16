#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

uint8_t SATURATED(float val){
    if (val < 0){ return 0; }
    if (val > 255){ return 255;}
    else return (uint8_t) floor(val+0.5);
}

void one_YCbCr_mcu_to_rgb(/*struct MCU* mcu_ycbcr,*/uint8_t** mcu_rgb,int nb_horz_blocks_in_mcu,int nb_vertical_blocks_in_mcu){
    int nb_blocks=nb_horz_blocks_in_mcu*nb_vertical_blocks_in_mcu;
    /*uint8_t Cb[2][8][8]={{{ 0x7B, 0x85, 0x88, 0x69, 0x43, 0x4A, 0x40, 0x4B},{ 0x84, 0x82, 0x6F, 0x36, 0x3E, 0x3B, 0x56, 0x55},{0x89, 0x5F, 0x44, 0x38, 0x66, 0x4D, 0x3E, 0x4E},{ 0x81, 0x79, 0x3F, 0x46, 0x6D, 0x4C, 0x46, 0x4B}
    ,{ 0x81, 0x66, 0x43, 0x40, 0x79, 0x74, 0x68, 0x72},{0x77, 0x80, 0x65, 0x67, 0x73, 0x71, 0x77, 0x73},{0x87, 0x63, 0x68, 0x7C, 0x77, 0x78, 0x71, 0x81},{0x72, 0x55, 0x7F
, 0x7A, 0x7F, 0x71, 0x83, 0x74}},{{0x44, 0x49, 0x85, 0x8B, 0x82, 0x7F, 0x84, 0x81},{ 0x37, 0x30, 0x51, 0x85, 0x8A, 0x7E, 0x81, 0x87},{
    0x49, 0x41, 0x36, 0x3A, 0x3F, 0x6A, 0x7C, 0x8C
},{ 0x45, 0x55, 0x37, 0x3E, 0x3B, 0x48, 0x75, 0x85},{  0x7B, 0x72, 0x69, 0x52, 0x46, 0x41, 0x63, 0x88},{0x77, 0x80, 0x74, 0x58, 0x52, 0x51, 0x7F, 0x7F},{ 0x83, 0x76, 0x7A, 0x5D, 0x49, 0x5B, 0x6E, 0x77},{0x7B, 0x75, 0x7C, 0x70, 0x4B, 0x63, 0x70, 0x77}}};
    uint8_t Y[2][8][8] = {
        {

            {0x70, 0x74, 0x6D, 0x99, 0xC8, 0xB7, 0xBD, 0xC3},
            {0x73, 0x74, 0x8B, 0xC9, 0xB9, 0xD6, 0x8F, 0x83},
            {0x6C, 0x9F, 0xCF, 0xC3, 0x63, 0x8F, 0xBC, 0x99},
            {0x6F, 0x62, 0xC0, 0xAB, 0x6B, 0xC3, 0xCB, 0xC9},
            {0x72, 0x7C, 0xCE, 0xB8, 0xBC, 0xC4, 0xC1, 0xC4},
            {0x73, 0x4E, 0x72, 0xA1, 0xBD, 0xC6, 0xC6, 0xC2},
            {0x71, 0x90, 0x69, 0x88, 0xBE, 0xC4, 0xC3, 0xC3},
            {0x89, 0xA2, 0x21, 0x91, 0x8D, 0xBE, 0xC6, 0xAB}
           
            
         
        },
        {  
            {0xBD, 0xBC, 0x7C, 0x74, 0x74, 0x74, 0x77, 0x72},  
            {0xCC, 0xCB, 0xBF, 0x77, 0x74, 0x77, 0x6D, 0x76},
            {0xC7, 0xD0, 0xC6, 0xCE, 0xC7, 0x9C, 0x7B, 0x73},
            {0xCC, 0x98, 0xD6, 0xD3, 0xD1, 0xD2, 0x90, 0x6F},
            {0xC9, 0xC5, 0xC6, 0xB6, 0xB9, 0xC5, 0x82, 0x7E},
            {0xC0, 0xCB, 0xC7, 0x8D, 0xC0, 0x8B, 0x4B, 0x79},
            {0xC6, 0xC5, 0xC3, 0x9A, 0xB4, 0x9C, 0x66, 0x8A},
            {0x94, 0xA2, 0x84, 0x6D, 0xA5, 0x88, 0x5B, 0x8E}
        }
    };

    uint8_t Cr[2][8][8] = {
        {
            {0xCA, 0xC6, 0xBB, 0xAD, 0xA5, 0xA0, 0x9C, 0x9A},
            {0xC6, 0xB8, 0xB1, 0xA7, 0x99, 0x98, 0x9C, 0x93},
            {0xC1, 0xA8, 0xA2, 0x9F, 0x8D, 0x91, 0x9B, 0x8F},
            {0xBE, 0xA3, 0x9A, 0x98, 0x8E, 0x92, 0x9B, 0x94},
            {0xBE, 0xA6, 0x95, 0x95, 0x98, 0x99, 0x9B, 0x9E},
            {0xBE, 0xA5, 0x91, 0x95, 0xA0, 0x9E, 0x9C, 0xA3},
            {0xBD, 0x9B, 0x8C, 0x96, 0x9D, 0x9D, 0x9E, 0x9E},
            {0xBB, 0x90, 0x87, 0x97, 0x97, 0x9A, 0x9F, 0x97},
            
        },
        {   {0x9D, 0xA8, 0xB3, 0xC7, 0xC4, 0xC3, 0xD0, 0xBF},
            {0x92, 0x9E, 0xAB, 0xBE, 0xBE, 0xBC, 0xC8, 0xBB},
            {0x99, 0x99, 0x9A, 0xA3, 0xA3, 0xAB, 0xC2, 0xC6},
            {0x98, 0x96, 0x96, 0x9B, 0x9B, 0xA0, 0xB4, 0xC1},
            {0xA1, 0x9A, 0x97, 0x95, 0x95, 0x98, 0xA7, 0xBF},
            {0xAA, 0x9F, 0x97, 0x8F, 0x8E, 0x90, 0x9E, 0xC1},
            {0x9D, 0x97, 0x9A, 0x97, 0x97, 0x91, 0x91, 0xB4},
            {0x9C, 0x93, 0x95, 0x90, 0x91, 0x8D, 0x8E, 0xB7}
            
        }
    };*/
uint8_t Y[2][8][8]={{{0x76, 0x77, 0x64, 0x9e, 0xc6, 0xb9, 0xba, 0xc3},
                    {0x6b, 0x85, 0x91, 0xb9, 0xb3, 0xba, 0x9c, 0x94},
                    {0x72, 0x87, 0xc7, 0xc7, 0x69, 0x9e, 0xa7, 0xa2},
                    {0x77, 0x75, 0xb5, 0xa4, 0x75, 0xbc, 0xc6, 0xce},
                    {0x6b, 0x75, 0xc8, 0xbc, 0xad, 0xba, 0xbe, 0xd4},
                    {0x6a, 0x59, 0x7c, 0x9f, 0xc9, 0xc9, 0xc3, 0xb7},
                    { 0x7b, 0x8d, 0x5f, 0x8f, 0xbc, 0xbd, 0xc0, 0xca},
                    {0x86, 0x96, 0x3b, 0x8a, 0x9f, 0xba, 0xbf, 0xa8}},
                    {{0xbd, 0xb6, 0x8c, 0x68, 0x73, 0x79, 0x6f, 0x78},
                    { 0xd0, 0xbf, 0xb4, 0x92, 0x78, 0x7b, 0x75, 0x71},
                    {0xc6, 0xc6, 0xd6, 0xc1, 0xb5, 0xa9, 0x7a, 0x72},
                    {0xbf, 0xaf, 0xd0, 0xd0, 0xd1, 0xcd, 0x8f, 0x75},
                    { 0xcd, 0xbb, 0xc2, 0xb4, 0xc1, 0xbe, 0x84, 0x83},
                    {0xc5, 0xd1, 0xbd, 0x9a, 0xab, 0x91, 0x56, 0x7a},
                    {0xc0, 0xca, 0xb8, 0xa2, 0xaa, 0x8e, 0x66, 0x82,},
                    {0x9b, 0xa9, 0x7f, 0x6f, 0x9a, 0x89, 0x66, 0x8e}}};
    uint8_t Cr[2][8][8] = 
    {
        {0x82, 0x82, 0x6f, 0x6f, 0x4f, 0x4f, 0x4e, 0x4e},
        {0x7c, 0x7c, 0x53, 0x53, 0x6f, 0x6f, 0x60, 0x60},
        {0x78, 0x78, 0x5f, 0x5f, 0x51, 0x51, 0x49, 0x49},
        {0x7d, 0x7d, 0x66, 0x66, 0x73, 0x73, 0x70, 0x70},
        {0x70, 0x70, 0x4c, 0x4c, 0x58, 0x58, 0x47, 0x47},
        {0x71, 0x71, 0x74, 0x74, 0x70, 0x70, 0x78, 0x78},
        {0x73, 0x73, 0x48, 0x48, 0x63, 0x63, 0x4f, 0x4f},
        {0x64, 0x64, 0x7a, 0x7a, 0x6b, 0x6b, 0x7a, 0x7a},
       
    },
    {   {0x41, 0x41, 0x74, 0x74, 0x8c, 0x8c, 0x83, 0x83},
        {0x6a, 0x6a, 0x57, 0x57, 0x4f, 0x4f, 0x78, 0x78},
        {0x3d, 0x3d, 0x5f, 0x5f, 0x75, 0x75, 0x7e, 0x7e},
         {0x7b, 0x7b, 0x6c, 0x6c, 0x58, 0x58, 0x74, 0x74},
        {0x40, 0x40, 0x49, 0x49, 0x5a, 0x5a, 0x79, 0x79},

       
        
        {0x7e, 0x7e, 0x77, 0x77, 0x5e, 0x5e, 0x6b, 0x6b},

        {0x51, 0x51, 0x46, 0x46, 0x4c, 0x4c, 0x78, 0x78},
         {0x7a, 0x7a, 0x7a, 0x7a, 0x5f, 0x5f, 0x63, 0x63}


}
    float R,G,B;
    for (uint32_t i=0;i<nb_blocks;i++){
        for (uint32_t j=0;j<8;j++){
            for (uint32_t k=0;k<8;k++){
            R = Y[i][j][k] - 0.0009267 * (Cb[i][j][k] - 128) + 1.4016868 * (Cr[i][j][k] - 128);
            G = Y[i][j][k] - 0.3436954 * (Cb[i][j][k] - 128) - 0.7141690 * (Cr[i][j][k] - 128);
            B = Y[i][j][k] + 1.7721604 * (Cb[i][j][k] - 128) + 0.0009902 * (Cr[i][j][k] - 128);
            
            mcu_rgb[i*64 + j*8 + k][0]=SATURATED(R) ;
            mcu_rgb[i*64 + j*8 + k][1]=SATURATED(G) ; 
            mcu_rgb[i*64 + j*8 + k][2]=SATURATED(B) ;
        }
    }
}
}

/*void one_Y_mcu_to_rgb(/*struct mcu* mcu_Y,uint8_t* mcu_rgb,uint32_t nb_horz_blocks_in_mcu,uint32_t nb_vertical_blocks_in_mcu){
        const uint32_t nb_blocks = nb_horz_blocks_in_mcu * nb_vertical_blocks_in_mcu;
        uint8_t g;
        uint8_t Y[1][8][8]={{{0,0,0,255,255,0,0,0},{0,0,255,255,255,255,0,0},{0,255,255,255,255,255,255,0},{255,255,0,255,255,0,255,255},{255,255,255,255,255,255,255,255},{0,0,255,0,0,255,0,0},{0,255,0,255,255,0,255,0},{255,0,255,0,0,255,0,255}}};
        /* Convert MCUs to RGB using Y MCUs 
        for (uint32_t i = 0; i < nb_blocks; i++) {
            for (uint32_t j=0;j<8;j++){
                for (uint32_t k=0;k<8;k++){
                /* Extract RGB values from Y values 
                    g = Y[i][j][k];
                    mcu_rgb[i*64 + j*8 + k] = g << 16 | g << 8 | g;
                    printf("%02x ",mcu_rgb[i*64 + j*8 + k]);
        
        }
}
}
}*/

int main(void){
    uint8_t** mcu_rgb=malloc(128*sizeof(uint8_t*));
    for (int i=0;i<128;i++){
        mcu_rgb[i]=malloc(3*sizeof(uint8_t));
    }
    one_YCbCr_mcu_to_rgb(mcu_rgb,2,1);
    for (int i=0;i<128;i++){
        printf("{");
        for (int j=0;j<2;j++)
        printf("0x%02x,",mcu_rgb[i][j]);
        printf("0x%02x",mcu_rgb[i][2]);
        printf("},");
    }

    return 0;
}