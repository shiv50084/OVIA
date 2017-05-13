#include "../Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../libModernPNG/include/libModernPNG.h"

#define ModernFLACVersion "0.1.0"

#ifdef __cplusplus
extern "C" {
#endif
    
    CommandLineOptions *SetModernPNGOptions(void) {
        CommandLineOptions *CMD = InitCommandLineOptions(7);
        
        SetCMDName(CMD, "ModernPNG");
        SetCMDVersion(CMD, ModernFLACVersion);
        SetCMDAuthor(CMD, "BumbleBritches57");
        SetCMDCopyright(CMD, "2017-2017");
        SetCMDDescription(CMD, "PNG encoder/decoder written from scratch in modern C");
        SetCMDLicense(CMD, "Revised BSD (3 clause)", false);
        SetCMDLicenseURL(CMD, "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCMDMinSwitches(CMD, 3);
        
        SetSwitchFlag(CMD, 0, "Input", 5);
        SetSwitchDescription(CMD, 0, "Input file or stdin with: -");
        SetSwitchResultStatus(CMD, 0, false);
        
        SetSwitchFlag(CMD, 1, "Output", 6);
        SetSwitchDescription(CMD, 1, "Output file or stdout with: -");
        SetSwitchResultStatus(CMD, 1, false);
        
        SetSwitchFlag(CMD, 2, "Resolution", 10);
        SetSwitchDescription(CMD, 2, "Resolution in WidthxHeight format");
        SetSwitchResultStatus(CMD, 2, false);
        
        SetSwitchFlag(CMD, 3, "Encode", 6);
        SetSwitchDescription(CMD, 4, "Encode input to PNG");
        SetSwitchResultStatus(CMD, 4, true);
        
        SetSwitchFlag(CMD, 4, "Decode", 6);
        SetSwitchDescription(CMD, 3, "Decode PNG to output");
        SetSwitchResultStatus(CMD, 3, true);
        
        SetSwitchFlag(CMD, 5, "Optimize", 8); // Should quotes be included with the string in strlen?
        SetSwitchDescription(CMD, 5, "Optimize the encoded PNG to be as small as possible (try all filter options)");
        SetSwitchResultStatus(CMD, 5, true);
        
        SetSwitchFlag(CMD, 6, "Help", 4);
        SetSwitchDescription(CMD, 6, "Prints all the command line options");
        SetSwitchResultStatus(CMD, 6, true);
        
        // Switch 6 is NULL, aka the auto Help option. Fuck it, the user will just have to code that themselves.
        
        return CMD;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineOptions *CMD  = SetModernPNGOptions();
        BitInput           *BitI = InitBitInput();
        BitOutput          *BitO = InitBitOutput();
        
        BitBuffer          *InputBitB = InitBitBuffer();
        BitBuffer          *OutputBitB = InitBitBuffer();
        
        // We should add a function to BitIO that can tell us if an input command is a network address or a file.
        
        ParseCommandLineArguments(CMD, argc, argv);
        if        (GetSwitchPresence(CMD, 2) == true) {
            // Split string into resolution.
        } else if (GetSwitchPresence(CMD, 3) == true) { // Encode
            EncodePNG     *Enc   = InitEncodePNG();
            PNGEncodeImage(Enc, BitO);
        } else if (GetSwitchPresence(CMD, 4) == true) { // Decode
            DecodePNG     *Dec   = InitDecodePNG();
            PNGDecodeImage(InputBitB, Dec, NULL);
        }
        
        CloseCommandLineOptions(CMD);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
