#if defined(_DEBUG)
#define XLOG(format, ...)\
        do {\
            char _buffer[4096];\
            int ret = _snprintf_s(_buffer, std::size(_buffer), _TRUNCATE, "%s(%d) "##format, __FILE__, __LINE__, __VA_ARGS__);\
            OutputDebugStringA(_buffer);\
            if (ret == -1) OutputDebugStringA("** message truncated **\n");\
            OutputDebugStringA("\n");\
        } while (false)

#define XASSERT(condition, format, ...)\
        do {\
            if (!(condition))\
            {\
                XLOG(format, __VA_ARGS__);\
                DebugBreak();\
            }\
        } while (false)

#define XVERIFY(condition, format, ...)\
        do {\
            if (!(condition))\
            {\
                XLOG(format, __VA_ARGS__);\
                DebugBreak();\
            }\
        } while (false)
#else
#define XLOG(format, ...)
#define XASSERT(condition, format, ...)
#define XVERIFY(condition, format, ...) condition;
#endif
