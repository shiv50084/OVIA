#include "../Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../libModernPNG/include/libModernPNG.h"

#define ModernPNGVersion "0.1.0"

#ifdef __cplusplus
extern "C" {
#endif
    
    CommandLineOptions *SetModernPNGOptions(void) {
        CommandLineOptions *CMD = InitCommandLineOptions(7);
        
        SetCMDName(CMD, "ModernPNG");
        SetCMDVersion(CMD, ModernPNGVersion);
        SetCMDAuthor(CMD, "BumbleBritches57");
        SetCMDCopyright(CMD, "2017-2017");
        SetCMDDescription(CMD, "PNG encoder/decoder written from scratch in modern C");
        SetCMDLicense(CMD, "Revised BSD (3 clause)", false);
        SetCMDLicenseURL(CMD, "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCMDMinSwitches(CMD, 3);
        
        SetCMDSwitchFlag(CMD, 0, "Input", 5);
        SetCMDSwitchDescription(CMD, 0, "Input file or stdin with: -");
        SetCMDSwitchResultStatus(CMD, 0, true);
        
        SetCMDSwitchFlag(CMD, 1, "Output", 6);
        SetCMDSwitchDescription(CMD, 1, "Output file or stdout with: -");
        SetCMDSwitchResultStatus(CMD, 1, true);
        
        /* Start Encode Options */
        SetCMDSwitchFlag(CMD, 2, "Encode", 6);
        SetCMDSwitchDescription(CMD, 2, "Encode input to PNG");
        SetCMDSwitchResultStatus(CMD, 2, false);
        
        SetCMDSwitchFlag(CMD, 3, "Resolution", 10);
        SetCMDSwitchDescription(CMD, 3, "Resolution in WidthxHeight format (if 3D specify the per eye resolution)");
        SetCMDSwitchResultStatus(CMD, 3, true);
        
        SetCMDSwitchFlag(CMD, 4, "Interlace", 10);
        SetCMDSwitchDescription(CMD, 4, "Resolution in WidthxHeight format (if 3D specify the per eye resolution)");
        SetCMDSwitchResultStatus(CMD, 4, true);
        
        SetCMDSwitchFlag(CMD, 5, "Optimize", 8);
        SetCMDSwitchDescription(CMD, 5, "Optimize the encoded PNG to be as small as possible (try all filter options)");
        SetCMDSwitchResultStatus(CMD, 5, false);
        
        SetCMDSwitchFlag(CMD, 5, "Stereo3D", 8);
        SetCMDSwitchDescription(CMD, 5, "Encode an image as a single stereoscopic, 3D image (the first option should be the left eye)");
        SetCMDSwitchResultStatus(CMD, 5, false);
        /* End Encode Options */
        
        /* Start Decode Options */
        SetCMDSwitchFlag(CMD, 6, "Decode", 6);
        SetCMDSwitchDescription(CMD, 6, "Decode PNG to output");
        SetCMDSwitchResultStatus(CMD, 6, false);
        
        /* Enc Decode Options */
        
        
        
        SetCMDSwitchFlag(CMD, 7, "Help", 4);
        SetCMDSwitchDescription(CMD, 7, "Prints all the command line options");
        SetCMDSwitchResultStatus(CMD, 7, false);
        
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
        if        (GetCMDSwitchPresence(CMD, 2) == true) {
            // Split string into resolution.
        } else if (GetCMDSwitchPresence(CMD, 3) == true) { // Encode
            EncodePNG     *Enc   = InitEncodePNG();
            PNGEncodeImage(Enc, BitO);
        } else if (GetCMDSwitchPresence(CMD, 4) == true) { // Decode
            DecodePNG     *Dec   = InitDecodePNG();
            PNGDecodeImage(InputBitB, Dec, NULL);
        }
        
        CloseCommandLineOptions(CMD);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
