#include "../Dependencies/BitIO/libBitIO/include/CommandLineInterface.h"
#include "../Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../libModernPNG/include/libModernPNG.h"

#define ModernPNGVersion "0.1.0"

#ifdef __cplusplus
extern "C" {
#endif
    
    enum CommandLineSwitchNames {
        Input      = 0,
        Output     = 1,
        LeftEye    = 2,
        RightEye   = 3,
        Encode     = 4,
        Resolution = 5,
        Interlace  = 6,
        Optimize   = 7,
        Decode     = 8,
        Help       = 9,
    };
    
    CommandLineInterface *SetModernPNGOptions(void) {
        CommandLineInterface *CLI = InitCommandLineInterface(10);
        
        SetCLIName(CLI, "ModernPNG");
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLIVersion(CLI, ModernPNGVersion);
        SetCLICopyright(CLI, "2017 - 2017");
        SetCLIDescription(CLI, "PNG encoder/decoder written from scratch in modern C");
        SetCLILicense(CLI, "Revised BSD (3 clause)", false);
        SetCLILicenseURL(CLI, "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCLIMinSwitches(CLI, 3);
        
        SetCLISwitchFlag(CLI, Input, "Input", 5);
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: -");
        SetCLISwitchResultStatus(CLI, Input, true);
        
        SetCLISwitchFlag(CLI, Output, "Output", 6);
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: -");
        SetCLISwitchResultStatus(CLI, Output, true);
        
        SetCLISwitchFlag(CLI, LeftEye, "LeftEye", 7);
        SetCLISwitchDescription(CLI, LeftEye, "The left view for encoding or decoding");
        SetCLISwitchResultStatus(CLI, LeftEye, true);
        
        SetCLISwitchFlag(CLI, RightEye, "RightEye", 8);
        SetCLISwitchDescription(CLI, RightEye, "The right view for encoding or decoding");
        SetCLISwitchResultStatus(CLI, RightEye, true);
        
        /* Start Encode Options */
        SetCLISwitchFlag(CLI, Encode, "Encode", 6);
        SetCLISwitchDescription(CLI, Encode, "Encode input to PNG");
        SetCLISwitchResultStatus(CLI, Encode, false);
        
        SetCLISwitchFlag(CLI, Resolution, "Resolution", 10);
        SetCLISwitchDescription(CLI, Resolution, "Resolution in WidthxHeight format (if 3D specify the per eye resolution)");
        SetCLISwitchResultStatus(CLI, Resolution, true);
        
        SetCLISwitchFlag(CLI, Interlace, "Interlace", 10);
        SetCLISwitchDescription(CLI, Interlace, "Resolution in WidthxHeight format (if 3D specify the per eye resolution)");
        SetCLISwitchResultStatus(CLI, Interlace, true);
        
        SetCLISwitchFlag(CLI, Optimize, "Optimize", 8);
        SetCLISwitchDescription(CLI, Optimize, "Optimize the encoded PNG to be as small as possible (try all filter options)");
        SetCLISwitchResultStatus(CLI, Optimize, false);
        /* End Encode Options */
        
        /* Start Decode Options */
        SetCLISwitchFlag(CLI, Decode, "Decode", 6);
        SetCLISwitchDescription(CLI, Decode, "Decode PNG to output");
        SetCLISwitchResultStatus(CLI, Decode, false);
        /* End Decode Options */
        
        SetCLISwitchFlag(CLI, Help, "Help", 4);
        SetCLISwitchDescription(CLI, Help, "Prints all the command line options");
        SetCLISwitchResultStatus(CLI, Help, false);
        
        return CLI;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineInterface *CLI = SetModernPNGOptions();
        ParseCommandLineArguments(CLI, argc, argv);
        
        if ((GetCLISwitchPresence(CLI, LeftEye) && GetCLISwitchPresence(CLI, RightEye)) && GetCLISwitchPresence(CLI, Encode) == true) {
            // 3D image, so check for LeftEye/RightEye
            BitInput  *Lefteye    = InitBitInput();
            BitInput  *Righteye   = InitBitInput();
            BitOutput *Stereo     = InitBitOutput();
            
            uint64_t LeftEyeArg   = GetCLIMetaSwitchArgument(CLI, Encode, LeftEye);
            uint64_t RightEyeArg  = GetCLIMetaSwitchArgument(CLI, Encode, RightEye);
            uint64_t StereoArg    = GetCLIMetaSwitchArgument(CLI, Encode, Output);
            
            OpenInputFile(Lefteye,  GetCLIArgumentResult(CLI, LeftEyeArg));
            OpenInputFile(Righteye, GetCLIArgumentResult(CLI, RightEyeArg));
            OpenOutputFile(Stereo,  GetCLIArgumentResult(CLI, StereoArg));
        } else if ((GetCLISwitchPresence(CLI, LeftEye) && GetCLISwitchPresence(CLI, RightEye)) && GetCLISwitchPresence(CLI, Decode) == true) {
            BitInput  *Stereo     = InitBitInput();
            BitOutput *Lefteye    = InitBitOutput();
            BitOutput *Righteye   = InitBitOutput();
            
            uint64_t StereoArg    = GetCLIMetaSwitchArgument(CLI, Decode, Output);
            uint64_t LeftEyeArg   = GetCLIMetaSwitchArgument(CLI, Decode, LeftEye);
            uint64_t RightEyeArg  = GetCLIMetaSwitchArgument(CLI, Decode, RightEye);
            
            OpenInputFile(Stereo, GetCLIArgumentResult(CLI, StereoArg));
            OpenOutputFile(Lefteye, GetCLIArgumentResult(CLI, LeftEyeArg));
            OpenOutputFile(Righteye, GetCLIArgumentResult(CLI, RightEyeArg));
        } else {
            // 2D image
            BitInput  *InputFile  = InitBitInput();
            BitOutput *OutputFile = InitBitOutput();
            
            char       InputPath  = GetCLIArgumentResult(CLI, Input);
            char      OutputPath  = GetCLIArgumentResult(CLI, Output);
            
            OpenInputFile(InputFile, &InputPath);
            OpenOutputFile(OutputFile, &OutputPath);
        }
        
        DeinitCommandLineInterface(CLI);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
