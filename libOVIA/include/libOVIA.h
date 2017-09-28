#include "../../Dependencies/BitIO/libBitIO/include/BitIO.h"

#pragma once

#ifndef LIBPCM_libPCM_H
#define LIBPCM_libPCM_H

#ifdef __cplusplus
extern "C" {
#endif
    
    enum libPCMFileFormats {
        UnknownFormat       = 0,
        AIFFormat           = 1,
        WAVFormat           = 2,
        W64Format           = 3,
        BMPFormat           = 4,
        PXMFormat           = 5,
    };
    
    enum AIFSpeakerMask {
        AIFFrontLeft        = 0x1,
        AIFFrontRight       = 0x2,
        AIFFrontCenter      = 0x4,
        AIFRearLeft         = 0x8,
        AIFRearRight        = 0x10,
        AIFSurround         = 0x20,
        AIFLeftCenter       = 0x40,
        AIFRightCenter      = 0x80,
    };
    
    enum WAVSpeakerMask {
        WAVFrontRight       = 0x1,
        WAVFrontLeft        = 0x2,
        WAVFrontCenter      = 0x4,
        WAVLFE              = 0x8,
        WAVBackLeft         = 0x10,
        WAVBackRight        = 0x20,
        WAVFrontLeftCenter  = 0x40,
        WAVFrontRightCenter = 0x80,
        WAVBackCenter       = 0x100,
        WAVSideLeft         = 0x200,
        WAVSideRight        = 0x400,
        WAVTopCenter        = 0x800,
        WAVTopFrontLeft     = 0x1000,
        WAVTopFrontCenter   = 0x2000,
        WAVTopFrontRight    = 0x4000,
        WAVTopBackLeft      = 0x8000,
        WAVTopBackCenter    = 0x10000,
        WAVTopBackRight     = 0x20000,
    };
    
    enum AIFChunkIDs {
        AIF_FORM            = 0x464F524D,
        AIF_AIFF            = 0x41494646,
        AIF_AIFC            = 0x41494643,
        AIF_NAME            = 0x4E414D45,
        AIF_COMM            = 0x434F4D4D,
        AIF_SSND            = 0x53534E44,
    };
    
    enum WAVChunkIDs { // odd sized chunks have a trailing 0 byte, and strings are null padded, this is for all RIFF based formats.
        WAV_RIFF            = 0x52494646,
        WAV_WAVE            = 0x57415645,
        WAV_FMT             = 0x666D7420,
        WAV_LIST            = 0x4C495354,
        WAV_INFO            = 0x494E464F,
        WAV_IART            = 0x49415254, // Artist
        WAV_ICRD            = 0x49435244, // Release data
        WAV_IGNR            = 0x49474E52, // Genre
        WAV_INAM            = 0x494E414D, // Song name aka title
        WAV_IPRD            = 0x49505244, // Album name
        WAV_IPRT            = 0x49505254, // Track number
        WAV_ISFT            = 0x49534654, // Software that created it?
        WAV_CODE            = 0x434f4445, // Encoder
        WAV_ICMT            = 0x49434d54, // Comment
        WAV_ICOP            = 0x49434f50, // Copyright
        WAV_YEAR            = 0x59454152, // Year
        WAV_TRCK            = 0x5452434b, // Track
        WAV_DATA            = 0x64617461,
    };
    
    enum BMPMagic {
        BMP_BM              = 0x424D,
    };
    
    enum PXMMagic {
        PXM_P1              = 0x5031,
        PXM_P2              = 0x5032,
        PXM_P3              = 0x5033,
        PXM_P4              = 0x5034,
        PXM_P5              = 0x5035,
        PXM_P6              = 0x5036,
        PXM_P7              = 0x5037,
    };
    
    typedef PCMFile PCMFile;
    
    PCMFile *InitPCMFile(void);
    
    void     IdentifyPCMFile(PCMFile *PCM, BitBuffer *BitB);
    
    void     ParsePCMMetadata(PCMFile *PCM, BitBuffer *BitB);
    
    void     ExtractPCMSamples(PCMFile *PCM, BitBuffer *BitB, uint64_t Samples2Extract);
    
    void     DeinitPCMFile(PCMFile *PCM);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCM_H */
