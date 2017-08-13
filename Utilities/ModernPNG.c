#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/CommandLineIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"

#include "../libModernPNG/include/libModernPNG.h"

#define ModernPNGVersion "0.5.0"

#ifdef __cplusplus
extern "C" {
#endif
    
    enum ModernPNGCommandLineSwitchNames {
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
        LogFile    = 10,
    };
    
    CommandLineIO *SetModernPNGOptions(void) {
        CommandLineIO *CLI = InitCommandLineIO(11);
        
        SetCLIName(CLI, "ModernPNG");
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLIVersion(CLI, ModernPNGVersion);
        SetCLICopyright(CLI, "2017 - 2017");
        SetCLIDescription(CLI, "PNG encoder/decoder written from scratch in modern C");
        SetCLILicense(CLI, "Revised BSD", "a truly free open source license", false);
        SetCLILicenseURL(CLI, "https://opensource.org/licenses/BSD-3-Clause");
        SetCLIMinSwitches(CLI, 3);
        
        SetCLISwitchFlag(CLI, Input, "Input");
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: -");
        SetCLISwitchResultStatus(CLI, Input, true);
        SetCLISwitchAsMain(CLI, Input, true);
        
        SetCLISwitchFlag(CLI, Output, "Output");
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: -");
        SetCLISwitchResultStatus(CLI, Output, true);
        SetCLISwitchAsMain(CLI, Output, true);
        
        SetCLISwitchFlag(CLI, LeftEye, "LeftEye");
        SetCLISwitchDescription(CLI, LeftEye, "The left view for encoding or decoding");
        SetCLISwitchResultStatus(CLI, LeftEye, true);
        
        SetCLISwitchFlag(CLI, RightEye, "RightEye");
        SetCLISwitchDescription(CLI, RightEye, "The right view for encoding or decoding");
        SetCLISwitchResultStatus(CLI, RightEye, true);
        
        /* General Meta Switches */
        SetCLISwitchAsChild(CLI, Input, LeftEye);
        SetCLISwitchAsChild(CLI, Input, RightEye);
        SetCLISwitchAsChild(CLI, Output, LeftEye);
        SetCLISwitchAsChild(CLI, Output, RightEye);
        
        /* Start Encode Options */
        SetCLISwitchFlag(CLI, Encode, "Encode");
        SetCLISwitchDescription(CLI, Encode, "Encode input to PNG");
        SetCLISwitchResultStatus(CLI, Encode, false);
        SetCLISwitchAsMain(CLI, Encode, true);
        
        SetCLISwitchFlag(CLI, Resolution, "Resolution");
        SetCLISwitchDescription(CLI, Resolution, "Resolution in WidthxHeight format (if 3D specify the per eye resolution)");
        SetCLISwitchResultStatus(CLI, Resolution, true);
        SetCLISwitchAsChild(CLI, Encode, Resolution);
        
        SetCLISwitchFlag(CLI, Interlace, "Interlace");
        SetCLISwitchDescription(CLI, Interlace, "Resolution in WidthxHeight format (if 3D specify the per eye resolution)");
        SetCLISwitchResultStatus(CLI, Interlace, true);
        SetCLISwitchAsChild(CLI, Encode, Interlace);
        
        SetCLISwitchFlag(CLI, Optimize, "Optimize");
        SetCLISwitchDescription(CLI, Optimize, "Optimize (try all filter options) the encoded PNG to be as small as possible");
        SetCLISwitchResultStatus(CLI, Optimize, false);
        SetCLISwitchAsChild(CLI, Encode, Optimize);
        /* End Encode Options */
        
        /* Start Decode Options */
        SetCLISwitchFlag(CLI, Decode, "Decode");
        SetCLISwitchDescription(CLI, Decode, "Decode PNG to output");
        SetCLISwitchResultStatus(CLI, Decode, false);
        SetCLISwitchAsMain(CLI, Decode, true);
        /* End Decode Options */
        
        SetCLISwitchFlag(CLI, Help, "Help");
        SetCLISwitchDescription(CLI, Help, "Prints all the command line options");
        SetCLISwitchResultStatus(CLI, Help, false);
        SetCLISwitchAsMain(CLI, Help, true);
        
        SetCLISwitchFlag(CLI, LogFile, "LogFile");
        SetCLISwitchDescription(CLI, LogFile, "Outputs the logs to a specified path");
        SetCLISwitchResultStatus(CLI, LogFile, false);
        SetCLISwitchAsMain(CLI, LogFile, true);
        
        return CLI;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineIO *CLI  = SetModernPNGOptions();
        ParseCommandLineArguments(CLI, argc, argv);
        
        uint64_t LogArgumentNum = GetCLISwitchNumFromFlag(CLI, LogFile);
        char    *LogFilePath    = GetCLIArgumentResult(CLI, LogArgumentNum);
        OpenLogFile(LogFilePath);
        
        /*
         So, the question is, does the Input switch have LeftEye or RightEye listed in the arguments?
         
         How do I generalize this to be included in CommandLineIO?
         
         */
        
        if ((GetCLISwitchPresence(CLI, LeftEye) && GetCLISwitchPresence(CLI, RightEye)) && GetCLISwitchPresence(CLI, Encode) == true) {
            // 3D image, so check for LeftEye/RightEye
            BitInput  *Lefteye     = InitBitInput();
            BitInput  *Righteye    = InitBitInput();
            BitOutput *Stereo      = InitBitOutput();
            
            uint64_t LeftEyeArg    = GetCLIChildSwitchArgument(CLI, Encode, LeftEye);
            uint64_t RightEyeArg   = GetCLIChildSwitchArgument(CLI, Encode, RightEye);
            uint64_t StereoArg     = GetCLIChildSwitchArgument(CLI, Encode, Output);
            
            OpenInputFile(Lefteye,   GetCLIArgumentResult(CLI, LeftEyeArg), false);
            OpenInputFile(Righteye,  GetCLIArgumentResult(CLI, RightEyeArg), false);
            OpenOutputFile(Stereo,   GetCLIArgumentResult(CLI, StereoArg));
        } else if ((GetCLISwitchPresence(CLI, LeftEye) && GetCLISwitchPresence(CLI, RightEye)) && GetCLISwitchPresence(CLI, Decode) == true) {
            BitInput  *Stereo      = InitBitInput();
            BitOutput *Lefteye     = InitBitOutput();
            BitOutput *Righteye    = InitBitOutput();
            
            uint64_t StereoArg     = GetCLIChildSwitchArgument(CLI, Decode, Output);
            uint64_t LeftEyeArg    = GetCLIChildSwitchArgument(CLI, Decode, LeftEye);
            uint64_t RightEyeArg   = GetCLIChildSwitchArgument(CLI, Decode, RightEye);
            
            OpenInputFile(Stereo,    GetCLIArgumentResult(CLI, StereoArg), false);
            OpenOutputFile(Lefteye,  GetCLIArgumentResult(CLI, LeftEyeArg));
            OpenOutputFile(Righteye, GetCLIArgumentResult(CLI, RightEyeArg));
        } else {
            // 2D image
            BitInput  *InputFile   = InitBitInput();
            BitOutput *OutputFile  = InitBitOutput();
            
            char       *InputPath  = GetCLIArgumentResult(CLI, Input);
            char      *OutputPath  = GetCLIArgumentResult(CLI, Output);
            
            OpenInputFile(InputFile, InputPath, false);
            OpenOutputFile(OutputFile, OutputPath);
        }
        
        DeinitCommandLineIO(CLI);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
