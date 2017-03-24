#include "../Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../libModernPNG/include/libModernPNG.h"
#include "../libModernPNG/include/Decoder/DecodePNG.h"
#include "../libModernPNG/include/Encoder/EncodePNG.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void SetModernPNGOptions(CommandLineOptions *CMD) {
        CMD->NumSwitches                  = 5;
        CMD->ProgramName                  = "ModernPNG";
        CMD->ProgramDescription           = "PNG encoder/decoder written in modern C, from scratch";
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
        
        CMD->Switch[0]->Switch            = "-i";
        CMD->Switch[0]->SwitchDescription = "Input file or stdin with: '-'\n";
        
        CMD->Switch[1]->Switch            = "-o";
        CMD->Switch[1]->SwitchDescription = "Output file or stdout with: '-'\n";
        
        CMD->Switch[2]->Switch            = "-Res";
        CMD->Switch[2]->SwitchDescription = "Resolution in WidthxHeight format\n";
        CMD->Switch[2]->Resultless        = true;
        
        CMD->Switch[3]->Switch            = "-d";
        CMD->Switch[3]->SwitchDescription = "Decode PNG to output\n";
        CMD->Switch[3]->Resultless        = true;
        
        CMD->Switch[4]->Switch            = "-e";
        CMD->Switch[4]->SwitchDescription = "Encode input to PNG\n";
        CMD->Switch[4]->Resultless        = true;
        
        CMD->Switch[5]->Switch            = "-Optimize";
        CMD->Switch[5]->SwitchDescription = "Optimize the encoded PNG to be as small as possible (try all filter options)\n";
        CMD->Switch[5]->Resultless        = true;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineOptions *CMD   = calloc(sizeof(CommandLineOptions), 1);
        SetModernPNGOptions(CMD);
        if (argc < 5) {
            DisplayCMDHelp(CMD);
        } else {
            ParseCommandLineArguments(CMD, argc, argv);
            BitInput           *BitI = InitBitInput();
            BitOutput          *BitO = InitBitOutput();
            if        (IsSwitchPresent(CMD, 2) == true) {
                // Split string into resolution.
            } else if (IsSwitchPresent(CMD, 3) == true) {
                DecodePNG     *Dec   = InitPNGDecoder();
                DecodePNGImage(BitI, Dec, NULL);
            } else if (IsSwitchPresent(CMD, 4) == true) {
                EncodePNG     *Enc   = InitPNGEncoder();
                EncodePNGImage();
            }
        }
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
