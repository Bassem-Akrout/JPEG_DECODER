#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "jpeg_reader.c"

/* THIS FUNCTION WORKS ON 1 MCU AND RETURNS THE NUMBER OF Y , Cb , Cr COMPONENTS AS WELL AS THE ORDER OF Y, Cb, Cr COMPONENTS*/


struct component{
    uint8_t identifier_C;
    uint8_t sampling_horziontal;
    uint8_t sampling_vertical;
    uint8_t i_q;
};


/*il faut extraire le nombre de blocs par mcu 
    if N==1 : seulement Y : on a donc : - identifiant/ facteur vertical/ facteur horizontal / identifiant iq 
    if N==3 : Y, Cb , Cr : on a donc - identifiant pour chaque composante / facteur vertical / horizontal / identifiant iq */


uint8_t bloc_number_component(uint8_t sampling_horz,uint8_t sampling_vertical){
    return sampling_horz*sampling_vertical;
}


uint8_t* components_order(struct SOF* sof,struct SOS* sos){
    /*RETURNS ORDER OF COMPONENTS */
    uint8_t components_number=sof->components_number;
    if (components_number==1)
       { printf("Only one component Y");
        return NULL; }
    
    else { /* N=3 */

        uint8_t* order=malloc(3*sizeof(uint8_t));
        for (uint8_t i=0;i<3;i++){
            for (uint8_t j=0;j<3;j++){
                if (sos->i_c[i]==sof->i_c[j]){
                    order[i]=j;
                }
            }

        }
    return order;
    }
    
    

}


uint8_t* block_number_list(struct SOF* sof){
    
    /*CALCULATES THE NUMBER OF BLOCKS PER COMPONENT IN AN MCU AND RETURNS IT INTO A LIST*/
    uint8_t nb_comp=sof->components_number;
    uint8_t* block_number=malloc(sof->components_number*sizeof(uint8_t));
    if (nb_comp==1){
        block_number[0]=sof->sampling_horizontal[0]*sof->sampling_vertical[0];
    }
    else {
        for (int i=0;i<nb_comp;i++){
            block_number[i]=sof->sampling_horizontal[i]*sof->sampling_vertical[i];
        }

    }
    return block_number;
}



int main(int argc,char** argv){

    FILE *jpeg_image= fopen(argv[1],"r");
    struct HEADER* header=calloc(1,sizeof(struct HEADER));
    extract_header(header,jpeg_image);
    uint8_t nb_composantes=header->sof->components_number;
    uint8_t* order=components_order(header->sof,header->sos);
    printf("nb_comp= %u\n",nb_composantes);
    uint8_t* b_number=block_number_list(header->sof);
    for (int i=0;i<nb_composantes;i++){
        printf("block: %u\n",b_number[i]);
        printf("ORDER: %u\n",order[i]);
    }



}


