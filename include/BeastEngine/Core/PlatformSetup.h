#pragma once
#ifdef BE_PLATFORM_WINDOWS
    #ifndef UNICODE
        #define UNICODE
    #endif

    // Make sure to use only required stuff from windows
    #define WIN32_LEAN_AND_MEAN

// The following list contains commented directives
// in order to keep the list of all possible defines in one place.
// Comment out the elements that are required and #define the macros for unused Windows stuff

    #define NOGDICAPMASKS // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
    //#define NOVIRTUALKEYCODES		// VK_*
    //#define NOWINMESSAGES			// WM_*, EM_*, LB_*, CB_*
    //#define NOWINSTYLES			// WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
    //#define NOSYSMETRICS  // SM_*
    #define NOMENUS // MF_*
    #define NOICONS // IDI_*
    //#define NOKEYSTATES   // MK_*
    #define NOSYSCOMMANDS // SC_*
    #define NORASTEROPS   // Binary and Tertiary raster ops
    //#define NOSHOWWINDOW			// SW_*
    #define OEMRESOURCE // OEM Resource values
    #define NOATOM      // Atom Manager routines
    #define NOCLIPBOARD // Clipboard routines
    #define NOCOLOR     // Screen colors
    #define NOCTLMGR    // Control and Dialog routines
    #define NODRAWTEXT  // DrawText() and DT_*
    //#define NOGDI       // All GDI #defines and routines
    #define NOKERNEL // All KERNEL #defines and routines
    //#define NOUSER    // All USER #defines and routines
    //#define NONLS		// All NLS #defines and routines
    #define NOMB       // MB_* and MessageBox()
    #define NOMEMMGR   // GMEM_*, LMEM_*, GHND, LHND, associated routines
    #define NOMETAFILE // typedef METAFILEPICT
    #define NOMINMAX   // Macros min(a,b) and max(a,b)
    //#define NOMSG          // typedef MSG and associated routines
    #define NOOPENFILE   // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
    #define NOSCROLL     // SB_* and scrolling routines
    #define NOSERVICE    // All Service Controller routines, SERVICE_ equates, etc.
    #define NOSOUND      // Sound driver routines
    #define NOTEXTMETRIC // typedef TEXTMETRIC and associated routines
    #define NOWH         // SetWindowsHook and WH_*
    //#define NOWINOFFSETS     // GWL_*, GCL_*, associated routines
    #define NOCOMM           // COMM driver routines
    #define NOKANJI          // Kanji support stuff.
    #define NOHELP           // Help engine interface.
    #define NOPROFILER       // Profiler interface.
    #define NODEFERWINDOWPOS // DeferWindowPos routines
    #define NOMCX            // Modem Configuration Extensions
    #define NOCRYPT
    #define NOTAPE
    #define NOIMAGE
    #define NOPROXYSTUB
    #define NORPC

    // Use Windows types strict mode to increase type safety
    #ifndef STRICT
        #define STRICT
    #endif
    #include <Windows.h>
#endif

namespace be
{
#ifndef BE_WINDOW_HANDLE_INSTANCE_TYPE
    #define BE_WINDOW_HANDLE_INSTANCE_TYPE 1
    #ifdef BE_PLATFORM_WINDOWS
    using WindowHandleInstanceType = HINSTANCE;
    #else
        #error "No instance defined for this platform!"
    #endif
#endif
} // namespace be
