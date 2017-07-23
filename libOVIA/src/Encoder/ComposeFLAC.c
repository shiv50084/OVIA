#include "../../include/Encoder/ComposeFLAC.h"
#include "../../include/FLACTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void   FLACWriteStreaminfo(BitBuffer *OutputFLAC, EncodeFLAC *Enc) {
        WriteBits(OutputFLAC, 34, 24, true); // StreamInfoSize
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->MinimumBlockSize, 16, false);
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->MaximumBlockSize, 16, false);
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->MinimumFrameSize, 24, false);
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->MaximumFrameSize, 24, false);
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->SampleRate, 20, false);
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->MinimumFrameSize, 24, false);
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->Channels - 1, 3, false);
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->BitDepth - 1, 5, false);
        WriteBits(OutputFLAC, Enc->Meta->StreamInfo->SamplesInStream, 36, false);
        WriteBits(OutputFLAC, 0, 128, true); // ROOM for the MD5.
    }
    
    void   FLACWriteVorbisComment(BitBuffer *OutputFLAC, EncodeFLAC *Enc) {
        WriteBits(OutputFLAC, Enc->Meta->Vorbis->VendorTagSize, 32, false);
        for (uint32_t TagByte = 0; TagByte < Enc->Meta->Vorbis->VendorTagSize; TagByte++) {
            WriteBits(OutputFLAC, Enc->Meta->Vorbis->VendorTag[TagByte], 8, true); // C strings are big endian
        }
        WriteBits(OutputFLAC, Enc->Meta->Vorbis->NumUserTags, 32, false);
        for (uint32_t UserTag = 0; UserTag < Enc->Meta->Vorbis->NumUserTags; UserTag++) {
            WriteBits(OutputFLAC, Enc->Meta->Vorbis->UserTagSize, 8, false);
        }
    }
    
#ifdef __cplusplus
}
#endif
