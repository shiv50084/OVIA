#include "../../include/Private/PNMCommon.h"


#ifdef __cplusplus
extern "C" {
#endif
    
    static void PNMWriteHeader_ASCII(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            PNMOptions *PNM = Options;
            /* Write the Width */
            UTF8 *Width = UTF8_Integer2String(Base10, PNM->Width);
            BitBuffer_WriteUTF8(BitB, Width);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, PNMEndField);
            free(Width);
            /* Write the Width */
            
            /* Write the Height */
            UTF8 *Height = UTF8_Integer2String(Base10, PNM->Height);
            BitBuffer_WriteUTF8(BitB, Height);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, PNMEndField);
            free(Height);
            /* Write the Height */
        } else if (Options == NULL) {
            Log(Log_DEBUG, __func__, U8("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void PNMWriteHeader_Binary(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            PNMOptions *PNM = Options;
            /* Write the Width */
            UTF8 *Width = UTF8_Integer2String(Base10, PNM->Width);
            BitBuffer_WriteUTF8(BitB, Width);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            free(Width);
            /* Write the Width */
            
            /* Write the Height */
            UTF8 *Height = UTF8_Integer2String(Base10, PNM->Height);
            BitBuffer_WriteUTF8(BitB, Height);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            free(Height);
            /* Write the Height */
            
            /* Write the BitDepth */
            uint64_t MaxVal    = Exponentiate(2, PNM->BitDepth) - 1;
            UTF8    *BitDepth  = UTF8_Integer2String(Base10, MaxVal);
            BitBuffer_WriteUTF8(BitB, BitDepth);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            free(BitDepth);
            /* Write the BitDepth */
        } else if (Options == NULL) {
            Log(Log_DEBUG, __func__, U8("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void PNMWriteHeader_PAM(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            PNMOptions *PNM = Options;
            BitBuffer_WriteUTF8(BitB, U8("P7"));
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            
            /* Write the Width */
            UTF8 *Width         = UTF8_Integer2String(Base10, PNM->Width);
            BitBuffer_WriteUTF8(BitB, U8("WIDTH "));
            BitBuffer_WriteUTF8(BitB, Width);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            free(Width);
            /* Write the Width */
            
            /* Write the Height */
            UTF8 *Height = UTF8_Integer2String(Base10, PNM->Height);
            BitBuffer_WriteUTF8(BitB, U8("HEIGHT "));
            BitBuffer_WriteUTF8(BitB, Height);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            free(Height);
            /* Write the Height */
            
            /* Write the NumChannels */
            UTF8 *NumChannels = UTF8_Integer2String(Base10, PNM->NumChannels);
            BitBuffer_WriteUTF8(BitB, U8("DEPTH "));
            BitBuffer_WriteUTF8(BitB, NumChannels);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            free(NumChannels);
            /* Write the NumChannels */
            
            /* Write the BitDepth */
            uint64_t MaxVal = Exponentiate(2, PNM->BitDepth) - 1;
            UTF8 *BitDepth  = UTF8_Integer2String(Base10, MaxVal);
            BitBuffer_WriteUTF8(BitB, U8("MAXVAL "));
            BitBuffer_WriteUTF8(BitB, BitDepth);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            free(BitDepth);
            /* Write the BitDepth */
            
            /* Write the TUPLTYPE */
            BitBuffer_WriteUTF8(BitB, U8("TUPLTYPE "));
            PNMTupleTypes TupleType = PNM->TupleType;
            if (TupleType == PNM_TUPLE_BnW) {
                BitBuffer_WriteUTF8(BitB, U8("BLACKANDWHITE"));
            } else if (TupleType == PNM_TUPLE_Gray) {
                BitBuffer_WriteUTF8(BitB, U8("GRAYSCALE"));
            } else if (TupleType == PNM_TUPLE_GrayAlpha) {
                BitBuffer_WriteUTF8(BitB, U8("GRAYSCALE_ALPHA"));
            } else if (TupleType == PNM_TUPLE_RGB) {
                BitBuffer_WriteUTF8(BitB, U8("RGB"));
            } else if (TupleType == PNM_TUPLE_RGBAlpha) {
                BitBuffer_WriteUTF8(BitB, U8("RGB_ALPHA"));
            }
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            /* Write the TUPLTYPE */
            
            /* Write the ENDHDR */
            BitBuffer_WriteUTF8(BitB, U8("ENDHDR"));
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, 0x0A);
            /* Write the ENDHDR */
        } else if (Options == NULL) {
            Log(Log_DEBUG, __func__, U8("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void PNMInsertImage(void *Options, BitBuffer *BitB, ImageContainer *Image) {
        if (Options != NULL && BitB != NULL && Image != NULL) {
            PNMOptions *PNM       = Options;
            uint64_t ChannelCount = PNM->NumChannels;
            uint64_t Width        = ImageContainer_GetWidth(Image);
            uint64_t Height       = ImageContainer_GetHeight(Image);
            Image_Types Type      = ImageContainer_GetType(Image);
            if (Type == ImageType_Integer8) {
                uint8_t *Array  = (uint8_t*) ImageContainer_GetArray(Image);
                for (uint64_t W = 0ULL; W < Width; W++) {
                    for (uint64_t H = 0ULL; H < Height; H++) {
                        for (uint16_t Channel = 0; Channel < ChannelCount; Channel++) {
                            BitBuffer_WriteBits(BitB, MSByteFirst, MSBitFirst, ChannelCount, Array[W * H * Channel]);
                        }
                    }
                }
            } else if (Type == ImageType_Integer16) {
                uint16_t *Array  = (uint16_t*) ImageContainer_GetArray(Image);
                for (uint64_t W = 0ULL; W < Width; W++) {
                    for (uint64_t H = 0ULL; H < Height; H++) {
                        for (uint16_t Channel = 0; Channel < ChannelCount; Channel++) {
                            BitBuffer_WriteBits(BitB, MSByteFirst, MSBitFirst, ChannelCount, Array[W * H * Channel]);
                        }
                    }
                }
            }
        } else if (Options == NULL) {
            Log(Log_DEBUG, __func__, U8("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Image == NULL) {
            Log(Log_DEBUG, __func__, U8("Pixels2Write Pointer is NULL"));
        }
    }
    
    static void RegisterEncoder_PNM_ASCII(OVIA *Ovia) {
        Ovia->NumEncoders                                 += 1;
        uint64_t EncoderIndex                              = Ovia->NumEncoders;
        Ovia->Encoders                                     = realloc(Ovia->Encoders, sizeof(OVIAEncoder) * Ovia->NumEncoders);
        
        Ovia->Encoders[EncoderIndex].EncoderID             = CodecID_PNM;
        Ovia->Encoders[EncoderIndex].MediaType             = MediaType_Image;
        Ovia->Encoders[EncoderIndex].Function_Initialize   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader  = PNMWriteHeader_ASCII;
        Ovia->Encoders[EncoderIndex].Function_Encode       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_WriteFooter  = NULL;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize = PNMOptions_Deinit;
    }
    
    static void RegisterEncoder_PNM_Binary(OVIA *Ovia) {
        Ovia->NumEncoders                                 += 1;
        uint64_t EncoderIndex                              = Ovia->NumEncoders;
        Ovia->Encoders                                     = realloc(Ovia->Encoders, sizeof(OVIAEncoder) * Ovia->NumEncoders);
        
        Ovia->Encoders[EncoderIndex].EncoderID             = CodecID_PNM;
        Ovia->Encoders[EncoderIndex].MediaType             = MediaType_Image;
        Ovia->Encoders[EncoderIndex].Function_Initialize   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader  = PNMWriteHeader_Binary;
        Ovia->Encoders[EncoderIndex].Function_Encode       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_WriteFooter  = NULL;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize = PNMOptions_Deinit;
    }
    
    static void RegisterEncoder_PAM(OVIA *Ovia) {
        Ovia->NumEncoders                                 += 1;
        uint64_t EncoderIndex                              = Ovia->NumEncoders;
        Ovia->Encoders                                     = realloc(Ovia->Encoders, sizeof(OVIAEncoder) * Ovia->NumEncoders);
        
        Ovia->Encoders[EncoderIndex].EncoderID             = CodecID_PNM;
        Ovia->Encoders[EncoderIndex].MediaType             = MediaType_Image;
        Ovia->Encoders[EncoderIndex].Function_Initialize   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader  = PNMWriteHeader_PAM;
        Ovia->Encoders[EncoderIndex].Function_Encode       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_WriteFooter  = NULL;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize = PNMOptions_Deinit;
    }
    
    static void RegisterEncoder_PNM(OVIA *Ovia) {
        Ovia->NumEncoders                                    += 1;
        uint64_t EncoderIndex                                 = Ovia->NumDecoders - 1;
        Ovia->Encoders                                        = realloc(Ovia->Encoders, sizeof(OVIAEncoder) * Ovia->NumEncoders);
        
        Ovia->Encoders[EncoderIndex].EncoderID                = CodecID_PNM;
        Ovia->Encoders[EncoderIndex].MediaType                = MediaType_Image;
        
        /* MagicID 0 */
        Ovia->Encoders[EncoderIndex].Function_Initialize[0]   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader[0]  = PNMWriteHeader_ASCII;
        Ovia->Encoders[EncoderIndex].Function_Encode[0]       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize[0] = PNMOptions_Deinit;
        /* MagicID 0 */
        
        /* MagicID 1 */
        Ovia->Encoders[EncoderIndex].Function_Initialize[1]   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader[1]  = PNMWriteHeader_ASCII;
        Ovia->Encoders[EncoderIndex].Function_Encode[1]       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize[1] = PNMOptions_Deinit;
        /* MagicID 1 */
        
        /* MagicID 2 */
        Ovia->Encoders[EncoderIndex].Function_Initialize[2]   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader[2]  = PNMWriteHeader_ASCII;
        Ovia->Encoders[EncoderIndex].Function_Encode[2]       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize[2] = PNMOptions_Deinit;
        /* MagicID 2 */ // Last ASCII
        
        /* MagicID 3 */
        Ovia->Encoders[EncoderIndex].Function_Initialize[3]   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader[3]  = PNMWriteHeader_Binary;
        Ovia->Encoders[EncoderIndex].Function_Encode[3]       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize[3] = PNMOptions_Deinit;
        /* MagicID 3 */
        
        /* MagicID 4 */
        Ovia->Encoders[EncoderIndex].Function_Initialize[4]   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader[4]  = PNMWriteHeader_Binary;
        Ovia->Encoders[EncoderIndex].Function_Encode[4]       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize[4] = PNMOptions_Deinit;
        /* MagicID 4 */
        
        /* MagicID 5 */
        Ovia->Encoders[EncoderIndex].Function_Initialize[5]   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader[5]  = PNMWriteHeader_Binary;
        Ovia->Encoders[EncoderIndex].Function_Encode[5]       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize[5] = PNMOptions_Deinit;
        /* MagicID 5 */
        
        /* MagicID 6 */
        Ovia->Encoders[EncoderIndex].Function_Initialize[6]   = PNMOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader[6]  = PNMWriteHeader_PAM;
        Ovia->Encoders[EncoderIndex].Function_Encode[6]       = PNMInsertImage;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize[6] = PNMOptions_Deinit;
        /* MagicID 6 */
    }
    
    static OVIACodecRegistry Register_PNMEncoder = {
        .Function_RegisterEncoder[CodecID_PNM - 1] = RegisterEncoder_PNM,
    };
    
#ifdef __cplusplus
}
#endif
