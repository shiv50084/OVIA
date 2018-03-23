#include "../libModernPNG/include/libModernPNG.h"
#include "../Dependencies/libPCM/Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../Dependencies/libPCM/Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../Dependencies/libPCM/Dependencies/FoundationIO/libFoundationIO/include/CommandLineIO.h"
#include "../Dependencies/libPCM/libPCM/include/libPCM.h"

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
        
        UTF8 ProgramName[]    = U8("ModernPNG");
        UTF8 ProgramVersion[] = U8("0.5.0");
        
        Log_SetProgramName(ProgramName);
        
        UTF8 ProgramAuthor[]                = U8("BumbleBritches57");
        UTF8 ProgramCopyright[]             = U8("2017+");
        UTF8 ProgramDescription[]           = U8("PNG En/De Coder Written in Modern C, and Released Under A Permissive License");
        UTF8 ProgramLicenseName[]           = U8("Revised BSD");
        UTF8 ProgramLicenseURL[]            = U8("https://tldrlegal.com/license/bsd-3-clause-license-(revised)");
        
        UTF8 InputSwitchName[]              = U8("Input");
        UTF8 InputSwitchDescription[]       = U8("Input file or stdin with: -");
        
        UTF8 OutputSwitchName[]             = U8("Output");
        UTF8 OutputSwitchDescription[]      = U8("Output file or stdout with: -");
        
        UTF8 LeftEyeSwitchName[]            = U8("LeftEye");
        UTF8 LeftEyeSwitchDescription[]     = U8("The left eye view for encoding or decoding");
        
        UTF8 RightEyeSwitchName[]           = U8("RightEye");
        UTF8 RightEyeSwitchDescripton[]     = U8("The right eye view for encoding or decoding");
        
        UTF8 LogFileSwitchName[]            = U8("LogFile");
        UTF8 LogFileSwitchDescription[]     = U8("Outputs the logs to the specified path");
        
        UTF8 EncodeSwitchName[]             = U8("Encode");
        UTF8 EncodeSwitchDescription[]      = U8("Encodes input(s) to PNG");
        
        UTF8 InterlaceSwitchName[]          = U8("Interlace");
        UTF8 InterlaceSwitchDescription[]   = U8("Interlaces the output PNG");
        
        UTF8 OptimizeSwitchName[]           = U8("Optimize");
        UTF8 OptimizeSwitchDescription[]    = U8("Optimizes the output PNG");
        
        UTF8 DecodeSwitchName[]             = U8("Decode");
        UTF8 DecodeSwitchDescription[]      = U8("Decodes input PNG to whatever the extension matches");
        
        UTF8 InsertMetaSwitchName[]         = U8("InsertMeta");
        UTF8 InsertMetaSwitchDescription[]  = U8("Inserts metadata from a string or file into the PNG file");
        
        UTF8 ExtractMetaSwitchName[]        = U8("ExtractMeta");
        UTF8 ExtractMetaSwitchDescription[] = U8("Extracts metadata to a string or file depending on the type");
        
        UTF8 RemoveMetaSwitchName[]         = U8("RemoveMeta");
        UTF8 RemoveMetaSwitchDescription[]  = U8("Removes the specified metadata type(s) from the PNG file");
        
        UTF8 ICCProfileSwitchName[]         = U8("ICCProfile");
        UTF8 ICCProfileSwitchDescription[]  = U8("Adds/Removes/Extracts the ICC Color Profile");
        
        UTF8 GammaSwitchName[]              = U8("Gamma");
        UTF8 GammaSwitchDescription[]       = U8("Adds/Removes/Extracts the Gamma profile");
        
        UTF8 TextSwitchName[]               = U8("Text");
        UTF8 TextSwitchDescription[]        = U8("Searches for Keyword in all text chunks to Adds/Removes/Extract, Otherwise it applies to all text chunks");
        
        UTF8 HistogramSwitchName[]          = U8("Histogram");
        UTF8 HistogramSwitchDescription[]   = U8("Adds/Removes/Extracts the Histogram chunk if it's present");
        
        UTF8 HelpSwitchName[]               = U8("Help");
        UTF8 HelpSwitchDescription[]        = U8("Prints all of the available options, and their relationships");
        
        CLISetName(CLI, ProgramName);
        CLISetAuthor(CLI, ProgramAuthor);
        CLISetVersion(CLI, ProgramVersion);
        CLISetCopyright(CLI, ProgramCopyright);
        CLISetDescription(CLI, ProgramDescription);
        CLISetLicense(CLI, PermissiveLicense, ProgramLicenseName, ProgramLicenseURL);
        CLISetMinOptions(CLI, 3);
        
        CLISetSwitchName(CLI, Input, InputSwitchName);
        CLISetSwitchDescription(CLI, Input, InputSwitchDescription);
        CLISetSwitchType(CLI, Input, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, Input, 1);
        
        CLISetSwitchName(CLI, Output, OutputSwitchName);
        CLISetSwitchDescription(CLI, Output, OutputSwitchDescription);
        CLISetSwitchType(CLI, Output, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, Output, 1);
        
        CLISetSwitchName(CLI, LeftEye, LeftEyeSwitchName);
        CLISetSwitchDescription(CLI, LeftEye, LeftEyeSwitchDescription);
        CLISetSwitchType(CLI, LeftEye, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, Input, LeftEye);
        CLISetSwitchAsSlave(CLI, Output, LeftEye);
        
        CLISetSwitchName(CLI, RightEye, RightEyeSwitchName);
        CLISetSwitchDescription(CLI, RightEye, RightEyeSwitchDescripton);
        CLISetSwitchType(CLI, RightEye, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, Input, RightEye);
        CLISetSwitchAsSlave(CLI, Output, RightEye);
        
        CLISetSwitchName(CLI, LogFile, LogFileSwitchName);
        CLISetSwitchDescription(CLI, LogFile, LogFileSwitchDescription);
        CLISetSwitchType(CLI, LogFile, SwitchCantHaveSlaves);
        
        /* Start Encode Options */
        CLISetSwitchName(CLI, Encode, EncodeSwitchName);
        CLISetSwitchDescription(CLI, Encode, EncodeSwitchDescription);
        CLISetSwitchType(CLI, Encode, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, Encode, 2);
        
        CLISetSwitchName(CLI, Interlace, InterlaceSwitchName);
        CLISetSwitchDescription(CLI, Interlace, InterlaceSwitchDescription);
        CLISetSwitchType(CLI, Interlace, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, Encode, Interlace);
        
        CLISetSwitchName(CLI, Optimize, OptimizeSwitchName);
        CLISetSwitchDescription(CLI, Optimize, OptimizeSwitchDescription);
        CLISetSwitchType(CLI, Optimize, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, Encode, Optimize);
        /* End Encode Options */
        
        /* Start Decode Options */
        CLISetSwitchName(CLI, Decode, DecodeSwitchName);
        CLISetSwitchDescription(CLI, Decode, DecodeSwitchDescription);
        CLISetSwitchType(CLI, Decode, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, Decode, 0);
        /* End Decode Options */
        
        /* Metadata Options */
        CLISetSwitchName(CLI, InsertMeta, InsertMetaSwitchName);
        CLISetSwitchDescription(CLI, InsertMeta, InsertMetaSwitchDescription);
        CLISetSwitchType(CLI, InsertMeta, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, InsertMeta, 4);
        
        CLISetSwitchName(CLI, ExtractMeta, ExtractMetaSwitchName);
        CLISetSwitchDescription(CLI, ExtractMeta, ExtractMetaSwitchDescription);
       	CLISetSwitchType(CLI, ExtractMeta, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, ExtractMeta, 4);
        
        CLISetSwitchName(CLI, RemoveMeta, RemoveMetaSwitchName);
        CLISetSwitchDescription(CLI, RemoveMeta, RemoveMetaSwitchDescription);
        CLISetSwitchType(CLI, RemoveMeta, SwitchMayHaveSlaves);
        CLISetSwitchMaxConcurrentSlaves(CLI, RemoveMeta, 4);
        
        CLISetSwitchName(CLI, ICCProfile, ICCProfileSwitchName);
        CLISetSwitchDescription(CLI, ICCProfile, ICCProfileSwitchDescription);
        CLISetSwitchType(CLI, ICCProfile, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, InsertMeta, ICCProfile);
        CLISetSwitchAsSlave(CLI, ExtractMeta, ICCProfile);
        CLISetSwitchAsSlave(CLI, RemoveMeta, ICCProfile);
        
        CLISetSwitchName(CLI, Gamma, GammaSwitchName);
        CLISetSwitchDescription(CLI, Gamma, GammaSwitchDescription);
        CLISetSwitchType(CLI, Gamma, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, InsertMeta, Gamma);
        CLISetSwitchAsSlave(CLI, ExtractMeta, Gamma);
        CLISetSwitchAsSlave(CLI, RemoveMeta, Gamma);
        
        CLISetSwitchName(CLI, Text, TextSwitchName);
        CLISetSwitchDescription(CLI, Text, TextSwitchDescription);
        CLISetSwitchType(CLI, Text, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, InsertMeta, Text);
        CLISetSwitchAsSlave(CLI, ExtractMeta, Text);
        CLISetSwitchAsSlave(CLI, RemoveMeta, Text);
        
        CLISetSwitchName(CLI, Histogram, HistogramSwitchName);
        CLISetSwitchDescription(CLI, Histogram, HistogramSwitchDescription);
        CLISetSwitchType(CLI, Histogram, SwitchIsASlave);
        CLISetSwitchAsSlave(CLI, InsertMeta, Histogram);
        CLISetSwitchAsSlave(CLI, ExtractMeta, Histogram);
        CLISetSwitchAsSlave(CLI, RemoveMeta, Histogram);
        /* Metadata Options */
        
        CLISetSwitchName(CLI, Help, HelpSwitchName);
        CLISetSwitchDescription(CLI, Help, HelpSwitchDescription);
        CLISetSwitchType(CLI, Help, SwitchCantHaveSlaves);
        CLISetHelpSwitch(CLI, Help);
        
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
        uint64_t First8Bytes  = PeekBits(MSByteFirst, LSBitFirst, BitB, 64);
        uint16_t First2Bytes  = PeekBits(LSByteFirst, LSBitFirst, BitB, 16);
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
            
#if FoundationIOTargetOS == POSIXOS
            UTF8_ParseCommandLineOptions(CLI, argc, (UTF8**)argv);
#elif FoundationIOTargetOS == WindowsOS
            UTF16_ParseCommandLineOptions(CLI, __argc, __wargv);
#endif
            
            DEBUGCommandLineOptions(CLI);
            
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
