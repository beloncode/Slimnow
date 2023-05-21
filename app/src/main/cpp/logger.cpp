// This source code is part of SlimNow project

#include <logger.h>

namespace slim {
   thread_local char Logger::m_uniqueBuffer[MESSAGE_BUFFER_SZ];

}
