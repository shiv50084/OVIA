#include <stdlib.h>

#include "../Dependencies/BitIO/libBitIO/include/BitIO.h"
#include "../Dependencies/BitIO/libBitIO/include/BitIOLog.h"
#include "../Dependencies/BitIO/libBitIO/include/BitIOMath.h"
#include "../Dependencies/BitIO/libBitIO/include/CommandLineIO.h"

#include "../libPCM/include/libPCM.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef enum CommandLineSwitches { // CommandLineSwitchNames
        Input         = 0,
        Output        = 1,
        LogFile       = 2,
        SilenceLevel  = 3,
        Help          = 4,
        NumSwitches   = 5,
    } CommandLineSwitches;
    
    static CommandLineIO *SetTrimSilenceOptions(void) {
        CommandLineIO *CLI = CommandLineIO_Init(NumSwitches);
        
        UTF8Constant ProgramName = u8"TrimSilence";
        UTF8Constant ProgramVersion = u8"0.1.0";
        UTF8Constant ProgramAuthor = u8"BumbleBritches57";
        UTF8Constant ProgramCopyright = u8"2017";
        UTF8Constant ProgramDescription = u8"Removes leading and trailing silence";
        UTF8Constant LicenseName = u8"Revised BSD";
        UTF8Constant LicenseType = u8"Permissive";
        UTF8Constant LicenseURL = u8"https://opensource.com/licenses/BSD-3-Clause";
        
        UTF8Constant Switch0Name = u8"Input";
        UTF8Constant Switch0Description = u8"Input file path or stdin";
        
        UTF8Constant Switch1Name = u8"Output";
        UTF8Constant Switch1Description = u8"Output file path or stdout";
        
        UTF8Constant Switch2Name = u8"LogFile";
        UTF8Constant Switch2Description = u8"Log file path or stderr";
        
        UTF8Constant Switch3Name = u8"SilenceLevel";
        UTF8Constant Switch3Description = u8"Absolute value of cutoff for what's considered silence";
        
        UTF8Constant Switch4Name = u8"Help";
        UTF8Constant Switch4Description = u8"Prints all the command line options and their relationships";
        
        
        BitIOLog_SetProgramName((UTF8String) ProgramName);
        
        CLISetName(CLI, (UTF8String)ProgramName);
        CLISetVersion(CLI, (UTF8String)ProgramVersion);
        CLISetAuthor(CLI, (UTF8String)ProgramAuthor);
        CLISetCopyright(CLI, (UTF8String)ProgramCopyright);
        CLISetDescription(CLI, (UTF8String)ProgramDescription);
        CLISetLicense(CLI, (UTF8String)LicenseName, (UTF8String)LicenseType, (UTF8String)LicenseURL, No);
        CLISetMinOptions(CLI, 3);
        
        CLISetSwitchName(CLI, Input, (UTF8String)Switch0Name);
        CLISetSwitchDescription(CLI, Input, (UTF8String)Switch0Description);
        CLISetSwitchType(CLI, Input, SwitchCantHaveSlaves);
        
        CLISetSwitchName(CLI, Output, (UTF8String)Switch1Name);
        CLISetSwitchDescription(CLI, Output, (UTF8String)Switch1Description);
        CLISetSwitchType(CLI, Output, SwitchCantHaveSlaves);
        
        CLISetSwitchName(CLI, LogFile, (UTF8String)Switch2Name);
        CLISetSwitchDescription(CLI, LogFile, (UTF8String)Switch2Description);
        CLISetSwitchType(CLI, LogFile, SwitchCantHaveSlaves);
        
        CLISetSwitchName(CLI, SilenceLevel, (UTF8String)Switch3Name);
        CLISetSwitchDescription(CLI, SilenceLevel, (UTF8String)Switch3Description);
        CLISetSwitchType(CLI, SilenceLevel, SwitchCantHaveSlaves);
        
        CLISetSwitchName(CLI, Help, (UTF8String)Switch4Name);
        CLISetSwitchDescription(CLI, Help, (UTF8String)Switch4Description);
        CLISetSwitchType(CLI, Help, ExistentialSwitch);
        CLISetHelpSwitch(CLI, Help);
        
        return CLI;
    }
    
    void RemoveEmptySamples(PCMFile *PCM, uint32_t NumChannels, uint32_t NumSamples, uint32_t **AudioSamples) {
        uint32_t  CurrentSampleIndex = 0UL;
        uint8_t   BitDepth           = PCM_GetBitDepth(PCM);
        uint32_t *CurrentSampleValue = calloc(NumChannels, NumSamples * Bits2Bytes(BitDepth, Yes));
        for (uint32_t Channel = 0UL; Channel < NumChannels; Channel++) {
            for (uint32_t Sample = 0UL; Sample < NumSamples; Sample++) {
                // Ok, so we need to check each sample to see if it is 0, and if it is, make sure all samples in that group are zer0, then loop until we find a non-zero sample.
                while (AudioSamples[Channel][Sample] == 0x0) {
                    CurrentSampleIndex += 1;
                }
            }
        }
        free(CurrentSampleValue);
    }
    
    int main(int argc, const char *argv[]) {
        
        CommandLineIO *CLI                = SetTrimSilenceOptions();
        BitInput      *BitI               = BitInput_Init();
        BitOutput     *BitO               = BitOutput_Init();
        PCMFile       *PCM                = PCMFile_Init();
        BitBuffer     *BitB               = BitBuffer_Init(40);
        
        ParseCommandLineOptions(CLI, argc, argv);
        
        int64_t InputFileOption           = CLIGetOptionNum(CLI, Input, 0, NULL);
        int64_t OutputFileOption          = CLIGetOptionNum(CLI, Output, 0, NULL);
        int64_t LogFileOption             = CLIGetOptionNum(CLI, LogFile, 0, NULL);
        int64_t SilenceLevelOption        = CLIGetOptionNum(CLI, SilenceLevel, 0, NULL);
        
        UTF8String  InputPath             = CLIGetOptionResult(CLI, InputFileOption);
        UTF8String  OutputPath            = CLIGetOptionResult(CLI, OutputFileOption);
        UTF8String  OutputExtension       = GetExtensionFromPath(OutputPath);
        uint64_t    OutputExtensionSize   = UTF8String_GetNumCodePoints(OutputExtension);
        UTF8String  LogFilePath           = CLIGetOptionResult(CLI, LogFileOption);
        UTF8String  SilenceLevel          = CLIGetOptionResult(CLI, SilenceLevelOption);
        uint64_t    SilenceLevelSize      = UTF8String_GetNumCodePoints(SilenceLevel);
        UTF32String SilenceLevelUTF32     = UTF8String_Decode(SilenceLevel, SilenceLevelSize);
        int64_t     SilenceValue          = UTF32String_ToNumber(SilenceLevelUTF32, SilenceLevelSize);
        
        BitInput_OpenFile(BitI, InputPath);
        BitIOLog_OpenFile(LogFilePath);
        BitOutput_OpenFile(BitO, OutputPath);
        
        UTF8Constant WAVExtension  = u8".wav";
        UTF8Constant W64Extension  = u8".w64";
        UTF8Constant AIFExtension  = u8".aif";
        UTF8Constant AIFFExtension = u8".aiff";
        
        UTF8String_Compare(OutputExtension, OutputExtensionSize, (UTF8String)WAVExtension,  4, Yes, Yes);
        UTF8String_Compare(OutputExtension, OutputExtensionSize, (UTF8String)W64Extension,  4, Yes, Yes);
        UTF8String_Compare(OutputExtension, OutputExtensionSize, (UTF8String)AIFExtension,  4, Yes, Yes);
        UTF8String_Compare(OutputExtension, OutputExtensionSize, (UTF8String)AIFFExtension, 5, Yes, Yes);
        
        if (UTF8String_Compare(OutputExtension, OutputExtensionSize, (UTF8String)WAVExtension,  4, Yes, Yes)) { // WAV output
            PCM_SetOutputFileType(PCM, WAVFormat);
        } else if (UTF8String_Compare(OutputExtension, OutputExtensionSize, (UTF8String)W64Extension,  4, Yes, Yes)) {
            PCM_SetOutputFileType(PCM, W64Format);
        } else if (UTF8String_Compare(OutputExtension, OutputExtensionSize, (UTF8String)AIFExtension,  4, Yes, Yes)) {
            PCM_SetOutputFileType(PCM, AIFFormat);
        } else if (UTF8String_Compare(OutputExtension, OutputExtensionSize, (UTF8String)AIFFExtension, 5, Yes, Yes)) {
            PCM_SetOutputFileType(PCM, AIFFormat);
        } else {
            BitIOLog(BitIOLog_ERROR, u8"TrimSilence", __func__, "Unknown extension: %s", OutputExtension);
        }
        
        // So now we go ahead and mess around with the samples, looking for empty SampleGroups, then write it all out with the generic Write functions that I need to write.
        
        PCMFile_Identify(PCM, BitB);
        PCMFile_ParseMetadata(PCM, BitB);
        
        // Honestly, fuck this; I'm just gonna read all the samples in at once.
        
        uint64_t NumSamples         = PCM_GetNumSamples(PCM);
        uint64_t NumChannels        = PCM_GetNumChannels(PCM);
        uint8_t  BitDepth           = PCM_GetBitDepth(PCM);
        
        uint32_t **ExtractedSamples = calloc(NumSamples * NumChannels, Bits2Bytes(BitDepth, Yes));
        
        PCM_ExtractSamples(PCM, BitB, NumSamples, ExtractedSamples);
        
        free(ExtractedSamples);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
