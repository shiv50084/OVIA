#include "../../include/Private/FLACCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void   FLACWriteMetadata(FLACOptions *FLAC, BitBuffer *BitB) {
        if (FLAC != NULL && BitB != NULL) {
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 32, FLACMagic);
            bool IsLastMetadataBlock = false;
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 1, IsLastMetadataBlock);
            uint8_t MetadataBlockType = 1;
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 7, MetadataBlockType);
        } else if (FLAC == NULL) {
            Log(Log_DEBUG, __func__, U8("FLACOptions Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLAC_Encode(FLACOptions *FLAC, Audio2DContainer *Audio, BitBuffer *BitB) {
        if (FLAC != NULL && Audio && BitB != NULL) {
            if (FLAC->EncodeSubset == true && Audio2DContainer_GetSampleRate(Audio) <= 48000) {
                FLAC->StreamInfo->MaximumBlockSize = 4608;
                FLAC->Frame->Sub->LPCFilterOrder   = 12;
                FLAC->Frame->PartitionOrder        = 8;
            } else {
                FLAC->StreamInfo->MaximumBlockSize = 16384;
                FLAC->Frame->Sub->LPCFilterOrder   = 32;
                FLAC->Frame->PartitionOrder        = 15;
            }
        } else if (FLAC == NULL) {
            Log(Log_DEBUG, __func__, U8("FLACOptions Pointer is NULL"));
        } else if (Audio == NULL) {
            Log(Log_DEBUG, __func__, U8("Audio2DContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLAC_WriteStreamInfo(FLACOptions *FLAC, BitBuffer *BitB) {
        if (FLAC != NULL && BitB != NULL) {
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 24, 34); // StreamInfoSize
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, FLAC->StreamInfo->MinimumBlockSize);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, FLAC->StreamInfo->MaximumBlockSize);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 24, FLAC->StreamInfo->MinimumFrameSize);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 24, FLAC->StreamInfo->MaximumFrameSize);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 20, FLAC->StreamInfo->CodedSampleRate);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst,  3, FLAC->StreamInfo->CodedSampleRate - 1);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst,  5, FLAC->StreamInfo->CodedBitDepth - 1);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 36, FLAC->StreamInfo->SamplesInStream);
            BitBuffer_Seek(BitB, 128); // Room for the MD5
        } else if (FLAC == NULL) {
            Log(Log_DEBUG, __func__, U8("FLACOptions Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLAC_WriteVorbis(FLACOptions *FLAC, BitBuffer *BitB) {
        if (FLAC != NULL && BitB != NULL) {
            BitBuffer_WriteBits(BitB, MSByteFirst, MSBitFirst, 32, 4);
            BitBuffer_WriteUTF8(BitB, U8("OVIA")); // Vendor tag
            
        } else if (FLAC == NULL) {
            Log(Log_DEBUG, __func__, U8("FLACOptions Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif