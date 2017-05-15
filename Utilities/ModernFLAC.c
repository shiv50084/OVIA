#include <stdio.h>
#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/BitIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"
#include "../libModernFLAC/include/Decoder/DecodeFLAC.h"
#include "../libModernFLAC/include/Encoder/EncodeFLAC.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    CommandLineOptions *SetModernFLACOptions(void) {
        CommandLineOptions *CMD = InitCommandLineOptions(7);
        
        SetCMDName(CMD, "ModernFLAC");
        SetCMDDescription(CMD, "FLAC encoder/decoder written in modern C");
        SetCMDAuthor(CMD, "BumbleBritches57");
        SetCMDCopyright(CMD, "2017-2017");
        SetCMDLicense(CMD, "Revised BSD (3 clause)", false);
        SetCMDMinSwitches(CMD, 3);
        
        SetCMDSwitchFlag(CMD, 0, "Input", 5);
        SetCMDSwitchDescription(CMD, 0, "Input file or stdin with: '-'");
        SetCMDSwitchResultStatus(CMD, 0, false);
        
        SetCMDSwitchFlag(CMD, 1, "Output", 6);
        SetCMDSwitchDescription(CMD, 1, "Output file or stdout with: '-'");
        SetCMDSwitchResultStatus(CMD, 1, false);
        
        SetCMDSwitchFlag(CMD, 2, "Decode", 6);
        SetCMDSwitchDescription(CMD, 2, "Decode input FLAC to output");
        SetCMDSwitchResultStatus(CMD, 2, true);
        
        SetCMDSwitchFlag(CMD, 3, "Encode", 6);
        SetCMDSwitchDescription(CMD, 3, "Encode input to output FLAC");
        SetCMDSwitchResultStatus(CMD, 3, true);
        
        SetCMDSwitchFlag(CMD, 4, "Reencode", 8);
        SetCMDSwitchDescription(CMD, 4, "Reencodes the input flac with -O");
        SetCMDSwitchResultStatus(CMD, 4, true);
        
        SetCMDSwitchFlag(CMD, 5, "Subset", 6);
        SetCMDSwitchDescription(CMD, 5, "Limit encoding to subset format");
        SetCMDSwitchResultStatus(CMD, 5, true);
        
        SetCMDSwitchFlag(CMD, 6, "Optimize", 8);
        SetCMDSwitchDescription(CMD, 6, "Optimize encoded FLAC to be as small as possible");
        SetCMDSwitchResultStatus(CMD, 6, true);
        
        return CMD;
    }
    
    void FLACDecodeFile(BitInput *BitI, BitOutput *BitO, DecodeFLAC *Dec, CommandLineOptions *CMD) {
        uint32_t FileMagic = ReadBits(BitI, 32, true);
        
        if (FileMagic != FLACMagic) {
            // Not a FLAC file
            char Error[BitIOStringSize];
            snprintf(Error, BitIOStringSize, "Not a FLAC file, magic was: 0x%X\n", FileMagic);
            Log(LOG_ERR, "ModernFLAC", "FLACDecodeFile", Error);
        } else {
            for (size_t Byte = 4; Byte < GetBitInputFileSize(BitI); Byte++) { // loop to decode file
                while (Dec->LastMetadataBlock == false) {
                    FLACParseMetadata(BitI, Dec);
                }
                if (ReadBits(BitI, 14, true) == FLACFrameMagic) {
                    FLACReadFrame(BitI, Dec);
                }
            }
        }
    }
    
    void FLACEncodeFile(BitInput *BitI, BitOutput *BitO, EncodeFLAC *Enc, CommandLineOptions *CMD) {
        Enc->EncodeSubset = GetCMDSwitchPresence(CMD, 4);
        Enc->OptimizeFile = GetCMDSwitchPresence(CMD, 5);
        // Start requesting PCM samples to encode into frames, given all PCM formats are interleaved, you'll need to handle that.
    }
    
    void CloseEverything(BitInput *BitI, BitOutput *BitO, PCMFile *PCM, CommandLineOptions *CMD, DecodeFLAC *Dec, EncodeFLAC *Enc) {
        CloseBitInput(BitI);
        CloseBitOutput(BitO);
        ClosePCMFile(PCM);
        CloseCommandLineOptions(CMD);
        CloseFLACDecoder(Dec);
        CloseFLACEncoder(Enc);
    }
    
    int main(int argc, const char *argv[]) {
        CommandLineOptions *CMD = SetModernFLACOptions();
        
        ParseCommandLineArguments(CMD, argc, argv);
        BitInput           *BitI    = InitBitInput();
        BitOutput          *BitO    = InitBitOutput();
        PCMFile            *PCM     = InitPCMFile();
        DecodeFLAC         *Dec     = InitDecodeFLAC();
        EncodeFLAC         *Enc     = InitEncodeFLAC();
        OpenCMDInputFile(BitI, CMD, 0);
        OpenCMDOutputFile(BitO, CMD, 1);
        
        bool Decode   = GetCMDSwitchPresence(CMD, 2);
        bool Encode   = GetCMDSwitchPresence(CMD, 3);
        bool Reencode = GetCMDSwitchPresence(CMD, 4);
        bool Subset   = GetCMDSwitchPresence(CMD, 5);
        
        // Find out if -d or -e was included on the command line
        if (Decode == true || Reencode == true) {
            if (ReadBits(BitI, 32, true) == FLACMagic) {
                for (uint8_t Byte = 0; Byte < GetBitInputFileSize(BitI); Byte++) {
                    if (PeekBits(BitI, 14, true) == FLACFrameMagic) {
                        FLACReadFrame(BitI, Dec);
                    } else {
                        FLACParseMetadata(BitI, Dec);
                    }
                }
            }
            // Decode the file.
            // To decode we'll need to init the DecodeFLAC, and output the stuff to wav or w64
        } else if (Encode == true) {
            Enc->EncodeSubset = GetCMDSwitchPresence(CMD, 5);
            Enc->OptimizeFile = GetCMDSwitchPresence(CMD, 6);
            IdentifyPCMFile(BitI, PCM);
            EncodeFLACFile(PCM, BitO, Enc);
            
            // Encode the file to FLAC
            // ParseWAV and encode FLAC
        } else {
            // Reencode the input
        }
        
        CloseEverything(BitI, BitO, PCM, CMD, Dec, Enc);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
