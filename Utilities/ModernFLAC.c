#include "../libModernFLAC/include/DecodeFLAC.h"
#include "../libModernFLAC/include/ParseFLAC.h"

#include "../libModernFLAC/include/EncodeFLAC.h"

void FLACDecodeFile(int argc, const char *argv[]) {
    BitInput    *BitI  = calloc(sizeof(BitInput), 1);
    BitOutput   *BitO  = calloc(sizeof(BitOutput), 1);
    ErrorStatus *Error = calloc(sizeof(ErrorStatus), 1);
    FLACDecoder *Dec   = calloc(sizeof(FLACDecoder), 1);
    InitBitInput(BitI, Error, argc, argv);
    InitBitOutput(BitO, Error, argc, argv);
    InitFLACDecoder(Dec);
    
    uint32_t FileMagic = ReadBits(BitI, 32);
    
    if (FileMagic != FLACMagic) {
        // Not a FLAC file
        char Error[BitIOStringSize];
        snprintf(Error, BitIOStringSize, "Not a FLAC file, magic was: 0x%X\n", FileMagic);
        Log(SYSError, "ModernFLAC", "FLACDecodeFile", Error);
    } else {
        bool IsLastBlock = false;
        for (size_t Byte = 4; Byte < BitI->FileSize; Byte++) { // loop to decode file
            while (IsLastBlock == false) {
                IsLastBlock = FLACParseMetadata(BitI, Dec);
            }
            FLACReadFrame(BitI, Dec);
        }
    }
}

int main(int argc, const char *argv[]) {
    FLACDecodeFile(argc, argv);
    return EXIT_SUCCESS;
}
