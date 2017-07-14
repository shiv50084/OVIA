#include <stdio.h>

#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/CommandLineInterface.h"
#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/BitIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"
#include "../libModernFLAC/include/Decoder/DecodeFLAC.h"
#include "../libModernFLAC/include/Encoder/EncodeFLAC.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    CommandLineInterface *SetModernFLACOptions(void) {
        CommandLineInterface *CLI = InitCommandLineInterface(7);
        
        SetCLIName(CLI, "ModernFLAC");
        SetCLIDescription(CLI, "FLAC encoder/decoder written in modern C");
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLICopyright(CLI, "2017-2017");
        SetCLILicense(CLI, "Revised BSD (3 clause)", false);
        SetCLIMinSwitches(CLI, 3);
        
        SetCLISwitchFlag(CLI, 0, "Input", 5);
        SetCLISwitchDescription(CLI, 0, "Input file or stdin with: '-'");
        SetCLISwitchResultStatus(CLI, 0, true);
        
        SetCLISwitchFlag(CLI, 1, "Output", 6);
        SetCLISwitchDescription(CLI, 1, "Output file or stdout with: '-'");
        SetCLISwitchResultStatus(CLI, 1, true);
        
        SetCLISwitchFlag(CLI, 2, "Decode", 6);
        SetCLISwitchDescription(CLI, 2, "Decode input FLAC to output");
        SetCLISwitchResultStatus(CLI, 2, false);
        
        SetCLISwitchFlag(CLI, 3, "Encode", 6);
        SetCLISwitchDescription(CLI, 3, "Encode input to output FLAC");
        SetCLISwitchResultStatus(CLI, 3, false);
        
        SetCLISwitchFlag(CLI, 4, "Reencode", 8);
        SetCLISwitchDescription(CLI, 4, "Reencodes the input flac with -O");
        SetCLISwitchResultStatus(CLI, 4, false);
        
        SetCLISwitchFlag(CLI, 5, "Subset", 6);
        SetCLISwitchDescription(CLI, 5, "Limit encoding to subset format");
        SetCLISwitchResultStatus(CLI, 5, false);
        
        SetCLISwitchFlag(CLI, 6, "Optimize", 8);
        SetCLISwitchDescription(CLI, 6, "Optimize encoded FLAC to be as small as possible");
        SetCLISwitchResultStatus(CLI, 6, false);
        
        return CLI;
    }
    
    void FLACDecodeFile(BitInput *BitI, BitOutput *BitO, DecodeFLAC *Dec, CommandLineInterface *CLI) {
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
    
    void FLACEncodeFile(BitInput *BitI, BitOutput *BitO, EncodeFLAC *Enc, CommandLineInterface *CLI) {
        Enc->EncodeSubset = GetCLISwitchPresence(CLI, 4);
        Enc->OptimizeFile = GetCLISwitchPresence(CLI, 5);
        // Start requesting PCM samples to encode into frames, given all PCM formats are interleaved, you'll need to handle that.
    }
    
    void CloseEverything(BitInput *BitI, BitOutput *BitO, PCMFile *PCM, CommandLineInterface *CLI, DecodeFLAC *Dec, EncodeFLAC *Enc) {
        CloseBitInput(BitI);
        CloseBitOutput(BitO);
        ClosePCMFile(PCM);
        CloseCommandLineInterface(CLI);
        CloseFLACDecoder(Dec);
        CloseFLACEncoder(Enc);
    }
    
    int main(int argc, const char *argv[]) {
        CommandLineInterface *CLI = SetModernFLACOptions();
        
        ParseCommandLineArguments(CLI, argc, argv);
        BitInput           *BitI    = InitBitInput();
        BitOutput          *BitO    = InitBitOutput();
        PCMFile            *PCM     = InitPCMFile();
        DecodeFLAC         *Dec     = InitDecodeFLAC();
        EncodeFLAC         *Enc     = InitEncodeFLAC();
        OpenCLIInputFile(BitI, CLI, 0);
        OpenCLIOutputFile(BitO, CLI, 1);
        
        bool Decode   = GetCLISwitchPresence(CLI, 2);
        bool Encode   = GetCLISwitchPresence(CLI, 3);
        bool Reencode = GetCLISwitchPresence(CLI, 4);
        bool Subset   = GetCLISwitchPresence(CLI, 5);
        
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
            Enc->EncodeSubset = GetCLISwitchPresence(CLI, 5);
            Enc->OptimizeFile = GetCLISwitchPresence(CLI, 6);
            IdentifyPCMFile(BitI, PCM);
            EncodeFLACFile(PCM, BitO, Enc);
            
            // Encode the file to FLAC
            // ParseWAV and encode FLAC
        } else {
            // Reencode the input
        }
        
        CloseEverything(BitI, BitO, PCM, CLI, Dec, Enc);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
