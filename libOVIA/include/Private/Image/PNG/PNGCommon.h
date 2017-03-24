#include "../../Dependencies/BitIO/libBitIO/include/BitIO.h"

#pragma once

#ifndef LIBMODERNPNG_LIBMODERNPNG_H
#define LIBMODERNPNG_LIBMODERNPNG_H

#ifdef __cplusplus
extern "C" {
#endif
    
    enum PNGConstants {
        PNGMagic  = 0x89504E470D0A1A0A,
    } PNGConstants;
    
    enum PNGChunkMarkers {
        iHDRMarker = 0x49484452,
        acTLMarker = 0x6163544c,
        fcTLMarker = 0x6663544c,
        fDATMarker = 0x66444154,
        sTERMarker = 0x73544552,
        bKGDMarker = 0x626b4744,
        cHRMMarker = 0x6348524d,
        gAMAMarker = 0x67414d41,
        oFFsMarker = 0x6f464673,
        iCCPMarker = 0x69434350,
        sBITMarker = 0x73424954,
        sRGBMarker = 0x73524742,
        pHYsMarker = 0x70485973,
        pCALMarker = 0x7043414c,
    };
    
    enum PNGFlags {
        PNGIsStereoscopic       = false,
        PNGNeedsColorCorrection = false,
    } PNGFlags;
    
    enum PNGColorTypes {
        Grayscale   = 0,
        RGB         = 2,
        PalettedRGB = 3,
        GrayAlpha   = 4,
        RGBA        = 6,
    };
    
    static const char Number2Month[12][3] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
    };
    
    static const uint8_t ChannelsPerColorType[7] = {
        1, 0, 3, 3, 4, 0, 4
    };
    
    typedef struct PNGDecoder DecodePNG;
    
    typedef struct PNGEncoder EncodePNG;
    
    DecodePNG *InitDecodePNG(void);
    
    EncodePNG *InitEncodePNG(void);
    
    void        DecodePNGImage(BitInput *BitI, DecodePNG *PNG, uint16_t *DecodedImage);

#ifdef __cplusplus
}
#endif

#endif /* LIBMODERNPNG_LIBMODERNPNG_H */
