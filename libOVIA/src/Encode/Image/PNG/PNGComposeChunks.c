#include "../../include/libModernPNG.h"
#include "../../include/Private/libModernPNG_Types.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void PNGIsAnimated(EncodePNG *Enc, const bool PNGIsAnimated) {
        if (Enc != NULL) {
            Enc->PNGIsAnimated = PNGIsAnimated;
        } else {
            Log(Log_ERROR, __func__, U8("Pointer to EncodePNG is NULL"));
        }
    }
    
    void ComposeAPNGNumFrames(EncodePNG *Enc, const uint32_t NumFrames) {
        if (Enc != NULL) {
            Enc->acTL->NumFrames = NumFrames;
        } else {
            Log(Log_ERROR, __func__, U8("Pointer to EncodePNG is NULL"));
        }
    }
    
    void ComposeAPNGTimes2Loop(EncodePNG *Enc, const uint32_t NumTimes2Loop) {
        if (Enc != NULL) {
            Enc->acTL->TimesToLoop = NumTimes2Loop;
        } else {
            Log(Log_ERROR, __func__, U8("Pointer to EncodePNG is NULL"));
        }
    }
    
    void ComposeAPNGFrameDelay(EncodePNG *Enc, const uint32_t FrameDelayNumerator, const uint32_t FrameDelayDenominator) {
        if (Enc == NULL) {
            Log(Log_ERROR, __func__, U8("Pointer to EncodePNG is NULL"));
        } else {
            Enc->fcTL->FrameDelayNumerator   = FrameDelayNumerator;
            Enc->fcTL->FrameDelayDenominator = FrameDelayDenominator;
        }
    }
    
    void PNGSetIHDR(EncodePNG *Enc, const uint32_t Height, const uint32_t Width, const uint8_t BitDepth, PNGColorTypes ColorType, const bool Interlace) {
        if (Enc != NULL && Height > 0 && Width > 0 && (BitDepth > 0 || BitDepth > 16) && (ColorType <= 6 && ColorType != 1 && ColorType != 5) && Interlace >= 0 && Interlace <= 1) {
            Enc->iHDR->Height       = Height;
            Enc->iHDR->Width        = Width;
            Enc->iHDR->BitDepth     = BitDepth;
            Enc->iHDR->ColorType    = ColorType;
            Enc->iHDR->IsInterlaced = Interlace;
        } else if (Enc == NULL) {
            Log(Log_ERROR, __func__, U8("EncodePNG Pointer is NULL"));
        } else if (Height == 0) {
            Log(Log_ERROR, __func__, U8("Height is 0, which is invalid"));
        } else if (Width == 0) {
            Log(Log_ERROR, __func__, U8("Width is 0, which is invalid"));
        } else if (BitDepth == 0 || BitDepth > 16) {
            Log(Log_ERROR, __func__, U8("BitDepth %d is invalid, valid values range from 1-16"), BitDepth);
        } else if (ColorType > 6 || ColorType == 1 || ColorType == 5) {
            Log(Log_ERROR, __func__, U8("ColorType %d is invalid, valid values range from 0-6, excluding 1 and 5"), ColorType);
        } else if (Interlace > 1) {
            Log(Log_ERROR, __func__, U8("Interlace %d is invalid, valid values range from 0-1"), Interlace);
        }
    }
    
    void PNGSetSTER(EncodePNG *Enc, const bool StereoType) {
        if (Enc != NULL && StereoType >= 0 && StereoType <= 1) {
            Enc->sTER->StereoType = StereoType;
        } else if (Enc == NULL) {
            Log(Log_ERROR, __func__, U8("Pointer to EncodePNG is NULL"));
        } else if (StereoType < 0 || StereoType > 1) {
            Log(Log_ERROR, __func__, U8("StereoType %d is invalid, valid values range from 0-1"));
        }
    }
    
    void PNGSetACTL(EncodePNG *Enc, const uint32_t NumFrames, const uint32_t Times2Loop) {
        if (Enc != NULL && NumFrames > 0) {
            Enc->acTL->NumFrames   = NumFrames;
            Enc->acTL->TimesToLoop = Times2Loop;
        } else if (Enc == NULL) {
            Log(Log_ERROR, __func__, U8("Pointer to EncodePNG is NULL"));
        } else if (NumFrames == 0) {
            Log(Log_ERROR, __func__, U8("NumFrames is 0, that isn't possible..."));
        }
    }
    
    void PNGSetFCTL(EncodePNG *Enc, const uint32_t FrameNum, const uint32_t FrameWidth, const uint32_t FrameHeight, uint32_t XOffset, uint32_t YOffset, uint16_t FrameDelayNumerator, uint16_t FrameDelayDenominator, uint8_t DisposalType, uint8_t BlendType) {
        
    }
    
    void ComposePNGCHRMWhitePoint() {
        
    }
    
#ifdef __cplusplus
}
#endif
