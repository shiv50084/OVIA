/*!
 @header              OVIACommon.h
 @author              Marcus Johnson
 @copyright           2016+
 @version             1.0.0
 @brief               This header contains code for reading and writing lossless PNM image files
 */

#include "../OVIA.h"

#include "../../../Dependencies/FoundationIO/Library/include/BitIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/ContainerIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/CryptographyIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/MathIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/UnicodeIO/LogIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/UnicodeIO/StringIO.h"

#pragma once

#ifndef OVIA_OVIACommon_H
#define OVIA_OVIACommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    /* __COUNTER__ is a compiler specific extension, but it's supported by Clang, gcc, and MSVC */
#define OVIADecoderID          __COUNTER__
#define OVIAEncoderID          __COUNTER__
#define OVIAForwardTransformID __COUNTER__
#define OVIAReverseTransformID __COUNTER__
    
#define MaximumMacro(A, B) (((A)>(B))?(A):(B))
    
    typedef enum OVIA_MediaTypes {
        MediaType_Unknown   = 0,
        MediaType_Container = 1,
        MediaType_Audio2D   = 2,
        MediaType_Audio3D   = 3,
        MediaType_Image     = 4,
        MediaType_Video     = 5,
    } OVIA_MediaTypes;
    
    typedef struct MagicIDSizes {
        const uint64_t NumSizes;
        const uint64_t Sizes[];
    } MagicIDSizes;
    
    typedef struct MagicIDOffsets {
        const uint64_t NumOffsets;
        const uint64_t Offsets[];
    } MagicIDOffsets;
    
    typedef struct MagicIDNumbers {
        const uint64_t NumMagicIDs;
        const uint8_t *MagicNumbers[];
    } MagicIDNumbers;
    
    typedef struct MagicIDs {
        const MagicIDSizes   *Sizes;
        const MagicIDOffsets *Offsets;
        const MagicIDNumbers *Number;
    } MagicIDs;
    
    typedef struct OVIADecoder {
        void*                 (*Function_Initialize)(void);
        void*                 (*Function_Decode)(void*, BitBuffer*); // Returns a Container pointer, takes Options and BitBuffer pointer
        void                  (*Function_Read)(void*, BitBuffer*); // Takes the Init type as a parameter
        void                  (*Function_Deinitialize)(void*);
        const MagicIDs         *MagicID;
        const OVIA_MediaTypes   MediaType;
        const OVIA_CodecIDs     DecoderID;
    } OVIADecoder;
    
    typedef struct OVIAEncoder {
        void *                (*Function_Initialize)(void);
        void                  (*Function_WriteHeader)(void *Options, BitBuffer *BitB);
        void                  (*Function_Encode)(void *Options, void *Contanier, BitBuffer *BitB); // Image*/Audio*, BitBuffer
        void                  (*Function_WriteFooter)(void *Options, BitBuffer *BitB);
        void                  (*Function_Deinitialize)(void *Options);
        const uint64_t          NumExtensions;
        const OVIA_CodecIDs     EncoderID;
        const OVIA_MediaTypes   MediaType;
        const UTF32           **Extensions;
        // How do we identify the encoder to choose? Maybe this should be an enum with a mapping function that maps all known codec names for example JPG, JPEG, JPE, JLS, JPEG-LS, JPEG-Lossless, LosslessJPEG to the CodecID
    } const OVIAEncoder;
    
    typedef struct OVIAColorTransform { // Ignore Alpha channels
        void                (*Function_Transform)(ImageContainer*);
        ImageChannelMask      InputChannels;
        ImageChannelMask      OutputChannels;
        OVIA_ColorTransforms  Transform;
    } OVIAColorTransform;
    
    typedef struct OVIADemuxer {
        const MagicIDs         *MagicID;
        const OVIA_MediaTypes   MediaType;
    } OVIADemuxer;
    
    typedef struct OVIAMuxer {
        const OVIA_MediaTypes   MediaType;
    } OVIAMuxer;
    
    typedef struct OVIA {
        OVIAEncoder        *Encoders;
        OVIADecoder        *Decoders;
        OVIAColorTransform *ForwardTransforms;
        OVIAColorTransform *ReverseTransforms;
        uint64_t            NumEncoders;
        uint64_t            NumDecoders;
        uint64_t            NumForwardTransforms;
        uint64_t            NumReverseTransforms;
    } OVIA;
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_OVIACommon_H */
