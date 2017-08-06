//#include <stdio.h>

#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/CommandLineInterface.h"
#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../Dependencies/libPCM/libPCM/include/libPCM.h"

#include "../libModernFLAC/include/libModernFLAC.h"

#define ModernFLACVersion "0.1.0"

#ifdef __cplusplus
extern "C" {
#endif
    
    enum ModernFLACCommandLineSwitchNames {
        Input    = 0,
        Output   = 1,
        Encode   = 2,
        Subset   = 3,
        Optimize = 4,
        Decode   = 5,
        Help     = 6,
    };
    
    CommandLineInterface *SetModernFLACOptions(void) {
        CommandLineInterface *CLI = InitCommandLineInterface(7);
        
        SetCLIName(CLI, "ModernFLAC");
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLIVersion(CLI, ModernFLACVersion);
        SetCLICopyright(CLI, "2017 - 2017");
        SetCLIDescription(CLI, "FLAC encoder/decoder written in modern C");
        SetCLILicense(CLI, "Revised BSD (3 clause)", false);
        SetCLIMinSwitches(CLI, 3);
        
        SetCLISwitchFlag(CLI, Input, "Input", 5);
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: '-'");
        SetCLISwitchResultStatus(CLI, Input, true);
        
        SetCLISwitchFlag(CLI, Output, "Output", 6);
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: '-'");
        SetCLISwitchResultStatus(CLI, Output, true);
        
        SetCLISwitchFlag(CLI, Encode, "Encode", 6);
        SetCLISwitchDescription(CLI, Encode, "Encode input to output FLAC");
        SetCLISwitchResultStatus(CLI, Encode, false);
        
        SetCLISwitchFlag(CLI, Subset, "Subset", 6);
        SetCLISwitchDescription(CLI, Subset, "Limit encoding to subset format");
        SetCLISwitchResultStatus(CLI, Subset, false);
        SetCLISwitchMetaFlag(CLI, Encode, Subset);
        
        SetCLISwitchFlag(CLI, Optimize, "Optimize", 8);
        SetCLISwitchDescription(CLI, Optimize, "Optimize encoded FLAC to be as small as possible");
        SetCLISwitchResultStatus(CLI, Optimize, false);
        
        SetCLISwitchFlag(CLI, Decode, "Decode", 6);
        SetCLISwitchDescription(CLI, Decode, "Decode input FLAC to output");
        SetCLISwitchResultStatus(CLI, Decode, false);
        
        SetCLISwitchFlag(CLI, Help, "Help", 4);
        SetCLISwitchDescription(CLI, Help, "Displays this usage information");
        SetCLISwitchResultStatus(CLI, Help, false);
        
        return CLI;
    }
    
    void FLACDecodeFile(BitBuffer *InputFLAC, BitOutput *BitO, DecodeFLAC *Dec, CommandLineInterface *CLI) {
        uint32_t FileMagic = ReadBits(InputFLAC, 32, true);
        
        if (FileMagic != FLACMagic) {
            // Not a FLAC file
            char Error[BitIOStringSize];
            snprintf(Error, BitIOStringSize, "Not a FLAC file, magic was: 0x%X\n", FileMagic);
            Log(LOG_ERR, "ModernFLAC", "FLACDecodeFile", Error);
        } else {
            for (size_t Byte = 4; Byte < GetBitInputFileSize(InputFLAC); Byte++) { // loop to decode file
                while (Dec->LastMetadataBlock == false) {
                    FLACParseMetadata(InputFLAC, Dec);
                }
                if (ReadBits(InputFLAC, 14, true) == FLACFrameMagic) {
                    FLACReadFrame(InputFLAC, Dec);
                }
            }
        }
    }
    
    void FLACEncodeFile(BitBuffer *InputFLAC, BitOutput *BitO, EncodeFLAC *Enc, CommandLineInterface *CLI) {
        Enc->EncodeSubset = GetCLISwitchPresence(CLI, 4);
        Enc->OptimizeFile = GetCLISwitchPresence(CLI, 5);
        // Start requesting PCM samples to encode into frames, given all PCM formats are interleaved, you'll need to handle that.
    }
    
    void CloseEverything(BitBuffer *InputFLAC, BitOutput *BitO, PCMFile *PCM, CommandLineInterface *CLI, DecodeFLAC *Dec, EncodeFLAC *Enc) {
        CloseBitOutput(BitO);
        CloseBitBuffer(InputFLAC);
        ClosePCMFile(PCM);
        DeinitCommandLineInterface(CLI);
        CloseFLACDecoder(Dec);
        CloseFLACEncoder(Enc);
    }
    
    int main(int argc, const char *argv[]) {
        CommandLineInterface *CLI = SetModernFLACOptions();
        
        ParseCommandLineArguments(CLI, argc, argv);
        BitInput           *InputFLAC    = InitBitInput();
        BitOutput          *BitO    = InitBitOutput();
        PCMFile            *PCM     = InitPCMFile();
        DecodeFLAC         *Dec     = InitDecodeFLAC();
        EncodeFLAC         *Enc     = InitEncodeFLAC();
        OpenCLIInputFile(InputFLAC, CLI, 0);
        OpenCLIOutputFile(BitO, CLI, 1);
        
        bool Decode   = GetCLISwitchPresence(CLI, 2);
        bool Encode   = GetCLISwitchPresence(CLI, 3);
        bool Reencode = GetCLISwitchPresence(CLI, 4);
        bool Subset   = GetCLISwitchPresence(CLI, 5);
        
        // Find out if -d or -e was included on the command line
        if (Decode == true || Reencode == true) {
            if (ReadBits(InputFLAC, 32, true) == FLACMagic) {
                for (uint8_t Byte = 0; Byte < GetBitInputFileSize(InputFLAC); Byte++) {
                    if (PeekBits(InputFLAC, 14, true) == FLACFrameMagic) {
                        FLACReadFrame(InputFLAC, Dec);
                    } else {
                        FLACParseMetadata(InputFLAC, Dec);
                    }
                }
            }
            // Decode the file.
            // To decode we'll need to init the DecodeFLAC, and output the stuff to wav or w64
        } else if (Encode == true) {
            Enc->EncodeSubset = GetCLISwitchPresence(CLI, 5);
            Enc->OptimizeFile = GetCLISwitchPresence(CLI, 6);
            IdentifyPCMFile(InputFLAC, PCM);
            EncodeFLACFile(PCM, BitO, Enc);
            
            // Encode the file to FLAC
            // ParseWAV and encode FLAC
        } else {
            // Reencode the input
        }
        
        CloseEverything(InputFLAC, BitO, PCM, CLI, Dec, Enc);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
