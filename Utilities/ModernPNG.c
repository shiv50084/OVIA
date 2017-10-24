#include "../libModernPNG/include/libModernPNG.h"

#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/CommandLineIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"

char *ModernPNGVersion = "0.5.0";

#ifdef __cplusplus
extern "C" {
#endif
    
    enum ModernPNGCommandLineSwitchNames {
        /* General Options */
        Input       = 0,
        Output      = 1,
        LogFile     = 2,
        LeftEye     = 3,
        RightEye    = 4,
        /* Encode Options */
        Encode      = 5,
        Interlace   = 6,
        Optimize    = 7,
        /* Decode Options */
        Decode      = 8,
        /* Metadata Options */
        InsertMeta  = 9,
        ExtractMeta = 10,
        RemoveMeta  = 11,
        /* Types of Meta to Insert, Extract, or Remove */
        ICCProfile  = 12,
        Gamma       = 13,
        Text        = 14,
        Histogram   = 15,
        /* Help Option */
        Help        = 16,
    };
    
    static CommandLineIO *SetModernPNGOptions(void) {
        CommandLineIO *CLI = CommandLineIOInit(17);
        
        SetCLIName(CLI, "ModernPNG");
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLIVersion(CLI, ModernPNGVersion);
        SetCLICopyright(CLI, "2017 - 2017");
        SetCLIDescription(CLI, "PNG encoder/decoder written from scratch in modern C");
        SetCLILicense(CLI, "Revised BSD", "A permissive open source license", "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCLIMinArguments(CLI, 3);
        SetCLIHelpSwitch(CLI, Help);
        
        SetCLISwitchFlag(CLI, Input, "Input");
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: -");
        SetCLISwitchAsIndependent(CLI, Input);
        
        SetCLISwitchFlag(CLI, Output, "Output");
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: -");
        SetCLISwitchAsIndependent(CLI, Output);
        
        SetCLISwitchFlag(CLI, LogFile, "LogFile");
        SetCLISwitchDescription(CLI, LogFile, "Outputs the logs to a specified path");
        SetCLISwitchAsIndependent(CLI, LogFile);
        
        SetCLISwitchFlag(CLI, LeftEye, "LeftEye");
        SetCLISwitchDescription(CLI, LeftEye, "The left view for encoding or decoding");
        SetCLISwitchAsDependent(CLI, Input, LeftEye);
        SetCLISwitchAsDependent(CLI, Output, LeftEye);
        
        SetCLISwitchFlag(CLI, RightEye, "RightEye");
        SetCLISwitchDescription(CLI, RightEye, "The right view for encoding or decoding");
        SetCLISwitchAsDependent(CLI, Input, RightEye);
        SetCLISwitchAsDependent(CLI, Output, RightEye);
        
        /* Start Encode Options */
        SetCLISwitchFlag(CLI, Encode, "Encode");
        SetCLISwitchDescription(CLI, Encode, "Encode input(s) to PNG");
        SetCLISwitchAsIndependent(CLI, Encode);
        
        SetCLISwitchFlag(CLI, Interlace, "Interlace");
        SetCLISwitchDescription(CLI, Interlace, "Should we encode the PNG with Adam7 Interlacing? (Currently not supported)");
        SetCLISwitchAsDependent(CLI, Encode, Interlace);
        
        SetCLISwitchFlag(CLI, Optimize, "Optimize");
        SetCLISwitchDescription(CLI, Optimize, "Optimize (try all filter options) the encoded PNG to be as small as possible");
        SetCLISwitchAsDependent(CLI, Encode, Optimize);
        /* End Encode Options */
        
        /* Start Decode Options */
        SetCLISwitchFlag(CLI, Decode, "Decode");
        SetCLISwitchDescription(CLI, Decode, "Decode PNG to output");
        SetCLISwitchAsIndependent(CLI, Decode);
        /* End Decode Options */
        
        /* Metadata Options */
        SetCLISwitchFlag(CLI, InsertMeta, "InsertMeta");
        SetCLISwitchDescription(CLI, InsertMeta, "Adds metadata to PNG file");
        SetCLISwitchAsIndependent(CLI, InsertMeta);
        
        SetCLISwitchFlag(CLI, ExtractMeta, "ExtractMeta");
        SetCLISwitchDescription(CLI, ExtractMeta, "Extracts metadata from PNG file");
        SetCLISwitchAsIndependent(CLI, ExtractMeta);
        
        SetCLISwitchFlag(CLI, RemoveMeta, "RemoveMeta");
        SetCLISwitchDescription(CLI, RemoveMeta, "Removes metadata from PNG file");
        SetCLISwitchAsIndependent(CLI, RemoveMeta);
        
        SetCLISwitchFlag(CLI, Help, "Help");
        SetCLISwitchDescription(CLI, Help, "Prints all the command line options");
        SetCLISwitchAsIndependent(CLI, Help);
        
        return CLI;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineIO *CLI  = SetModernPNGOptions();
        if (CLI == NULL) {
            exit(EXIT_FAILURE);
        } else {
            ParseCommandLineArguments(CLI, argc, argv);
            
            bool Encode3D                        = false;
            bool Decode3D                        = false;
            bool Encode2D                        = false;
            bool Decode2D                        = false;
            
            BitInput  *InputLeftEye              = NULL;
            BitInput  *InputRightEye             = NULL;
            BitInput  *Input3D                   = NULL;
            BitInput  *Input2D                   = NULL;
            BitOutput *OutputLeftEye             = NULL;
            BitOutput *OutputRightEye            = NULL;
            BitOutput *Output3D                  = NULL;
            BitOutput *Output2D                  = NULL;
            
            bool     EncodePNG                   = GetCLIArgumentPresenceFromSwitch(CLI, Encode);
            bool     DecodePNG                   = GetCLIArgumentPresenceFromSwitch(CLI, Decode);
            uint64_t NumInputFiles               = GetCLINumArgumentsWithIndependentAndDependents(CLI, Input, 0);
            uint64_t NumOutputFiles              = GetCLINumArgumentsWithIndependentAndDependents(CLI, Output, 0);
            
            if (EncodePNG == true) { // If we're supposed to Encode files to PNG, do this.
                if (NumInputFiles == 1 && NumOutputFiles == 1) {
                    Encode2D = true;
                    /* 2D Input */
                    uint64_t  Input2DArg         = GetCLIArgumentNumWithIndependentAndDependents(CLI, Input, 0);
                    char     *Input2DPath        = GetCLIArgumentResult(CLI, Input2DArg);
                    Input2D                      = BitInputInit();
                    BitInputOpenFile(Input2D, Input2DPath);
                    /* 2D Input */
                    
                    /* 2D Output */
                    uint64_t  Output2DArg        = GetCLIArgumentNumWithIndependentAndDependents(CLI, Output, 0);
                    char     *Output2DPath       = GetCLIArgumentResult(CLI, Output2DArg);
                    Output2D                     = BitOutputInit();
                    BitOutputOpenFile(Output2D, Output2DPath);
                    /* 2D Output */
                } else if (NumInputFiles == 2 && NumOutputFiles == 1) {
                    Encode3D = true;
                    /* LeftEye Input */
                    uint64_t InputLeftEyeArg     = GetCLIArgumentNumWithIndependentAndDependents(CLI, Input, 1, LeftEye);
                    char *InputLeftEyePath       = GetCLIArgumentResult(CLI, InputLeftEyeArg);
                    InputLeftEye                 = BitInputInit();
                    BitInputOpenFile(InputLeftEye, InputLeftEyePath);
                    /*  LeftEye Input */
                    
                    /*  RightEye Input */
                    uint64_t InputRightEyeArg    = GetCLIArgumentNumWithIndependentAndDependents(CLI, Input, 1, RightEye);
                    char *InputRightEyePath      = GetCLIArgumentResult(CLI, InputRightEyeArg);
                    InputRightEye                = BitInputInit();
                    BitInputOpenFile(InputRightEye, InputRightEyePath);
                    /*  RightEye Input */
                    
                    /* 3D Output */
                    uint64_t  Output3DArg        = GetCLIArgumentNumWithIndependentAndDependents(CLI, Output, 0);
                    char     *Output3DPath       = GetCLIArgumentResult(CLI, Output3DArg);
                    Output3D                     = BitOutputInit();
                    BitOutputOpenFile(Output3D, Output3DPath);
                    /* 3D Output */
                } else {
                    Log(LOG_ERR, "ModernPNG", "Main", "Too few/many Input %d or Output %d files, min is 1, max is 2", NumInputFiles, NumOutputFiles);
                }
            } else if (DecodePNG == true) { // If we're supposed to Decode a PNG file, do this.
                if (NumInputFiles == 1 && NumOutputFiles == 1) {
                    Decode2D = true;
                    /* 2D Input */
                    uint64_t  Input2DArg         = GetCLIArgumentNumWithIndependentAndDependents(CLI, Input, 0);
                    char     *Input2DPath        = GetCLIArgumentResult(CLI, Input2DArg);
                    Input2D                      = BitInputInit();
                    BitInputOpenFile(Input2D, Input2DPath);
                    /* 2D Input */
                    
                    /* 2D Output */
                    uint64_t  Output2DArg        = GetCLIArgumentNumWithIndependentAndDependents(CLI, Output, 0);
                    char     *Output2DPath       = GetCLIArgumentResult(CLI, Output2DArg);
                    Output2D                     = BitOutputInit();
                    BitOutputOpenFile(Output2D, Output2DPath);
                    /* 2D Output */
                } else if (NumInputFiles == 1 && NumOutputFiles == 2) {
                    Decode3D = true;
                    /* 3D Input */
                    uint64_t  Input3DArg         = GetCLIArgumentNumWithIndependentAndDependents(CLI, Input, 0);
                    char     *Input3DPath        = GetCLIArgumentResult(CLI, Input3DArg);
                    Input3D                      = BitInputInit();
                    BitInputOpenFile(Input3D, Input3DPath);
                    /* 3D Input */
                    
                    /* LeftEye Output */
                    uint64_t  OutputLeftEyeArg   = GetCLIArgumentNumWithIndependentAndDependents(CLI, Output, 1, LeftEye);
                    char     *OutputLeftEyePath  = GetCLIArgumentResult(CLI, OutputLeftEyeArg);
                    OutputLeftEye                = BitOutputInit();
                    BitOutputOpenFile(OutputLeftEye, OutputLeftEyePath);
                    /* LeftEye Output */
                    
                    /*  RightEye Output */
                    uint64_t  OutputRightEyeArg  = GetCLIArgumentNumWithIndependentAndDependents(CLI, Output, 1, RightEye);
                    char     *OutputRightEyePath = GetCLIArgumentResult(CLI, OutputRightEyeArg);
                    OutputRightEye               = BitOutputInit();
                    BitOutputOpenFile(OutputRightEye, OutputRightEyePath);
                    /*  RightEye Output */
                } else {
                    Log(LOG_ERR, "ModernPNG", "Main", "Too few/many Input %d or Output %d files, min is 1, max is 2", NumInputFiles, NumOutputFiles);
                }
            }
        }
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
