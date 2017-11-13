#include "../libModernPNG/include/libModernPNG.h"

#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/CommandLineIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"

static const char *ModernPNGVersion     = "0.5.0";
static const char *ModernPNGProgramName = "ModernPNG";

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
        NumOptions  = 17,
    };
    
    static CommandLineIO *SetModernPNGOptions(void) {
        CommandLineIO *CLI = CommandLineIO_Init(NumOptions);
        
        CLISetName(CLI, "ModernPNG");
        CLISetAuthor(CLI, "BumbleBritches57");
        CLISetVersion(CLI, ModernPNGVersion);
        CLISetCopyright(CLI, "2017 - 2017");
        CLISetDescription(CLI, "PNG encoder/decoder written from scratch in modern C");
        CLISetLicense(CLI, "Revised BSD", "A permissive open source license", "https://opensource.org/licenses/BSD-3-Clause", false);
        CLISetMinArguments(CLI, 3);
        CLISetHelpSwitch(CLI, Help);
        
        CLISetSwitchFlag(CLI, Input, "Input");
        CLISetSwitchDescription(CLI, Input, "Input file or stdin with: -");
        CLISetSwitchType(CLI, Input, MasterSwitch);
        
        CLISetSwitchFlag(CLI, Output, "Output");
        CLISetSwitchDescription(CLI, Output, "Output file or stdout with: -");
        CLISetSwitchType(CLI, Output, MasterSwitch);
        
        CLISetSwitchFlag(CLI, LogFile, "LogFile");
        CLISetSwitchDescription(CLI, LogFile, "Outputs the logs to a specified path");
        CLISetSwitchType(CLI, LogFile, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, LeftEye, "LeftEye");
        CLISetSwitchDescription(CLI, LeftEye, "The left view for encoding or decoding");
        CLISetSwitchType(CLI, LeftEye, SlaveSwitch);
        
        CLISetSwitchFlag(CLI, RightEye, "RightEye");
        CLISetSwitchDescription(CLI, RightEye, "The right view for encoding or decoding");
        CLISetSwitchType(CLI, RightEye, SlaveSwitch);
        
        CLISetSwitchAsSlave(CLI, Input, LeftEye);
        CLISetSwitchAsSlave(CLI, Input, RightEye);
        CLISetSwitchAsSlave(CLI, Output, LeftEye);
        CLISetSwitchAsSlave(CLI, Output, RightEye);
        
        /* Start Encode Options */
        CLISetSwitchFlag(CLI, Encode, "Encode");
        CLISetSwitchDescription(CLI, Encode, "Encode input(s) to PNG");
        CLISetSwitchType(CLI, Encode, MasterSwitch);
        
        CLISetSwitchFlag(CLI, Interlace, "Interlace");
        CLISetSwitchDescription(CLI, Interlace, "Should we encode the PNG with Adam7 Interlacing? (Currently not supported)");
        CLISetSwitchType(CLI, Interlace, SingleSwitchNoResult);
        CLISetSwitchAsSlave(CLI, Encode, Interlace);
        
        CLISetSwitchFlag(CLI, Optimize, "Optimize");
        CLISetSwitchDescription(CLI, Optimize, "Optimize (try all filter options) the encoded PNG to be as small as possible");
        CLISetSwitchType(CLI, Optimize, SingleSwitchNoResult);
        CLISetSwitchAsSlave(CLI, Encode, Optimize);
        /* End Encode Options */
        
        /* Start Decode Options */
        CLISetSwitchFlag(CLI, Decode, "Decode");
        CLISetSwitchDescription(CLI, Decode, "Decode PNG to output");
        CLISetSwitchType(CLI, Decode, SingleSwitchNoResult);
        /* End Decode Options */
        
        /* Metadata Options */
        CLISetSwitchFlag(CLI, InsertMeta, "InsertMeta");
        CLISetSwitchDescription(CLI, InsertMeta, "Adds metadata to PNG file");
        CLISetSwitchType(CLI, InsertMeta, MasterSwitch);
        
        CLISetSwitchFlag(CLI, ExtractMeta, "ExtractMeta");
        CLISetSwitchDescription(CLI, ExtractMeta, "Extracts metadata from PNG file");
       	CLISetSwitchType(CLI, ExtractMeta, MasterSwitch);
        
        CLISetSwitchFlag(CLI, RemoveMeta, "RemoveMeta");
        CLISetSwitchDescription(CLI, RemoveMeta, "Removes metadata from PNG file");
        CLISetSwitchType(CLI, RemoveMeta, MasterSwitch);
        
        CLISetSwitchFlag(CLI, Help, "Help");
        CLISetSwitchDescription(CLI, Help, "Prints all the command line options");
        CLISetSwitchType(CLI, Help, SingleSwitchNoResult);
        
        return CLI;
    }
    
    typedef enum FileTypes {
        UnknownFileFormat = 0,
        PNGFileFormat     = 1,
        PXMFileFormat     = 2,
        BMPFileFormat     = 3,
    } FileTypes;
    
    static FileTypes IdentifyInputFileFromBitBuffer(BitBuffer *BitB) {
        FileTypes InputFileType = 0;
        uint64_t First8Bytes  = PeekBits(BitIOMSByte, BitIOLSBit, BitB, 64);
        uint16_t First2Bytes  = PeekBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        if (First8Bytes == PNGMagic) {
            InputFileType = PNGFileFormat;
        } else if (First2Bytes == PXM_PBMA || First2Bytes == PXM_PBMB || First2Bytes == PXM_PGMA || First2Bytes == PXM_PGMB || First2Bytes == PXM_PPMA || First2Bytes == PXM_PPMB || First2Bytes == PXM_PAMB) {
            InputFileType = PXMFileFormat;
        } else if (First2Bytes == BMP_BM || First2Bytes == BMP_BitmapArray) {
            InputFileType = BMPFileFormat;
        }
        return InputFileType;
    }
    
    static void SetOutputFormatFromExtension(char *Extension) {
        if (strcasecmp(Extension, "bmp") == 0) {
            // Output Format = BMP
        } else if (strcasecmp(Extension, "pxm") == 0 || strcasecmp(Extension, "pnm") == 0 || strcasecmp(Extension, "pam") == 0 || strcasecmp(Extension, "pbm") == 0 || strcasecmp(Extension, "pgm") == 0) {
            // Output format = PXM
        }
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineIO *CLI  = SetModernPNGOptions();
        if (CLI == NULL) {
            exit(EXIT_FAILURE);
        } else {
            ParseCommandLineArguments(CLI, argc, argv);
            
            DecodePNG *Dec                      = NULL;
            EncodePNG *Enc                      = NULL;
            
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
            
            BitBuffer *InputLeftFile             = NULL;
            BitBuffer *InputRightFile            = NULL;
            BitBuffer *Input2DFile               = NULL;
            BitBuffer *Input3DFile               = NULL;
            BitBuffer *OutputLeftFile            = NULL;
            BitBuffer *OutputRightFile           = NULL;
            BitBuffer *Output2DFile              = NULL;
            BitBuffer *Output3DFile              = NULL;
            
            bool     EncodeAsPNG                 = 0;
            bool     DecodeAsPNG                 = 0;
            
            uint64_t NumEncodeArguments          = CLIGetNumMatchingArguments(CLI, Encode, 0, NULL);
            uint64_t NumDecodeArguments          = CLIGetNumMatchingArguments(CLI, Encode, 0, NULL);
            
            uint64_t InputSlaves[2]              = {LeftEye, RightEye};
            uint64_t OutputSlaves[2]             = {LeftEye, RightEye};
            
            uint64_t NumInputFiles               = CLIGetNumMatchingArguments(CLI, Input, 2, InputSlaves);
            uint64_t NumOutputFiles              = CLIGetNumMatchingArguments(CLI, Encode, 0, OutputSlaves);
            
            uint64_t LeftEyeSlave[1]             = {LeftEye};
            uint64_t RightEyeSlave[1]            = {RightEye};
            
            FileTypes Input2DFileType            = UnknownFileFormat;
            FileTypes Input3DFileType            = UnknownFileFormat;
            FileTypes InputLeftFileType          = UnknownFileFormat;
            FileTypes InputRightFileType         = UnknownFileFormat;
            
            FileTypes Output2DFileType           = UnknownFileFormat;
            FileTypes Output3DFileType           = UnknownFileFormat;
            FileTypes OutputLeftFileType         = UnknownFileFormat;
            FileTypes OutputRightFileType        = UnknownFileFormat;
            
            if (NumEncodeArguments == 1) {
                EncodeAsPNG = Yes;
            } else if (NumDecodeArguments == 1) {
                DecodeAsPNG = Yes;
            }
            
            if (EncodeAsPNG == Yes) { // If we're supposed to Encode files to PNG, do this.
                if (NumInputFiles == 1 && NumOutputFiles == 1) {
                    Encode2D = true;
                    /* 2D Input */
                    uint64_t  Input2DArg         = CLIGetMatchingArgumentNum(CLI, 1, Input, 0, NULL);
                    char     *Input2DPath        = CLIGetArgumentResult(CLI, Input2DArg);
                    Input2D                      = BitInput_Init();
                    BitInput_OpenFile(Input2D, Input2DPath);
                    Input2DFile                  = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(Input2D, Input2DFile, 8);
                    /* 2D Input */
                    
                    /* 2D Output */
                    uint64_t  Output2DArg        = CLIGetMatchingArgumentNum(CLI, 1, Output, 0, NULL);
                    char     *Output2DPath       = CLIGetArgumentResult(CLI, Output2DArg);
                    Output2D                     = BitOutput_Init();
                    BitOutput_OpenFile(Output2D, Output2DPath);
                    Output2DFile                 = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(Output2D, Output3DFile, 4096);
                    /* 2D Output */
                    
                    /* Decode Input, Encode Output */
                    Input2DFileType              = IdentifyInputFileFromBitBuffer(Input2DFile);
                    /* Decode Input, Encode Output */
                } else if (NumInputFiles == 2 && NumOutputFiles == 1) {
                    Encode3D = true;
                    /* LeftEye Input */
                    uint64_t InputLeftEyeArg     = CLIGetMatchingArgumentNum(CLI, 1, Input, 1, LeftEyeSlave);
                    char *InputLeftEyePath       = CLIGetArgumentResult(CLI, InputLeftEyeArg);
                    InputLeftEye                 = BitInput_Init();
                    BitInput_OpenFile(InputLeftEye, InputLeftEyePath);
                    InputLeftFile                = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(InputLeftEye, InputLeftFile, 8);
                    /*  LeftEye Input */
                    
                    /*  RightEye Input */
                    uint64_t InputRightEyeArg    = CLIGetMatchingArgumentNum(CLI, 1, Input, 1, RightEyeSlave);
                    char *InputRightEyePath      = CLIGetArgumentResult(CLI, InputRightEyeArg);
                    InputRightEye                = BitInput_Init();
                    BitInput_OpenFile(InputRightEye, InputRightEyePath);
                    InputRightFile               = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(InputRightEye, InputRightFile, 8);
                    /*  RightEye Input */
                    
                    /* 3D Output */
                    uint64_t  Output3DArg        = CLIGetMatchingArgumentNum(CLI, 1, Output, 0, NULL);
                    char     *Output3DPath       = CLIGetArgumentResult(CLI, Output3DArg);
                    Output3D                     = BitOutput_Init();
                    BitOutput_OpenFile(Output3D, Output3DPath);
                    Output3DFile                 = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(Output3D, Output3DFile, 4096);
                    /* 3D Output */
                    
                    /* Decode Input, Encode Output */
                    InputLeftFileType             = IdentifyInputFileFromBitBuffer(InputLeftFile);
                    InputRightFileType            = IdentifyInputFileFromBitBuffer(InputRightFile);
                    /* Decode Input, Encode Output */
                } else {
                    BitIOLog(LOG_ERROR, "ModernPNG", __func__, "Too few/many Input %d or Output %d files, min is 1, max is 2", NumInputFiles, NumOutputFiles);
                }
            } else if (DecodeAsPNG == Yes) { // If we're supposed to Decode a PNG file, do this.
                if (NumInputFiles == 1 && NumOutputFiles == 1) {
                    Decode2D = true;
                    /* 2D Input */
                    uint64_t  Input2DArg         = CLIGetMatchingArgumentNum(CLI, 1, Input, 0, NULL);
                    char     *Input2DPath        = CLIGetArgumentResult(CLI, Input2DArg);
                    Input2D                      = BitInput_Init();
                    BitInput_OpenFile(Input2D, Input2DPath);
                    Input2DFile                  = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(Input2D, Input2DFile, 8);
                    /* 2D Input */
                    
                    /* 2D Output */
                    uint64_t  Output2DArg        = CLIGetMatchingArgumentNum(CLI, 1, Output, 0, NULL);
                    char     *Output2DPath       = CLIGetArgumentResult(CLI, Output2DArg);
                    Output2D                     = BitOutput_Init();
                    BitOutput_OpenFile(Output2D, Output2DPath);
                    Output2DFile                 = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(Output2D, Output2DFile, 4096);
                    /* 2D Output */
                    
                    /* Decode Input, Encode Output */
                    // For this, we need to find the extension of the output path, so we can decode it to the format the user wants.
                    Output2DFileType = GetExtensionFromPath(Output2DPath);
                    /* Decode Input, Encode Output */
                } else if (NumInputFiles == 1 && NumOutputFiles == 2) {
                    Decode3D = true;
                    /* 3D Input */
                    uint64_t  Input3DArg         = CLIGetMatchingArgumentNum(CLI, 1, Input, 0, NULL);
                    char     *Input3DPath        = CLIGetArgumentResult(CLI, Input3DArg);
                    Input3D                      = BitInput_Init();
                    BitInput_OpenFile(Input3D, Input3DPath);
                    Input3DFile                  = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(Input3D, Input3DFile, 8);
                    /* 3D Input */
                    
                    /* LeftEye Output */
                    uint64_t  OutputLeftEyeArg   = CLIGetMatchingArgumentNum(CLI, 1, Output, 1, LeftEyeSlave);
                    char     *OutputLeftEyePath  = CLIGetArgumentResult(CLI, OutputLeftEyeArg);
                    OutputLeftEye                = BitOutput_Init();
                    BitOutput_OpenFile(OutputLeftEye, OutputLeftEyePath);
                    OutputLeftFile               = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(OutputLeftEye, OutputLeftFile, 4096);
                    /* LeftEye Output */
                    
                    /*  RightEye Output */
                    uint64_t  OutputRightEyeArg  = CLIGetMatchingArgumentNum(CLI, 1, Output, 1, RightEyeSlave);
                    char     *OutputRightEyePath = CLIGetArgumentResult(CLI, OutputRightEyeArg);
                    OutputRightEye               = BitOutput_Init();
                    BitOutput_OpenFile(OutputRightEye, OutputRightEyePath);
                    OutputRightFile              = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(OutputRightEye, OutputRightFile, 4096);
                    /*  RightEye Output */
                    
                    /* Decode Input, Encode Output */
                    OutputLeftFileType  = GetExtensionFromPath(OutputLeftEyePath);
                    OutputRightFileType = GetExtensionFromPath(OutputRightEyePath);
                    
                    // Decode the Input file, then encode that to the output format
                    if (Input3DFileType == PNGFileFormat) {
                        Dec = DecodePNGInit();
                    }
                 	uint16_t ***Decoded3DPixelArray = DecodePNGImage(Dec, Input3DFile);
                    /* Decode Input, Encode Output */
                } else {
                    BitIOLog(LOG_ERROR, "ModernPNG", __func__, "Too few/many Input %d or Output %d files, min is 1, max is 2", NumInputFiles, NumOutputFiles);
                }
            }
        }
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
