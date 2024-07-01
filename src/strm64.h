//© Copyright 2014-2022, Juan Linietsky, Ariel Manzur and the Godot community (CC-BY 3.0)
#ifndef STRM64_CLASS_H
#define STRM64_CLASS_H

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/ref.hpp>

extern "C" {
#include "vgmstream.h"
}

using namespace godot;
using namespace std;

class STRM64 : public RefCounted
{
    GDCLASS(STRM64, RefCounted);

private:
    string filename; // input
    string newFilename; // output
    VGMSTREAM *inFileProperties;
    uint16_t gInstFlags;
    bool generateStreams = true;
    bool generateSequence = true;
    bool generateSoundbank = true;
    bool printedHelp = false;

    bool forcedMono = false;
    uint8_t channelCountOverride = 0;
    string duplicateStringName = "";

protected:
    static void _bind_methods();

public:
    STRM64();
    ~STRM64();

    void run();

    string get_filename() const;
    void set_filename(string &s);

    string get_output() const;
    void set_output(string &s);

    bool get_mono() const;
    void set_mono(bool b);

    bool get_stream() const;
    void set_stream(bool b);

    bool get_seq() const;
    void set_seq(bool b);

    bool get_bank() const;
    void set_bank(bool b);

    void generate_filename();

    void test();

    int get_vgmstream_properties(const char *inFilename);
};

#endif // STRM64_CLASS_H