#include "../../include/Private/JPEGCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void WriteSegment_StartOfImage(BitBuffer *BitB) {
        if (BitB != NULL) {
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, Marker_StartOfImage);
        } else {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void WriteSegment_StartOfFrame(JPEGOptions *JPEG, BitBuffer *BitB, uint16_t StartOfFrameMarker) {
        if (BitB != NULL) {
            if (StartOfFrameMarker == Marker_StartOfFrameLossless1 ||
                StartOfFrameMarker == Marker_StartOfFrameLossless2 ||
                StartOfFrameMarker == Marker_StartOfFrameLossless3 ||
                StartOfFrameMarker == Marker_StartOfFrameLossless4) {
                uint16_t SegmentSize = 8 + (JPEG->NumChannels * 3);
                BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, StartOfFrameMarker);
                BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, SegmentSize);
                BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, JPEG->BitDepth);
                BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, JPEG->Height);
                BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, JPEG->Width);
                BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, JPEG->NumChannels);
                for (uint8_t Component = 0; Component < JPEG->NumChannels; Component++) {
                    BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, JPEG->Components[Component].ComponentID);
                    BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, JPEG->Components[Component].Horizontal);
                    BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 8, JPEG->Components[Component].Vertical);
                }
            } else {
                Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("StartOfFrameMarker %hu isn't a Lossless SOF Marker"), StartOfFrameMarker);
            }
        } else {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void WriteSegment_Comment(BitBuffer *BitB, UTF8 *CommentString) {
        if (BitB != NULL) {
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, Marker_Comment);
            uint16_t StringSize = UTF8_GetStringSizeInCodeUnits(CommentString);
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, StringSize + 2);
            BitBuffer_WriteUTF8(BitB, CommentString, WriteType_NULLTerminator);
        } else {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void WriteSegment_ICCProfile(BitBuffer *BitB) {
        if (BitB != NULL) {
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, Marker_ICCProfile);
            
        } else {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void JPEGWriteHeader(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            /*
             
             Not really sure, lol.
             
             */
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void JPEGWriteFooter(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, Marker_EndOfImage);
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void JPEGWriteImage(void *Options, void *Container, BitBuffer *BitB) {
        if (Options != NULL && Container != NULL && BitB != NULL) {
            /*
             
             Not really sure, lol.
             
             */
        } else if (Options == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (Container == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Container Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
#define NumJPEGExtensions 6
    
    static const UTF32 *JPEGExtensions[NumJPEGExtensions] = {
        [0] = UTF32String("jpg"),
        [1] = UTF32String("jpe"),
        [2] = UTF32String("jif"),
        [3] = UTF32String("jfi"),
        [4] = UTF32String("jfif"),
        [5] = UTF32String("jpeg"),
    };
    
    static const OVIAEncoder JPEGEncoder = {
        .EncoderID             = CodecID_JPEG,
        .MediaType             = MediaType_Image,
        .NumExtensions         = NumJPEGExtensions,
        .Extensions            = JPEGExtensions,
        .Function_Initialize   = JPEGOptions_Init,
        .Function_WriteHeader  = JPEGWriteHeader,
        .Function_Encode       = JPEGWriteImage,
        .Function_WriteFooter  = JPEGWriteFooter,
        .Function_Deinitialize = JPEGOptions_Deinit,
    };
    
#ifdef __cplusplus
}
#endif
