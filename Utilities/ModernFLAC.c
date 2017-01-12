#include "../libModernFLAC/include/DecodeFLAC.h"
#include "../libModernFLAC/include/ParseFLAC.h"

#include "../libModernFLAC/include/EncodeFLAC.h"

void ShowOptions(CommandLineOptions *CMD) {
    CMD->NumSwitches                  = 6;
    CMD->ProgramName                  = "ModernFLAC";
    CMD->ProgramDescription           = "FLAC encoder/decoder written in modern C for improved readability, maintence, and performance";
    CMD->AuthorCopyrightLicense       = "By BumbleBritches57, © 2017, Released under the BSD 3-clause license";
    
    CLSwitch *Switch0                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch1                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch2                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch3                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch4                 = calloc(sizeof(CLSwitch), 1);
    CLSwitch *Switch5                 = calloc(sizeof(CLSwitch), 1);
    CMD->Switch[0]                    = Switch0;
    CMD->Switch[1]                    = Switch1;
    CMD->Switch[2]                    = Switch2;
    CMD->Switch[3]                    = Switch3;
    CMD->Switch[4]                    = Switch4;
    CMD->Switch[5]                    = Switch5;
    
    CMD->Switch[0]->Switch            = "-d";
    CMD->Switch[0]->SwitchDescription = "Decode input to output";
    
    CMD->Switch[1]->Switch            = "-e";
    CMD->Switch[1]->SwitchDescription = "Encode input to output";
    
    CMD->Switch[2]->Switch            = "-i";
    CMD->Switch[2]->SwitchDescription = "Input file or stdin with: -";
    
    CMD->Switch[3]->Switch            = "-o";
    CMD->Switch[3]->SwitchDescription = "Output file or stdout with: -";
    
    CMD->Switch[4]->Switch            = "--Subset";
    CMD->Switch[4]->SwitchDescription = "Limit encoding to subset format";
    
    CMD->Switch[5]->Switch            = "--Optimize";
    CMD->Switch[5]->SwitchDescription = "Optimize encoded FLAC to be as small as possible (encoding only, does not ignore or override -Subset if set)";
}

void FLACDecodeFile(CommandLineOptions *CMD, int argc, const char *argv[]) {
    //CommandLineOptions *CMD     = calloc(sizeof(CommandLineOptions), 1);
    BitInput           *BitI    = calloc(sizeof(BitInput), 1);
    BitOutput          *BitO    = calloc(sizeof(BitOutput), 1);
    ErrorStatus        *Error   = calloc(sizeof(ErrorStatus), 1);
    FLACDecoder        *Dec     = calloc(sizeof(FLACDecoder), 1);
    
    InitBitInput(BitI, Error, argc, argv);
    InitBitOutput(BitO, Error, argc, argv);
    InitFLACDecoder(Dec);
    
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
            FLACReadFrame(BitI, Dec);
        }
    }
}

int main(int argc, const char *argv[]) {
    CommandLineOptions *CMD = calloc(sizeof(CommandLineOptions), 1);
    if (argc < 5 || strcasecmp(*argv, "-h") == 0 || strcasecmp(*argv, "--help") || strcasecmp(*argv, "/?")) {
        ShowOptions(CMD);
    }
    
    FLACDecodeFile(CMD, argc, argv);
    return EXIT_SUCCESS;
}
