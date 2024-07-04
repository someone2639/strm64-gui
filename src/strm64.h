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

extern bool customNewFilename;
extern bool generateStreams;
extern bool generateSequence;
extern bool generateSoundbank;
extern bool printedHelp;
extern bool forcedMono;
extern uint16_t gInstFlags;
extern VGMSTREAM *inFileProperties;
extern uint8_t channelCountOverride;

extern void print_seq_channels(uint16_t instFlags);
extern int get_vgmstream_properties(const char *inFilename);

using namespace godot;
using namespace std;

class STRM64 : public RefCounted
{
    GDCLASS(STRM64, RefCounted);

private:
    string filename; // input
    string newFilename; // output
    // PackedInt32Array waveform_l;
    // PackedInt32Array waveform_r;

protected:
    static void _bind_methods();

public:
    STRM64();
    ~STRM64();

    void run();

    void set_filename(String p_filename);
    void set_output(String p_output);

    void set__sample_rate(const int64_t p_rate);
    void set__resample_rate(const int64_t p_rate);
    void set_loop(bool b);
    // loop start (TODO: epic gui)
    // loop end (TODO: epic gui)
    void set_custom_channel_count(const int64_t p_count);
    void set_mute_scale(const int64_t p_vol);
    void set_master_volume(const int64_t p_vol);

    void set_mono(bool b);
    void set_stream(bool b);
    void set_seq(bool b);
    void set_bank(bool b);

    Array get_waveform();

    void generate_filename();
};

#endif // STRM64_CLASS_H