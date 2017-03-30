#include <stdio.h>
#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/BitIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"
#include "../libModernFLAC/include/Decoder/DecodeFLAC.h"
#include "../libModernFLAC/include/Encoder/EncodeFLAC.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    CommandLineOptions *SetModernFLACOptions(void) {
        CommandLineOptions *CMD = InitCommandLineOptions();
        InitCommandLineSwitches(CMD, 7);
        
        SetCMDName(CMD, "ModernFLAC");
        SetCMDDescription(CMD, "FLAC encoder/decoder written in modern C");
        SetCMDAuthor(CMD, "BumbleBritches57");
        SetCMDCopyright(CMD, "2017-2017");
        SetCMDLicense(CMD, "Revised BSD (3 clause)");
        SetCMDMinSwitches(CMD, 3);
        /*
        SetSwitchFlag(CMD, 0, "Input");
        SetSwitchDescription(CMD, 0, "Input file or stdin with: '-'");
        SetSwitchResultStatus(CMD, 0, false);
         */
        
        SetSwitchFlag(CMD, 1, "Output");
        SetSwitchDescription(CMD, 1, "Output file or stdout with: '-'");
        SetSwitchResultStatus(CMD, 1, false);
        
        SetSwitchFlag(CMD, 2, "Decode");
        SetSwitchDescription(CMD, 2, "Decode input FLAC to output");
        SetSwitchResultStatus(CMD, 2, true);
        
        SetSwitchFlag(CMD, 3, "Encode");
        SetSwitchFlag(CMD, 3, "Encode input to output FLAC");
        SetSwitchResultStatus(CMD, 3, true);
        
        SetSwitchFlag(CMD, 4, "Reencode");
        SetSwitchDescription(CMD, 4, "Reencodes the input flac with -O");
        SetSwitchResultStatus(CMD, 4, true);
        
        SetSwitchFlag(CMD, 5, "Subset");
        SetSwitchDescription(CMD, 5, "Limit encoding to subset format");
        SetSwitchResultStatus(CMD, 5, true);
        
        SetSwitchFlag(CMD, 6, "Optimize");
        SetSwitchDescription(CMD, 6, "Optimize encoded FLAC to be as small as possible");
        SetSwitchResultStatus(CMD, 6, true);
        
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
            for (size_t Byte = 4; Byte < GetInputFileSize(BitI); Byte++) { // loop to decode file
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
        Enc->EncodeSubset = IsSwitchPresent(CMD, 4);
        Enc->OptimizeFile = IsSwitchPresent(CMD, 5);
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
        
        bool Decode   = IsSwitchPresent(CMD, 2);
        bool Encode   = IsSwitchPresent(CMD, 3);
        bool Reencode = IsSwitchPresent(CMD, 4);
        bool Subset   = IsSwitchPresent(CMD, 5);
        
        // Find out if -d or -e was included on the command line
        if (Decode == true || Reencode == true) {
            if (ReadBits(BitI, 32, true) == FLACMagic) {
                for (uint8_t Byte = 0; Byte < GetInputFileSize(BitI); Byte++) {
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
            Enc->EncodeSubset = IsSwitchPresent(CMD, 5);
            Enc->OptimizeFile = IsSwitchPresent(CMD, 6);
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
