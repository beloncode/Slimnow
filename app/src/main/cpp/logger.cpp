// This source code is part of SlimNow project

#include <logger.h>

namespace slim {
   thread_local std::array<char, messageBufferSz> Logger::m_uniqueBuffer;

}
