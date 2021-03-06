#include "../../include/Private/WAVCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     This encoder works for WAVE, BroadcastWAVE, RF64
     
     BWF Spcific Chunks:
     BEXT
     UBXT
     
     BW64 Specific Chunks:
     BW64
     DS64
     JUNK
     AXML
     CHNA
     
     BW64 is used instead of RIFF
     DS64 replaces WAVE
     
     */
    
    void WAVReadChunk_DS64(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV       = Options;
            uint32_t ChunkSize    = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            uint32_t SizeLow      = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            uint32_t SizeHigh     = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            uint32_t DataSizeLow  = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            uint32_t DataSizeHigh = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void WAVReadChunk_BEXT(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t DescriptionStringSize = BitBuffer_GetUTF8StringSize(BitB);
            WAV->BEXT->Description         = BitBuffer_ReadUTF8(BitB, DescriptionStringSize);
            uint64_t OriginatorStringSize  = BitBuffer_GetUTF8StringSize(BitB);
            WAV->BEXT->Originator          = BitBuffer_ReadUTF8(BitB, OriginatorStringSize);
            uint64_t OriginatorRefSize     = BitBuffer_GetUTF8StringSize(BitB);
            WAV->BEXT->OriginatorRef       = BitBuffer_ReadUTF8(BitB, OriginatorRefSize);
            uint64_t OriginatorDateSize    = BitBuffer_GetUTF8StringSize(BitB);
            WAV->BEXT->OriginatorDate      = BitBuffer_ReadUTF8(BitB, OriginatorDateSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void ReadINFO_TRCK(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Artist              = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void ReadINFO_IPRT(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Artist              = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void ReadINFO_IART(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Artist              = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void ReadINFO_ICRD(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->ReleaseDate         = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void ReadINFO_IGNR(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Genre               = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void ReadINFO_INAM(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Title               = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void ReadINFO_IPRD(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Album               = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void ReadINFO_ISFT(void *Options, BitBuffer *BitB) { // Encoder
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->CreationSoftware    = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void WAVReadLISTChunk(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            uint32_t SubChunkID   = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            uint32_t SubChunkSize = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            
            switch (SubChunkID) {
                case WAV_IART: // Artist
                    ReadINFO_IART(Options, BitB);
                    break;
                case WAV_ICRD: // Release date
                    ReadINFO_ICRD(Options, BitB);
                    break;
                case WAV_IGNR: // Genre
                    ReadINFO_IGNR(Options, BitB);
                    break;
                case WAV_INAM: // Title
                    ReadINFO_INAM(Options, BitB);
                    break;
                case WAV_IPRD: // Album
                    ReadINFO_IPRD(Options, BitB);
                    break;
                case WAV_ISFT: // Encoder
                    ReadINFO_ISFT(Options, BitB);
                    break;
                default:
                    Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Unknown LIST Chunk: 0x%X"), SubChunkID);
                    break;
            }
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void WAVReadFMTChunk(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV        = Options;
            uint32_t ChunkSize     = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            WAV->CompressionFormat = BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 16);
            WAV->NumChannels       = BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 16);
            WAV->SampleRate        = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            WAV->ByteRate          = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            WAV->BlockAlign        = (uint16_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 16);
            WAV->BitDepth          = BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 16);
            if (ChunkSize == 18) {
                uint16_t CBSize             = BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 16);
                BitBuffer_Seek(BitB, Bytes2Bits(CBSize - 16));
            } else if (ChunkSize == 40) {
                uint16_t CBSize             = BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 16);
                WAV->ValidBitsPerSample     = BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 16);
                
                WAV->SpeakerMask            = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
                uint8_t  *BinaryGUIDFormat  = BitBuffer_ReadGUUID(BitB, BinaryGUID);
                BitBuffer_Seek(BitB, Bytes2Bits(CBSize - 22));
            }
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void WAVReadMetadata(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV    = Options;
            uint32_t ChunkID   = (uint32_t) BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
            uint32_t ChunkSize = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, 32);
            
            switch (ChunkID) {
                case WAV_LIST:
                    break;
                case WAV_FMT:
                    WAVReadFMTChunk(Options, BitB);
                    break;
                case WAV_WAVE:
                    BitBuffer_Seek(BitB, 32);
                    break;
                default:
                    Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Invalid ChunkID: 0x%X"), ChunkID);
                    break;
            }
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void *WAVExtractSamples(void *Options, BitBuffer *BitB) {
        Audio2DContainer *Audio        = NULL;
        if (Options != NULL && BitB != NULL) {
            WAVOptions *WAV            = Options;
            uint64_t NumSamples        = Audio2DContainer_GetNumSamples(Audio);
            uint8_t  SampleSizeRounded = (uint8_t) Bytes2Bits(Bits2Bytes(WAV->BitDepth, RoundingType_Up));
            if (WAV->BitDepth <= 8) {
                uint8_t **Samples = (uint8_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < WAV->NumChannels; Channel++) {
                        Samples[Channel][Sample] = (uint8_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, SampleSizeRounded);
                    }
                }
            } else if (WAV->BitDepth <= 16) {
                uint16_t **Samples = (uint16_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < WAV->NumChannels; Channel++) {
                        Samples[Channel][Sample] = (uint16_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, SampleSizeRounded);
                    }
                }
            } else if (WAV->BitDepth <= 32) {
                uint32_t **Samples = (uint32_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < WAV->NumChannels; Channel++) {
                        Samples[Channel][Sample] = (uint32_t) BitBuffer_ReadBits(BitB, LSByteFirst, LSBitFirst, SampleSizeRounded);
                    }
                }
            }
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
        return Audio;
    }
    
    static const MagicIDSizes WAVMagicIDSize = {
        .NumSizes = 3,
        .Sizes    = {
            [0]   = 2,
            [1]   = 2,
            [2]   = 2,
            [3]   = 2,
            [4]   = 2,
            [5]   = 2,
            [6]   = 2,
        },
    };
    
    static const MagicIDOffsets WAVMagicIDOffset = {
        .NumOffsets = 3,
        .Offsets    = {
            [0]     = 0,
            [1]     = 0,
            [2]     = 0,
            [3]     = 0,
            [4]     = 0,
            [5]     = 0,
            [6]     = 0,
        },
    };
    
    static const MagicIDNumbers WAVMagicIDNumber = {
        .NumMagicIDs  = 7,
        .MagicNumbers = {
            [0]       = (uint8_t[2]){0x50, 0x31},
            [1]       = (uint8_t[2]){0x50, 0x32},
            [2]       = (uint8_t[2]){0x50, 0x33},
            [3]       = (uint8_t[2]){0x50, 0x34},
            [4]       = (uint8_t[2]){0x50, 0x35},
            [5]       = (uint8_t[2]){0x50, 0x36},
            [6]       = (uint8_t[2]){0x50, 0x37},
        },
    };
    
    static const MagicIDs WAVMagicIDs = {
        .Sizes                 = &WAVMagicIDSize,
        .Offsets               = &WAVMagicIDOffset,
        .Number                = &WAVMagicIDNumber,
    };
    
    static const OVIADecoder WAVDecoder = {
        .Function_Initialize   = WAVOptions_Init,
        .Function_Decode       = WAVExtractSamples,
        .Function_Read         = WAVReadMetadata,
        .Function_Deinitialize = WAVOptions_Deinit,
        .MagicID               = &WAVMagicIDs,
        .MediaType             = MediaType_Audio2D,
        .DecoderID             = CodecID_WAV,
    };
    
#ifdef __cplusplus
}
#endif
