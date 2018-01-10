#include "../libModernPNG/include/libModernPNG.h"
#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/BitIO.h"
#include "../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/CommandLineIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"

static UTF8Constant ModernPNGVersion     = u8"0.5.0";
static UTF8String   ModernPNGProgramName = u8"ModernPNG";

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
        
        CLISetName(CLI, (UTF8String) ModernPNGProgramName);
        CLISetAuthor(CLI, u8"BumbleBritches57");
        CLISetVersion(CLI, (UTF8String) ModernPNGVersion);
        CLISetCopyright(CLI, u8"2017+");
        CLISetDescription(CLI, u8"PNG encoder/decoder written from scratch in modern C");
        CLISetLicense(CLI, u8"Revised BSD", u8"A permissive open source license", u8"https://opensource.org/licenses/BSD-3-Clause", No);
        CLISetMinOptions(CLI, 3);
        CLISetHelpSwitch(CLI, Help);
        
        CLISetSwitchName(CLI, Input, u8"Input");
        CLISetSwitchDescription(CLI, Input, u8"Input file or stdin with: -");
        CLISetSwitchType(CLI, Input, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, Input, 1);
        
        CLISetSwitchName(CLI, Output, "Output");
        CLISetSwitchDescription(CLI, Output, "Output file or stdout with: -");
        CLISetSwitchType(CLI, Output, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, Output, 1);
        
        CLISetSwitchName(CLI, LeftEye, "LeftEye");
        CLISetSwitchDescription(CLI, LeftEye, "The left view for encoding or decoding");
        CLISetSwitchType(CLI, LeftEye, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, Input, LeftEye);
        CLISetSwitchAsSlave(CLI, Output, LeftEye);
        
        CLISetSwitchName(CLI, RightEye, "RightEye");
        CLISetSwitchDescription(CLI, RightEye, "The right view for encoding or decoding");
        CLISetSwitchType(CLI, RightEye, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, Input, RightEye);
        CLISetSwitchAsSlave(CLI, Output, RightEye);
        
        CLISetSwitchName(CLI, LogFile, "LogFile");
        CLISetSwitchDescription(CLI, LogFile, "Outputs the logs to a specified path");
        CLISetSwitchType(CLI, LogFile, SwitchCantHaveSlaves);
        
        /* Start Encode Options */
        CLISetSwitchName(CLI, Encode, "Encode");
        CLISetSwitchDescription(CLI, Encode, "Encode input(s) to PNG");
        CLISetSwitchType(CLI, Encode, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, Encode, 2);
        
        CLISetSwitchName(CLI, Interlace, "Interlace");
        CLISetSwitchDescription(CLI, Interlace, "Should we encode the PNG with Adam7 Interlacing? (Currently not supported)");
        CLISetSwitchType(CLI, Interlace, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, Encode, Interlace);
        
        CLISetSwitchName(CLI, Optimize, "Optimize");
        CLISetSwitchDescription(CLI, Optimize, "Optimize (try all filter options) the encoded PNG to be as small as possible");
        CLISetSwitchType(CLI, Optimize, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, Encode, Optimize);
        /* End Encode Options */
        
        /* Start Decode Options */
        CLISetSwitchName(CLI, Decode, "Decode");
        CLISetSwitchDescription(CLI, Decode, "Decode PNG to output");
        CLISetSwitchType(CLI, Decode, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, Decode, 0);
        /* End Decode Options */
        
        /* Metadata Options */
        CLISetSwitchName(CLI, InsertMeta, "InsertMeta");
        CLISetSwitchDescription(CLI, InsertMeta, "Adds metadata to PNG file");
        CLISetSwitchType(CLI, InsertMeta, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, InsertMeta, 4);
        
        CLISetSwitchName(CLI, ExtractMeta, "ExtractMeta");
        CLISetSwitchDescription(CLI, ExtractMeta, "Extracts metadata from PNG file");
       	CLISetSwitchType(CLI, ExtractMeta, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, ExtractMeta, 4);
        
        CLISetSwitchName(CLI, RemoveMeta, "RemoveMeta");
        CLISetSwitchDescription(CLI, RemoveMeta, "Removes metadata from PNG file");
        CLISetSwitchType(CLI, RemoveMeta, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, RemoveMeta, 4);
        
        CLISetSwitchName(CLI, ICCProfile, "ICCProfile");
        CLISetSwitchDescription(CLI, ICCProfile, "ICC Format Color Profile");
        CLISetSwitchType(CLI, ICCProfile, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, InsertMeta, ICCProfile);
        CLISetSwitchAsSlave(CLI, ExtractMeta, ICCProfile);
        CLISetSwitchAsSlave(CLI, RemoveMeta, ICCProfile);
        
        CLISetSwitchName(CLI, Gamma, "Gamma");
        CLISetSwitchDescription(CLI, Gamma, "Gamma chunk");
        CLISetSwitchType(CLI, Gamma, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, InsertMeta, Gamma);
        CLISetSwitchAsSlave(CLI, ExtractMeta, Gamma);
        CLISetSwitchAsSlave(CLI, RemoveMeta, Gamma);

        
        CLISetSwitchName(CLI, Text, "Text");
        CLISetSwitchDescription(CLI, Text, "The various text chunks");
        CLISetSwitchType(CLI, Text, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, InsertMeta, Text);
        CLISetSwitchAsSlave(CLI, ExtractMeta, Text);
        CLISetSwitchAsSlave(CLI, RemoveMeta, Text);
        
        CLISetSwitchName(CLI, Histogram, "Histogram");
        CLISetSwitchDescription(CLI, Histogram, "The histogram for the image");
        CLISetSwitchType(CLI, Histogram, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, InsertMeta, Histogram);
        CLISetSwitchAsSlave(CLI, ExtractMeta, Histogram);
        CLISetSwitchAsSlave(CLI, RemoveMeta, Histogram);
        
        CLISetSwitchName(CLI, Help, "Help");
        CLISetSwitchDescription(CLI, Help, "Display the help");
        CLISetSwitchType(CLI, Help, SwitchCantHaveSlaves);
        
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
        /*
        if (First8Bytes == PNGMagic) {
            InputFileType = PNGFileFormat;
        } else if (PCMFile_Identify(PCM, BitB) != PCMFormat) {
            InputFileType = PXMFileFormat;
        } else if (First2Bytes == BMP_BM || First2Bytes == BMP_BitmapArray) {
            InputFileType = BMPFileFormat;
        }
         */
        return InputFileType;
    }
    
    int main(int argc, const char *argv[]) {
        CommandLineIO *CLI  = SetModernPNGOptions();
        if (CLI == NULL) {
            //exit(1);
        } else {
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
            
            DecodePNG *Dec                       = DecodePNG_Init();
            EncodePNG *Enc                       = NULL;
            
            bool Encode3D                        = false;
            bool Decode3D                        = false;
            bool Encode2D                        = false;
            bool Decode2D                        = false;
            
            
            
            bool     EncodeAsPNG                 = 0;
            bool     DecodeAsPNG                 = 0;
            
            uint64_t InputSlaves[2]              = {LeftEye, RightEye};
            uint64_t OutputSlaves[2]             = {LeftEye, RightEye};
            
            int64_t LeftEyeSlave[1]              = {LeftEye};
            int64_t RightEyeSlave[1]             = {RightEye};
            
            FileTypes Input2DFileType            = UnknownFileFormat;
            FileTypes Input3DFileType            = UnknownFileFormat;
            FileTypes InputLeftFileType          = UnknownFileFormat;
            FileTypes InputRightFileType         = UnknownFileFormat;
            
            FileTypes Output2DFileType           = UnknownFileFormat;
            FileTypes Output3DFileType           = UnknownFileFormat;
            FileTypes OutputLeftFileType         = UnknownFileFormat;
            FileTypes OutputRightFileType        = UnknownFileFormat;
            
            ParseCommandLineOptions(CLI, argc, argv);
            PrintCommandLineOptions(CLI);
            
            bool WeAreSupposedToEncode           = CLIGetNumMatchingOptions(CLI, Encode, 0, NULL);
            bool WeAreSupposedToDecode           = CLIGetNumMatchingOptions(CLI, Decode, 0, NULL);
            
            int64_t NumInputFiles                = CLIGetNumMatchingOptions(CLI, Input, 0, NULL);
            int64_t NumOutputFiles               = CLIGetNumMatchingOptions(CLI, Output, 0, NULL);
            
            if (WeAreSupposedToEncode == Yes) {
                if (NumInputFiles == 1 && NumOutputFiles == 1) {
                    // Encode 2D PCM to 2D PNG
                    /* 2D Input */
                    int64_t  Input2DOption       = CLIGetOptionNum(CLI, Input, 0, NULL);
                    char    *Input2DPath         = CLIGetOptionResult(CLI, Input2DOption);
                    Input2D                      = BitInput_Init();
                    BitInput_OpenFile(Input2D, Input2DPath);
                    Input2DFile                  = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(Input2D, Input2DFile, 8);
                    /* 2D Input */
                    
                    /* 2D Output */
                    uint64_t  Output2DOption     = CLIGetOptionNum(CLI, Output, 0, NULL);
                    char     *Output2DPath       = CLIGetOptionResult(CLI, Output2DOption);
                    Output2D                     = BitOutput_Init();
                    BitOutput_OpenFile(Output2D, Output2DPath);
                    Output2DFile                 = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(Output2D, Output3DFile, 4096);
                    /* 2D Output */
                } else if (NumInputFiles == 2 && NumOutputFiles == 1) {
                    // Encode 2 PCM to 3D PNG
                    Encode3D = true;
                    /* LeftEye Input */
                    uint64_t InputLeftEyeOption  = CLIGetOptionNum(CLI, Input, 1, LeftEyeSlave);
                    char *InputLeftEyePath       = CLIGetOptionResult(CLI, InputLeftEyeOption);
                    InputLeftEye                 = BitInput_Init();
                    BitInput_OpenFile(InputLeftEye, InputLeftEyePath);
                    InputLeftFile                = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(InputLeftEye, InputLeftFile, 8);
                    /*  LeftEye Input */
                    
                    /*  RightEye Input */
                    uint64_t InputRightEyeArg    = CLIGetOptionNum(CLI, Input, 1, RightEyeSlave);
                    char *InputRightEyePath      = CLIGetOptionResult(CLI, InputRightEyeArg);
                    InputRightEye                = BitInput_Init();
                    BitInput_OpenFile(InputRightEye, InputRightEyePath);
                    InputRightFile               = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(InputRightEye, InputRightFile, 8);
                    /*  RightEye Input */
                    
                    /* 3D Output */
                    uint64_t  Output3DArg        = CLIGetOptionNum(CLI, Output, 0, NULL);
                    char     *Output3DPath       = CLIGetOptionResult(CLI, Output3DArg);
                    Output3D                     = BitOutput_Init();
                    BitOutput_OpenFile(Output3D, Output3DPath);
                    Output3DFile                 = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(Output3D, Output3DFile, 4096);
                    /* 3D Output */
                }
            } else if (WeAreSupposedToDecode == Yes) {
                if (NumInputFiles == 1 && NumOutputFiles == 1) {
                    // Decode 2D PNG to 2D PCM
                    Decode2D = true;
                    /* 2D Input */
                    uint64_t  Input2DArg         = CLIGetOptionNum(CLI, Input, 0, NULL);
                    char     *Input2DPath        = CLIGetOptionResult(CLI, Input2DArg);
                    Input2D                      = BitInput_Init();
                    BitInput_OpenFile(Input2D, Input2DPath);
                    Input2DFile                  = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(Input2D, Input2DFile, 8);
                    /* 2D Input */
                    
                    /* 2D Output */
                    uint64_t  Output2DArg        = CLIGetOptionNum(CLI, Output, 0, NULL);
                    char     *Output2DPath       = CLIGetOptionResult(CLI, Output2DArg);
                    Output2D                     = BitOutput_Init();
                    BitOutput_OpenFile(Output2D, Output2DPath);
                    Output2DFile                 = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(Output2D, Output2DFile, 4096);
                    /* 2D Output */
                } else if (NumInputFiles == 1 && NumOutputFiles == 2) {
                    // Decode 3D PNG to 2 2D PCM
                    Decode3D = true;
                    /* 3D Input */
                    uint64_t  Input3DArg         = CLIGetOptionNum(CLI, Input, 0, NULL);
                    char     *Input3DPath        = CLIGetOptionResult(CLI, Input3DArg);
                    Input3D                      = BitInput_Init();
                    BitInput_OpenFile(Input3D, Input3DPath);
                    Input3DFile                  = BitBuffer_Init(8);
                    BitInput_Read2BitBuffer(Input3D, Input3DFile, 8);
                    /* 3D Input */
                    
                    /* LeftEye Output */
                    uint64_t  OutputLeftEyeArg   = CLIGetOptionNum(CLI, Output, 1, LeftEyeSlave);
                    char     *OutputLeftEyePath  = CLIGetOptionResult(CLI, OutputLeftEyeArg);
                    OutputLeftEye                = BitOutput_Init();
                    BitOutput_OpenFile(OutputLeftEye, OutputLeftEyePath);
                    OutputLeftFile               = BitBuffer_Init(8);
                    BitOutput_WriteBitBuffer(OutputLeftEye, OutputLeftFile, 4096);
                    /* LeftEye Output */
                    
                    /*  RightEye Output */
                    uint64_t  OutputRightEyeArg  = CLIGetOptionNum(CLI, Output, 1, RightEyeSlave);
                    char     *OutputRightEyePath = CLIGetOptionResult(CLI, OutputRightEyeArg);
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
                        Dec = DecodePNG_Init();
                    }
                    uint16_t ***Decoded3DPixelArray = DecodePNGImage(Dec, Input3DFile);
                    /* Decode Input, Encode Output */
                }
            }
        }
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
