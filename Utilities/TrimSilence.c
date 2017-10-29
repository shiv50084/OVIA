#include "../Dependencies/BitIO/libBitIO/include/CommandLineIO.h"

#include "../libPCM/include/libPCM.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define TrimSilenceVersion "0.1.0"
    
    enum CommandLineSwitchNames {
        Input   = 0,
        Output  = 1,
        LogFile = 2,
        Silence = 3,
        Help    = 4,
    };
    
    CommandLineIO *SetTrimSilenceOptions(void) {
        CommandLineIO     *CLI = CommandLineIOInit(4);
        
        SetCLIName(CLI, "TrimSilence");
        SetCLIVersion(CLI, TrimSilenceVersion);
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLICopyright(CLI, "2017-2017");
        SetCLIDescription(CLI, "PCM silence remover written from scratch in modern C");
        SetCLILicense(CLI, "Revised BSD", "Permissive open source license", "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCLIMinArguments(CLI, 3);
        
        SetCLISwitchFlag(CLI, Input, "Input");
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: '-'\n");
        SetCLISwitchAsIndependent(CLI, Input);
        
        SetCLISwitchFlag(CLI, Output, "Output");
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: '-'\n");
        SetCLISwitchAsIndependent(CLI, Output);
        
        SetCLISwitchFlag(CLI, LogFile, "LogFile");
        SetCLISwitchDescription(CLI, LogFile, "Where should the logs be written? if unspecified, logs are written to STDERR");
        SetCLISwitchAsIndependent(CLI, LogFile);
        
        SetCLISwitchFlag(CLI, Silence, "Silence");
        SetCLISwitchDescription(CLI, Silence, "Set the threshhold, in dB or absolute value like: (-|--|/)Silence 12dB or: (-|--|/)Silence 0");
        SetCLISwitchAsIndependent(CLI, Silence);
        
        SetCLISwitchFlag(CLI, Help, "Help");
        SetCLISwitchDescription(CLI, Help, "Prints all the command line options");
        SetCLISwitchAsIndependent(CLI, Help);
        
        return CLI;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineIO *CLI         = SetTrimSilenceOptions();
        ParseCommandLineArguments(CLI, argc, argv);
        BitInput            *BitI  = BitInputInit();
        BitOutput           *BitO  = BitOutputInit();
        PCMFile             *PCM   = PCMFileInit();
        BitBuffer           *BitB  = BitBufferInit(40);
        
        uint64_t InputFileArg      = GetCLIArgumentNumWithIndependentAndDependents(CLI, Input, 0);
        uint64_t OutputFileArg     = GetCLIArgumentNumWithIndependentAndDependents(CLI, Output, 0);
        
        BitInputOpenFile(BitI, GetCLIArgumentResult(CLI, InputFileArg));
        BitOutputOpenFile(BitO, GetCLIArgumentResult(CLI, OutputFileArg));
        
        IdentifyPCMFile(PCM, BitB);
        
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
