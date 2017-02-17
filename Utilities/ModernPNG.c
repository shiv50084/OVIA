#include "../libModernPNG/include/libModernPNG.h"
#include "../libModernPNG/include/DecodePNG.h"
#include "../libModernPNG/include/EncodePNG.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    int main(int argc, const char * argv[]) {
        BitInput    *BitI  = calloc(sizeof(BitInput), 1);
        BitOutput   *BitO  = calloc(sizeof(BitOutput), 1);
        PNGDecoder  *PNG   = calloc(sizeof(PNGDecoder), 1);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
