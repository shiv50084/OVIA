#include "../../../include/Private/Image/PNMCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    static void PNMWritePAMHeader(OVIA *Ovia, BitBuffer *BitB) {
        
    }
    
    static void PNMWriteBinaryPNMHeader(OVIA *Ovia, BitBuffer *BitB) {
        
    }
    
    static void PNMWriteASCIIPNMHeader(OVIA *Ovia, BitBuffer *BitB) {
        
    }
    
    void PNMWriteHeader(PNMTypes PNMType, BitBuffer *BitB) {
        if (PNMType == PAMPNM) {
            PNMWritePAMHeader(Ovia, BitB);
        } else if (PNMType == BinaryPNM) {
            PNMWriteBinaryPNMHeader(Ovia, BitB);
        } else if (PNMType == ASCIIPNM) {
            PNMWriteASCIIPNMHeader(Ovia, BitB);
        }
    }
    
    void PNMInsertImage(OVIA *Ovia, BitBuffer *CreatedImage, uint16_t ***Image2Insert) {
        if (PCM != NULL && CreatedImage != NULL && Image2Insert != NULL) {
            for (uint64_t Width = 0; Width < Ovia->Pic->Width; Width++) {
                for (uint64_t Height = 0; Height < Ovia->Pic->Height; Height++) {
                    for (uint16_t Channel = 0; Channel < Ovia->NumChannels; Channel++) {
                        WriteBits(MSByteFirst, MSBitFirst, CreatedImage, Ovia->BitDepth, Image2Insert[Width][Height][Channel]);
                    }
                }
            }
        } else if (PCM == NULL) {
            Log(Log_ERROR, __func__, U8("PCM Pointer is NULL"));
        } else if (CreatedImage == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Image2Insert == NULL) {
            Log(Log_ERROR, __func__, U8("Pixels2Write Pointer is NULL"));
        }
    }
    
    void PNMInsertPixels(OVIA *Ovia, BitBuffer *OutputPixels, uint32_t NumPixels2Write, uint16_t **Pixels2Write) {
        if (PCM != NULL && OutputPixels != NULL && Pixels2Write != NULL) {
            uint64_t ChannelCount = Ovia->NumChannels;
            for (uint32_t Pixel = 0; Pixel < NumPixels2Write; Pixel++) {
                for (uint16_t Channel = 0; Channel < ChannelCount; Channel++) {
                    WriteBits(MSByteFirst, MSBitFirst, OutputPixels, ChannelCount, Pixels2Write[Channel][Pixel]);
                }
            }
        } else if (PCM == NULL) {
            Log(Log_ERROR, __func__, U8("PCM Pointer is NULL"));
        } else if (OutputPixels == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Pixels2Write == NULL) {
            Log(Log_ERROR, __func__, U8("Pixels2Write Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif
