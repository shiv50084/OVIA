#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Macros.h"
#include "../../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Format Oviaoding */
    static void W64ParseFMTChunk(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            OVIA_W64_SetCompressionType(Ovia, BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16));
            OVIA_SetNumChannels(Ovia,         BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16));
            OVIA_SetSampleRate(Ovia,          BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32));
            OVIA_SetBlockSize(Ovia,           BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32));
            OVIA_W64_SetBlockAlignment(Ovia,  BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16));
            OVIA_SetBitDepth(Ovia,            BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void W64ParseBEXTChunk(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void W64ParseDATAChunk(OVIA *Ovia, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        if (Ovia != NULL && BitB != NULL) {
            OVIA_SetNumSamples(Ovia, (((ChunkSize - 24 / OVIA_W64_GetBlockAlignment(Ovia)) / OVIA_GetNumChannels(Ovia)) / OVIA_GetBitDepth(Ovia)));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void W64ParseLEVLChunk(OVIA *Ovia, BitBuffer *BitB) { // aka Peak Envelope Chunk
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void W64ParseMetadata(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint8_t *ChunkUUIDString = BitBuffer_ReadGUUID(BitB, GUIDString);
            uint64_t W64Size         = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 64);
            if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_RIFF_GUIDString) == Yes) {
                
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_WAVE_GUIDString) == Yes) {
                
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_FMT_GUIDString) == Yes) {
                W64ParseFMTChunk(Ovia, BitB);
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_DATA_GUIDString) == Yes) {
                
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_LEVL_GUIDString) == Yes) {
                
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_BEXT_GUIDString) == Yes) {
                W64ParseBEXTChunk(Ovia, BitB);
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_FACT_GUIDString) == Yes) {
                
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_JUNK_GUIDString) == Yes) {
                
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_MRKR_GUIDString) == Yes) {
                
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_SUMM_GUIDString) == Yes) {
                
            } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_LIST_GUIDString) == Yes) {
                
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    AudioContainer *W64ExtractSamples(OVIA *Ovia, BitBuffer *BitB) {
        AudioContainer *Audio = NULL;
        if (Ovia != NULL && BitB != NULL) {
            uint64_t BitDepth     = OVIA_GetBitDepth(Ovia);
            uint64_t NumChannels  = OVIA_GetNumChannels(Ovia);
            uint64_t SampleRate   = OVIA_GetSampleRate(Ovia);
            uint64_t NumSamples   = OVIA_GetNumSamples(Ovia);
            if (BitDepth <= 8) {
                Audio = AudioContainer_Init(AudioType_Integer8, BitDepth, NumChannels, SampleRate, NumSamples);
                uint8_t **Samples = (uint8_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Bits2Bytes(BitDepth, Yes));
                    }
                }
            } else if (BitDepth > 8 && BitDepth <= 16) {
                Audio = AudioContainer_Init(AudioType_Integer16, BitDepth, NumChannels, SampleRate, NumSamples);
                uint16_t **Samples = (uint16_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Bits2Bytes(BitDepth, Yes));
                    }
                }
            } else if (BitDepth > 16 && BitDepth <= 32) {
                Audio = AudioContainer_Init(AudioType_Integer32, BitDepth, NumChannels, SampleRate, NumSamples);
                uint32_t **Samples = (uint32_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Bits2Bytes(BitDepth, Yes));
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        return Audio;
    }
    
#ifdef __cplusplus
}
#endif

