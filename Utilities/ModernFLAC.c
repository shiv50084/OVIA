#include <stdio.h>
#include "../Dependencies/BitIO/libBitIO/include/BitIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"
#include "../libModernFLAC/include/Decoder/DecodeFLAC.h"
#include "../libModernFLAC/include/Encoder/EncodeFLAC.h"

void SetModernFLACOptions(CommandLineOptions *CMD) {
    CMD->NumSwitches                  = 7;
    CMD->ProgramName                  = "ModernFLAC";
    CMD->ProgramDescription           = "FLAC encoder/decoder written in modern C";
    CMD->AuthorCopyrightLicense       = "By BumbleBritches57, © 2017, Released under the BSD 3-clause license";
    
    CLSwitch *Switch0                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch1                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch2                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch3                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch4                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch5                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch6                 = calloc(sizeof(CLSwitch), 1);
    CMD->Switch[0]                    = Switch0;
    CMD->Switch[1]                    = Switch1;
    CMD->Switch[2]                    = Switch2;
    CMD->Switch[3]                    = Switch3;
    CMD->Switch[4]                    = Switch4;
    CMD->Switch[5]                    = Switch5;
    CMD->Switch[6]                    = Switch6;
    
    CMD->Switch[0]->Switch            = "-i";
    CMD->Switch[0]->SwitchDescription = "Input file or stdin with: '-'";
    
    CMD->Switch[1]->Switch            = "-o";
    CMD->Switch[1]->SwitchDescription = "Output file or stdout with: '-'";
    
    CMD->Switch[2]->Switch            = "-d";
    CMD->Switch[2]->SwitchDescription = "Decode input to output";
    CMD->Switch[2]->Resultless        = true;
    
    CMD->Switch[3]->Switch            = "-e";
    CMD->Switch[3]->SwitchDescription = "Encode input to output";
    CMD->Switch[3]->Resultless        = true;
    
    CMD->Switch[4]->Switch            = "-R";
    CMD->Switch[4]->SwitchDescription = "Reencodes the input flac with -O";
    CMD->Switch[4]->Resultless        = true;
    
    CMD->Switch[5]->Switch            = "-S";
    CMD->Switch[5]->SwitchDescription = "Limit encoding to subset format";
    CMD->Switch[5]->Resultless        = true;
    
    CMD->Switch[6]->Switch            = "-Optimize";
    CMD->Switch[6]->SwitchDescription = "Optimize encoded FLAC to be as small as possible";
    CMD->Switch[6]->Resultless        = true;
}

void FLACDecodeFile(BitInput *BitI, BitOutput *BitO, DecodeFLAC *Dec, CommandLineOptions *CMD) {
    uint32_t FileMagic = ReadBits(BitI, 32, true);
    
    if (FileMagic != FLACMagic) {
        // Not a FLAC file
        char Error[BitIOStringSize];
        snprintf(Error, BitIOStringSize, "Not a FLAC file, magic was: 0x%X\n", FileMagic);
        Log(LOG_ERR, "ModernFLAC", "FLACDecodeFile", Error);
    } else {
        for (size_t Byte = 4; Byte < BitI->FileSize; Byte++) { // loop to decode file
            while (Dec->LastMetadataBlock == false) {
                FLACParseMetadata(BitI, Dec);
            }
            if (ReadBits(BitI, 14, true) == FLACFrameMagic) {
                FLACReadFrame(BitI, Dec);
            }
        }
    }
}

void FLACEncodeFile(BitInput *BitI, BitOutput *BitO, FLACEncoder *Enc, CommandLineOptions *CMD) {
    Enc->EncodeSubset = CMD->Switch[4]->SwitchFound;
    Enc->OptimizeFile = CMD->Switch[5]->SwitchFound;
    // Start requesting PCM samples to encode into frames, given all PCM formats are interleaved, you'll need to handle that.
}

int main(int argc, const char *argv[]) {
    CommandLineOptions     *CMD     = calloc(sizeof(CommandLineOptions), 1);
    SetModernFLACOptions(CMD);
    if (argc < 5) {
        DisplayCMDHelp(CMD);
    } else {
        ParseCommandLineArguments(CMD, argc, argv);
        BitInput           *BitI    = calloc(sizeof(BitInput), 1);
        BitOutput          *BitO    = calloc(sizeof(BitOutput), 1);
        PCMFile            *PCM     = calloc(sizeof(PCMFile), 1);
        DecodeFLAC        *Dec     = calloc(sizeof(DecodeFLAC), 1);
        FLACEncoder        *Enc     = calloc(sizeof(FLACEncoder), 1);
        OpenCMDInputFile(BitI, CMD, 0);
        OpenCMDOutputFile(BitO, CMD, 1);
        InitDecodeFLAC(Dec);
        InitFLACEncoder(Enc);
        
        bool Decode   = CMD->Switch[2]->SwitchFound;
        bool Encode   = CMD->Switch[3]->SwitchFound;
        bool Reencode = CMD->Switch[4]->SwitchFound;
        bool Subset   = CMD->Switch[5]->SwitchFound;
        
        // Find out if -d or -e was included on the command line
        if (Decode == true || Reencode == true) {
            if (ReadBits(BitI, 32, true) == FLACMagic) {
                for (uint8_t Byte = 0; Byte < BitI->FileSize; Byte++) {
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
            Enc->EncodeSubset = CMD->Switch[5]->SwitchFound;
            Enc->OptimizeFile = CMD->Switch[6]->SwitchFound;
            IdentifyPCMFile(BitI, PCM);
            EncodeFLAC(PCM, BitI, Enc);
            
            //FLACEncodeFrame();
            
            // Encode the file to FLAC
            // ParseWAV and encode FLAC
        } else {
            // Reencode the input
        }
    }
    
    return EXIT_SUCCESS;
}
