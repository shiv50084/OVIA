#include <stdlib.h>

#include "../Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../Dependencies/FoundationIO/libFoundationIO/include/Math.h"
#include "../Dependencies/FoundationIO/libFoundationIO/include/CommandLineIO.h"
#include "../Dependencies/FoundationIO/libFoundationIO/include/StringIO.h"

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
        
        UTF8 ProgramName[] = U8("TrimSilence");
        UTF8 ProgramVersion[] = U8("0.1.0");
        UTF8 ProgramAuthor[] = U8("BumbleBritches57");
        UTF8 ProgramCopyright[] = U8("2017");
        UTF8 ProgramDescription[] = U8("Removes leading and trailing silence");
        UTF8 LicenseName[] = U8("Revised BSD");
        UTF8 LicenseURL[] = U8("https://tldrlegal.com/license/bsd-3-clause-license-(revised)");
        
        UTF8 InputSwitchName[] = U8("Input");
        UTF8 InputSwitchDescription[] = U8("Input file path or stdin");
        
        UTF8 OutputSwitchName[] = U8("Output");
        UTF8 OutputSwitchDescription[] = U8("Output file path or stdout");
        
        UTF8 LogFileSwitchName[] = U8("LogFile");
        UTF8 LogFileSwitchDescription[] = U8("Log file path or stderr");
        
        UTF8 SilenceLevelSwitchName[] = U8("SilenceLevel");
        UTF8 SilenceLevelSwitchDescription[] = U8("Absolute value of cutoff for what's considered silence");
        
        UTF8 HelpSwitchName[] = U8("Help");
        UTF8 HelpSwitchDescription[] = U8("Prints all the command line options and their relationships");
        
        Log_SetProgramName(ProgramName);
        
        CLISetName(CLI, ProgramName);
        CLISetVersion(CLI, ProgramVersion);
        CLISetAuthor(CLI, ProgramAuthor);
        CLISetCopyright(CLI, ProgramCopyright);
        CLISetDescription(CLI, ProgramDescription);
        CLISetLicense(CLI, PermissiveLicense, LicenseName, LicenseURL);
        CLISetMinOptions(CLI, 3);
        
        CLISetSwitchName(CLI, Input, InputSwitchName);
        CLISetSwitchDescription(CLI, Input, InputSwitchDescription);
        CLISetSwitchType(CLI, Input, SwitchCantHaveSlaves);
        
        CLISetSwitchName(CLI, Output, OutputSwitchName);
        CLISetSwitchDescription(CLI, Output, OutputSwitchDescription);
        CLISetSwitchType(CLI, Output, SwitchCantHaveSlaves);
        
        CLISetSwitchName(CLI, LogFile, LogFileSwitchName);
        CLISetSwitchDescription(CLI, LogFile, LogFileSwitchDescription);
        CLISetSwitchType(CLI, LogFile, SwitchCantHaveSlaves);
        
        CLISetSwitchName(CLI, SilenceLevel, SilenceLevelSwitchName);
        CLISetSwitchDescription(CLI, SilenceLevel, SilenceLevelSwitchDescription);
        CLISetSwitchType(CLI, SilenceLevel, SwitchCantHaveSlaves);
        
        CLISetSwitchName(CLI, Help, HelpSwitchName);
        CLISetSwitchDescription(CLI, Help, HelpSwitchDescription);
        CLISetSwitchType(CLI, Help, ExistentialSwitch);
        CLISetHelpSwitch(CLI, Help);
        
        return CLI;
    }
    
    static void RemoveEmptySamples(PCMFile *PCM, uint32_t NumChannels, uint32_t NumSamples, uint32_t **AudioSamples) {
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
        BitBuffer     *BitB               = NULL;
        
#if FoundationIOTargetOS == POSIXOS
        UTF8_ParseCommandLineOptions(CLI, argc, (UTF8**)argv);
#elif FoundationIOTargetOS == WindowsOS
        UTF16_ParseCommandLineOptions(CLI, __argc, __wargv);
#endif
        
        int64_t InputFileOption           = CLIGetOptionNum(CLI, Input, 0, NULL);
        int64_t OutputFileOption          = CLIGetOptionNum(CLI, Output, 0, NULL);
        int64_t LogFileOption             = CLIGetOptionNum(CLI, LogFile, 0, NULL);
        int64_t SilenceLevelOption        = CLIGetOptionNum(CLI, SilenceLevel, 0, NULL);
        
        UTF8       *InputPath             = CLIGetOptionResult(CLI, InputFileOption);
        UTF8       *OutputPath            = CLIGetOptionResult(CLI, OutputFileOption);
        UTF8       *OutputExtension       = GetExtensionFromPath(OutputPath);
        UTF8       *LogFilePath           = CLIGetOptionResult(CLI, LogFileOption);
        UTF8       *SilenceLevelString8   = CLIGetOptionResult(CLI, SilenceLevelOption);
        uint64_t    SilenceLevelSize      = UTF8_GetSizeInCodePoints(SilenceLevel);
        UTF32      *SilenceLevelString32  = UTF8_Decode(SilenceLevel);
        //int64_t     SilenceValue          = UTF32_ToNumber(SilenceLevelString32);
        
        BitInput_OpenFile(BitI, InputPath);
        Log_OpenFile(LogFilePath);
        BitOutput_OpenFile(BitO, OutputPath);
        
        UTF8 *WAVExtension  = U8(".wav");
        UTF8 *W64Extension  = U8(".w64");
        UTF8 *AIFExtension  = U8(".aif");
        UTF8 *AIFFExtension = U8(".aiff");
        
        if (UTF8_Compare(OutputExtension, WAVExtension, No, Yes)) { // WAV output
            PCM_SetOutputFileType(PCM, WAVFormat);
        } else if (UTF8_Compare(OutputExtension, W64Extension, No, Yes)) {
            PCM_SetOutputFileType(PCM, W64Format);
        } else if (UTF8_Compare(OutputExtension, AIFExtension, No, Yes)) {
            PCM_SetOutputFileType(PCM, AIFFormat);
        } else if (UTF8_Compare(OutputExtension, AIFFExtension, No, Yes)) {
            PCM_SetOutputFileType(PCM, AIFFormat);
        } else {
            Log(Log_ERROR, __func__, U8("Unknown extension: %s"), OutputExtension);
        }
        
        BitB = BitBuffer_Init(128);
        
        
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
