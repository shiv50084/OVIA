#include "../../../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/BitIO.h"
#include "../../../Dependencies/libPCM/libPCM/include/libPCM.h"
#include "../../include/libModernFLAC.h"
#include "../../include/Encoder/EncodeFLAC.h"
#include "../../include/FLACTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void   FLACWriteMetadata(BitOutput *BitO, EncodeFLAC *Enc) {
        WriteBits(BitO, FLACMagic, 32, true);
        WriteBits(BitO, false, 1, true); // IsLastMetadataBlock
        //WriteBits(BitO, FLACMetadataTypes, 7); // MetadataBlockType
        
    }
    
    int8_t EncodeFLACFile(PCMFile *PCM, BitOutput *BitO, EncodeFLAC *Enc) {
        if (Enc->EncodeSubset == true && Enc->Data->Frame->SampleRate <= 48000) {
            Enc->MaxBlockSize          =  4608;
            Enc->MaxFilterOrder        =    12;
            Enc->MaxRICEPartitionOrder =     8;
        } else {
            Enc->MaxBlockSize          = 16384;
            Enc->MaxFilterOrder        =    32;
            Enc->MaxRICEPartitionOrder =    15;
        }
        
        if (Enc->OptimizeFile == true) {
            // Optimize this mufucka

        }
        
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
