#include "../../include/Private/Common/ModernFLACCommon.h"

inline int64_t OnesCompliment2TwosCompliment(const int64_t OnesCompliment) {
    return ~OnesCompliment + 1;
}

inline int64_t TwosCompliment2OnesCompliment(const int64_t TwosCompliment) {
    return TwosCompliment ^ 0xFFFFFFFFFFFFFFFF;
}
