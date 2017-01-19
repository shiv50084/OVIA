#include "../libModernFLAC/include/DecodeFLAC.h"
#include "../libModernFLAC/include/ParseFLAC.h"

#include "../libModernFLAC/include/EncodeFLAC.h"

void SetModernFLACOptions(CommandLineOptions *CMD) {
    CMD->NumSwitches                  = 7;
    CMD->ProgramName                  = "ModernFLAC";
    CMD->ProgramDescription           = "FLAC encoder/decoder written in modern C for improved readability, maintence, and performance";
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
    CMD->Switch[0]->SwitchDescription = "Input file or stdin with: -";
    
    CMD->Switch[1]->Switch            = "-o";
    CMD->Switch[1]->SwitchDescription = "Output file or stdout with: -";
    
    CMD->Switch[2]->Switch            = "-d";
    CMD->Switch[2]->SwitchDescription = "Decode input to output";
    
    CMD->Switch[3]->Switch            = "-e";
    CMD->Switch[3]->SwitchDescription = "Encode input to output";
    
    CMD->Switch[4]->Switch            = "--Reencode";
    CMD->Switch[4]->SwitchDescription = "Reencodes the input flac with --Subset --Optimize";
    
    CMD->Switch[5]->Switch            = "--Subset";
    CMD->Switch[5]->SwitchDescription = "Limit encoding to subset format";
    
    CMD->Switch[6]->Switch            = "--Optimize";
    CMD->Switch[6]->SwitchDescription = "Optimize encoded FLAC to be as small as possible (encoding only, does not ignore or override -Subset if set)";
}

void FLACDecodeFile(BitInput *BitI, BitOutput *BitO, FLACDecoder *Dec, CommandLineOptions *CMD) {
    
    uint32_t FileMagic = ReadBits(BitI, 32);
    
    if (FileMagic != FLACMagic) {
        // Not a FLAC file
        char Error[BitIOStringSize];
        snprintf(Error, BitIOStringSize, "Not a FLAC file, magic was: 0x%X\n", FileMagic);
        Log(SYSError, "ModernFLAC", "FLACDecodeFile", Error);
    } else {
        bool IsLastBlock = false;
        for (size_t Byte = 4; Byte < BitI->FileSize; Byte++) { // loop to decode file
            while (IsLastBlock == false) {
                IsLastBlock = FLACParseMetadata(BitI, Dec);
            }
            if (ReadBits(BitI, 14) == FLACFrameMagic) {
                FLACReadFrame(BitI, Dec);
            }
        }
    }
}

void FLACEncodeFile(BitInput *BitI, BitOutput *BitO, FLACEncoder *Enc, CommandLineOptions *CMD) {
    Enc->EncodeSubset = CMD->Switch[4]->SwitchFound;
    Enc->OptimizeFile = CMD->Switch[5]->SwitchFound;
    // I'm not adding padding to the encoded FLAC file metadata section, because editors have to support that case anyway.
    // So, I also need to include
}

int main(int argc, const char *argv[]) {
    CommandLineOptions *CMD = calloc(sizeof(CommandLineOptions), 1);
    SetModernFLACOptions(CMD);
    if (argc < 5) {
        DisplayCMDHelp(CMD);
    }
    BitInput           *BitI    = calloc(sizeof(BitInput), 1);
    BitOutput          *BitO    = calloc(sizeof(BitOutput), 1);
    ErrorStatus        *Error   = calloc(sizeof(ErrorStatus), 1);
    FLACDecoder        *Dec     = calloc(sizeof(FLACDecoder), 1);
    FLACEncoder        *Enc     = calloc(sizeof(FLACEncoder), 1);
    InitBitInput(BitI, Error, argc, argv);
    //InitBitOutput(BitO, Error, argc, argv);
    InitFLACDecoder(Dec);
    //InitFLACEncoder(Enc);
    
    bool Decode   = 1; //CMD->Switch[2]->SwitchResult;
    bool Encode   = CMD->Switch[3]->SwitchResult;
    bool Reencode = CMD->Switch[4]->SwitchFound;
    
    // Find out if -d or -e was included on the command line
    if (Decode == true || Reencode == true) {
        if (ReadBits(BitI, 32) == FLACMagic) {
            bool IsLastMetadataBlock = false;
            while (IsLastMetadataBlock == false) {
                IsLastMetadataBlock = FLACParseMetadata(BitI, Dec);
            }
            
        }
        // Decode the file.
        // To decode we'll need to init the FLACDecoder, and output the stuff to wav or w64
    } else if (Encode == true) {
        // Encode the file to FLAC
        // ParseWAV and encode FLAC
    } else {
        // Reencode the input
    }
    
    // Find out what kind of file has been passed in and what the user wants us to do.
    return EXIT_SUCCESS;
}
