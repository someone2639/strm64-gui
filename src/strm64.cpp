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

STRM64::~STRM64() { }

void STRM64::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("run"), &STRM64::run);
    ClassDB::bind_method(D_METHOD("generate_filename"), &STRM64::generate_filename);

    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "filename", PROPERTY_HINT_RANGE, "0,20,0.01"),
        "set_filename",
        "get_filename"
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "forced_mono"),
        "set_mono",
        "get_mono"
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "gen_stream"),
        "set_stream",
        "get_stream"
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "gen_sequence"),
        "set_seq",
        "get_seq"
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "gen_soundbank"),
        "set_bank",
        "get_bank"
    );

    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "output_filename", PROPERTY_HINT_RANGE, "0,20,0.01"),
        "set_output",
        "get_output"
    );
}


int STRM64::get_vgmstream_properties(const char *inFilename) {
    printf("Opening %s for reading...", inFilename);
    fflush(stdout);

    if (!this->inFileProperties) {
        FILE *invalidFile = fopen(inFilename, "r");
        if (invalidFile == NULL) {
            printf("...FAILED!\nERROR: Input file cannot be found or opened!\n");
            return RETURN_CANNOT_FIND_INPUT_FILE;
        }
        fclose(invalidFile);

        printf("...FAILED!\nERROR: Input file is not a valid audio file!\nIf you believe this is a fluke, please make sure you have the proper audio libraries installed.\n");
        printf("Alternatively, you can convert the input file to WAV (16-bit) separately and try again.\n");
        return RETURN_INVALID_INPUT_FILE;
    }

    if (this->inFileProperties->channels <= 0) {
        printf("...FAILED!\nERROR: Audio must have at least 1 channel!\nCONTAINS: %d channels\n", this->inFileProperties->channels);
        close_vgmstream(this->inFileProperties);
        return RETURN_NOT_ENOUGH_CHANNELS;
    }

    if (inFileProperties->channels > (int) NUM_CHANNELS_MAX) {
        printf("...FAILED!\nERROR: Audio file exceeds maximum of %d channels!\nCONTAINS: %d channels\n", (int) NUM_CHANNELS_MAX, this->inFileProperties->channels);
        close_vgmstream(this->inFileProperties);
        return RETURN_TOO_MANY_CHANNELS;
    }

    // Currently using bitflags to represent channels serves no purpose, but it may be easier to automate for decomp soundbank optimization in the future.
    this->gInstFlags = (1ULL << this->inFileProperties->channels) - 1ULL;

    printf("...SUCCESS!\n");

    return RETURN_SUCCESS;
}

void STRM64::set_filename(string &s) {
    this->filename = s;
    this->inFileProperties = init_vgmstream(this->filename.c_str());
    int ret = get_vgmstream_properties(this->filename.c_str());
    if (ret) {
        // TODO: actually find out how to make this send an error back to godot
    }
}

void STRM64::run() {
    int ret;

    if (this->inFileProperties) {
        ret = generate_new_streams(this->inFileProperties, newFilename, this->filename.c_str(), this->generateStreams);

        // TODO: decide whether to communicate this to godot (and how)
        // if (!ret && !this->generateStreams)
        //     print_seq_channels(this->gInstFlags);

        if (generateSequence) {
            if (!ret)
                ret = generate_new_sequence(newFilename, this->gInstFlags);
            else
                generate_new_sequence(newFilename, this->gInstFlags);
        }

        if (generateSoundbank) {
            if (!ret)
                ret = generate_new_soundbank(newFilename, this->gInstFlags);
            else
                generate_new_soundbank(newFilename, this->gInstFlags);
        }
        close_vgmstream(this->inFileProperties);
        this->inFileProperties = nullptr;
    } else {
        // do nothing... (since the user has no control over this)
    }
}

extern string replace_spaces(string);
extern string strip_extension(string);
void STRM64::generate_filename() {
    this->newFilename = replace_spaces(this->newFilename);
    this->newFilename = strip_extension(this->newFilename);
}

string STRM64::get_filename() const {return this->filename;};
void STRM64::set_filename(string &s);

string STRM64::get_output() const {return this->newFilename;};
void STRM64::set_output(string &s) {this->newFilename = s;};

bool STRM64::get_mono() const {return this->forcedMono;};
void STRM64::set_mono(bool b) {this->forcedMono = b;};

bool STRM64::get_stream() const {return this->generateStreams;};
void STRM64::set_stream(bool b) {this->generateStreams = b;};

bool STRM64::get_seq() const {return this->generateSequence;};
void STRM64::set_seq(bool b) {this->generateSequence = b;};

bool STRM64::get_bank() const {return this->generateSoundbank;};
void STRM64::set_bank(bool b) {this->generateSoundbank = b;};

void STRM64::test() {
    set_filename("/home/faris/")
}

STRM64::STRM64() {
    this->inFileProperties = nullptr;
}

