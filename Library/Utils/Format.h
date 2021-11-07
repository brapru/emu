#pragma once

#ifndef __EMSCRIPTEN__
#    include <fmt/format.h>
#else
#    define FMT_HEADER_ONLY
#    include "fmt/format.h"
#endif

#include <iostream>
#include <string>

inline void vout(fmt::string_view format, fmt::format_args args, bool newline = false)
{
    //fmt::print("[-] ");

    fmt::vprint(format, args);

    if (newline)
        fmt::print("\n");
}

template<typename T, typename... Args>
void out(T const& format, Args&&... args)
{
    vout(format, fmt::make_format_args(args...));
}

template<typename T, typename... Args>
void outln(T const& format, Args&&... args)
{
    vout(format, fmt::make_format_args(args...), true);
}