#include "../Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../libModernPNG/include/libModernPNG.h"
#include "../libModernPNG/include/Decoder/DecodePNG.h"
#include "../libModernPNG/include/Encoder/EncodePNG.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    CommandLineOptions *SetModernPNGOptions(void) {
        CommandLineOptions *CMD           = InitCommandLineOptions();
        InitCommandLineSwitches(CMD, 6);
        
        // test the logger
        //Log(LOG_ERR, "libBitIO", "SetSwitchResultStatus", "SwitchNum: %d, should be between 0 and %d\n", 0, 43);
        
        SetCMDName(CMD, "ModernPNG");
        SetCMDDescription(CMD, "PNG encoder/decoder written in modern C, from scratch");
        SetCMDAuthor(CMD, "BumbleBritches57");
        SetCMDCopyright(CMD, "2017-2017");
        SetCMDLicense(CMD, "Revised BSD (3 clause)");
        
        SetSwitchFlag(CMD, 0, "Input");
        SetSwitchDescription(CMD, 0, "Input file or stdin with: '-'\n");
        SetSwitchResultStatus(CMD, 0, false);
        
        SetSwitchFlag(CMD, 1, "Output");
        SetSwitchDescription(CMD, 1, "Output file or stdout with: '-'\n");
        SetSwitchResultStatus(CMD, 1, false);
        
        SetSwitchFlag(CMD, 2, "Res");
        SetSwitchDescription(CMD, 2, "Resolution in WidthxHeight format\n");
        SetSwitchResultStatus(CMD, 2, false);
        
        SetSwitchFlag(CMD, 3, "Decode");
        SetSwitchDescription(CMD, 3, "Decode PNG to output\n");
        SetSwitchResultStatus(CMD, 3, true);
        
        SetSwitchFlag(CMD, 4, "Encode");
        SetSwitchDescription(CMD, 4, "Encode input to PNG\n");
        SetSwitchResultStatus(CMD, 4, true);
        
        SetSwitchFlag(CMD, 5, "Optimize");
        SetSwitchDescription(CMD, 5, "Optimize the encoded PNG to be as small as possible (try all filter options)\n");
        SetSwitchResultStatus(CMD, 5, true);
        
        return CMD;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineOptions *CMD  = SetModernPNGOptions();
        BitInput           *BitI =  InitBitInput();
        BitOutput          *BitO = InitBitOutput();
        
        ParseCommandLineArguments(CMD, argc, argv);
        if        (IsSwitchPresent(CMD, 2) == true) {
            // Split string into resolution.
        } else if (IsSwitchPresent(CMD, 3) == true) {
            DecodePNG     *Dec   = InitDecodePNG();
            PNGDecodeImage(BitI, Dec, NULL);
        } else if (IsSwitchPresent(CMD, 4) == true) {
            EncodePNG     *Enc   = InitEncodePNG();
            PNGEncodeImage(Enc, BitO);
        }
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
