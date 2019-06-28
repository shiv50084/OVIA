#include "../../include/Private/WAVCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    static const uint8_t WAVNULLBinaryGUID[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    
    void WAVWriteHeader(BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void WAVWriteFMTChunk(BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint64_t NumChannels = OVIA_GetNumChannels(Ovia);
            uint64_t SampleRate  = OVIA_GetSampleRate(Ovia);
            uint64_t BitDepth    = OVIA_GetBitDepth(Ovia);
            uint8_t CBSize = 46;
            
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, 40); // ChunkSize
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, 0xFFFE); // WaveFormatExtensible
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, NumChannels);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, SampleRate);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, SampleRate * NumChannels * BitDepth);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_GetBlockSize(Ovia));
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, BitDepth);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, CBSize);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, BitDepth); // ValidBitsPerSample
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_GetChannelMask(Ovia));
            BitBuffer_WriteGUUID(BitB, GUIDString, WAVNULLBinaryGUID);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void WAVWriteLISTChunk(BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            if (OVIA_GetNumTags(Ovia) > 0) {
                // Write the tags
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void WAVAppendSamples(Audio2DContainer *Audio, BitBuffer *BitB) {
        if (Audio != NULL && BitB != NULL) {
            uint64_t NumChannels   = AudioContainer_GetNumSamples(Audio);
            uint64_t BitDepth      = AudioContainer_GetBitDepth(Audio);
            uint64_t NumSamples    = AudioContainer_GetNumSamples(Audio);
            Audio_Types Type       = AudioContainer_GetType(Audio);
            
            if (Type == (AudioType_Unsigned | AudioType_Integer8)) {
                uint8_t **Samples  = (uint8_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer8)) {
                int8_t **Samples   = (int8_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer16)) {
                uint16_t **Samples = (uint16_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer16)) {
                int16_t **Samples  = (int16_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer32)) {
                uint32_t **Samples = (uint32_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer32)) {
                int32_t **Samples  = (int32_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, BitDepth, Samples[Channel][Sample]);
                    }
                }
            }
        } else if (Audio == NULL) {
            Log(Log_ERROR, __func__, U8("Audio2DContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    OVIAEncoder WAVEncoder = {
        .EncoderID             = CodecID_WAV,
        .MediaType             = MediaType_Audio2D,
        .Function_Initialize   = WAVOptions_Init,
        .Function_WriteHeader  = WAVWriteHeader,
        .Function_Encode       = WAVAppendSamples,
        .Function_WriteFooter  = NULL,
        .Function_Deinitialize = WAVOptions_Deinit,
    };
    
#ifdef __cplusplus
}
#endif
