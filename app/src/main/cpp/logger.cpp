#include <logger.h>

namespace slim {
   thread_local char Logger::m_uniqueBuffer[MESSAGE_BUFFER_SZ];

}
