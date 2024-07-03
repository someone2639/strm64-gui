// reimplementation of STRM64's main.cpp in a godot extension form.
// The godot-cpp-template repo is unlicense
//  but this file (originally summator.cpp) had a copyright string
//  (which i will not touch and is below)
//© Copyright 2014-2022, Juan Linietsky, Ariel Manzur and the Godot community (CC-BY 3.0)
#include <godot_cpp/core/class_db.hpp>

#include "strm64.h"
#include "main.hpp"
#include "stream.hpp"
#include "sequence.hpp"
#include "soundbank.hpp"

using namespace godot;

STRM64::~STRM64() {
}

string NewString(const String s) {
    CharString buf = s.utf8();

    return string(buf.get_data());
}

void STRM64::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("generate_filename"), &STRM64::generate_filename);
    ClassDB::bind_method(D_METHOD("set_input_file", "p_filename"), &STRM64::set_filename);
    ClassDB::bind_method(D_METHOD("set_output_file", "p_output"), &STRM64::set_output);
    ClassDB::bind_method(D_METHOD("set_mix_to_mono", "b"), &STRM64::set_mono);
    ClassDB::bind_method(D_METHOD("set_generate_stream", "b"), &STRM64::set_stream);
    ClassDB::bind_method(D_METHOD("set_generate_sequence", "b"), &STRM64::set_seq);
    ClassDB::bind_method(D_METHOD("set_generate_soundbank", "b"), &STRM64::set_bank);

    ClassDB::bind_method(D_METHOD("run"), &STRM64::run);
}

void STRM64::set_filename(String p_filename) {
    this->filename = NewString(p_filename);
    inFileProperties = init_vgmstream(this->filename.c_str());
    int ret = get_vgmstream_properties(this->filename.c_str());
    if (ret) {
        printf("ERROR: %d\n", ret);
        // TODO: actually find out how to make this send an error back to godot
    }
}

void STRM64::run() {
    int ret;

    printf("Debug State: generateStreams %d genSeq %d genBank %d\n", generateStreams, generateSequence, generateSoundbank);

    if (inFileProperties) {
        ret = generate_new_streams(inFileProperties, newFilename, this->filename.c_str(), generateStreams);

        // TODO: decide whether to communicate this to godot (and how)
        if (!ret && !generateStreams)
            print_seq_channels(gInstFlags);

        if (generateSequence) {
            if (!ret)
                ret = generate_new_sequence(newFilename, gInstFlags);
            else
                generate_new_sequence(newFilename, gInstFlags);
        }

        if (generateSoundbank) {
            if (!ret)
                ret = generate_new_soundbank(newFilename, gInstFlags);
            else
                generate_new_soundbank(newFilename, gInstFlags);
        }
        close_vgmstream(inFileProperties);
        inFileProperties = nullptr;
    } else {
        // do nothing... (since the user has no control over this)
    }
}

extern string replace_spaces(string);
extern string strip_extension(string);
void STRM64::generate_filename() {
    this->newFilename = replace_spaces(this->filename);
    this->newFilename = strip_extension(this->newFilename);
}

void STRM64::set_output(String p_output) {this->newFilename = NewString(p_output);}

void STRM64::set_mono(bool b) {
    printf("SETTING MONO: %d\n", b);
    forcedMono = b;
}
void STRM64::set_stream(bool b) {
    printf("SETTING STREAM: %d\n", b);
    generateStreams = b;
}
void STRM64::set_seq(bool b) {
    printf("SETTING SEQ: %d\n", b);
    generateSequence = b;
}
void STRM64::set_bank(bool b) {
    printf("SETTING BANK: %d\n", b);
    generateSoundbank = b;
}

STRM64::STRM64() {
    inFileProperties = nullptr;
}

