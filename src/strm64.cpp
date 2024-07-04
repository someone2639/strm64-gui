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
#include <byteswap.h>

using namespace godot;

STRM64::~STRM64() {
}

string NewString(const String s) {
    CharString buf = s.utf8();

    return string(buf.get_data());
}

Array STRM64::get_waveform() {
    int numChannels = inFileProperties->channels;
    int numSamples = inFileProperties->num_samples;

    Array ret;


    uint32_t samplesPadded = (uint32_t) numSamples;
    if (samplesPadded % SAMPLE_COUNT_PADDING)
        samplesPadded += SAMPLE_COUNT_PADDING - (samplesPadded % SAMPLE_COUNT_PADDING);

    uint32_t bufferSize = MIN_PRINT_BUFFER_SIZE;
    if (MIN_PRINT_BUFFER_SIZE < SAMPLE_COUNT_PADDING)
        bufferSize = SAMPLE_COUNT_PADDING;

    sample_t *audioBuffer = new sample_t[bufferSize * (size_t) numChannels];

    sample_t **printBuffer = new sample_t*[(size_t) numChannels];
    vector<PackedInt32Array> retchannels;
    for (int i = 0; i < numChannels; i++) {
        printBuffer[i] = new sample_t[bufferSize];
        retchannels.push_back(PackedInt32Array());
    }

    for (uint32_t samplesProcessed = 0; samplesProcessed < samplesPadded; samplesProcessed += bufferSize) {
        render_vgmstream(audioBuffer, bufferSize, inFileProperties);

        // Not using inFileProperties->num_samples here is by intention, so padding is composed of zeros rather than unwanted audio data.
        int64_t samplesToPadStart = ((int64_t) numSamples - samplesProcessed) * (int64_t) numChannels;
        if (samplesToPadStart < 0)
            samplesToPadStart = 0;
        for (int64_t j = samplesToPadStart; j < (int64_t) bufferSize * (int64_t) numChannels; j++)
            audioBuffer[j] = 0;

        for (uint32_t j = 0; j < bufferSize * (uint32_t) numChannels; j++)
            printBuffer[j % numChannels][j / numChannels] = (sample_t) bswap_16((uint16_t) audioBuffer[j]);

        for (int32_t j = 0; j < numChannels; j++) {
            if (samplesProcessed + bufferSize > (uint32_t) samplesPadded) {
                // PackedInt32Array tmp = ret[j];
                for (uint32_t k = 0; k < samplesPadded - samplesProcessed; k += sizeof(sample_t)) {
                    printf("Appending %d...\n", printBuffer[j][k]);
                    retchannels[j].append(printBuffer[j][k]);
                }
                // fwrite(printBuffer[j], sizeof(sample_t), samplesPadded - samplesProcessed, streamFiles[j]);
            }
            else {
                // PackedInt32Array tmp = ret[j];
                for (uint32_t k = 0; k < bufferSize; k += sizeof(sample_t)) {
                    printf("Appending normal %d...\n", printBuffer[j][k]);
                    retchannels[j].append(printBuffer[j][k]);
                }
                // fwrite(printBuffer[j], sizeof(sample_t), bufferSize, streamFiles[j]);
            }
        }
    }

    for (int i = 0; i < numChannels; i++)
        delete[] printBuffer[i];
    delete[] printBuffer;
    delete[] audioBuffer;

    for (int32_t j = 0; j < numChannels; j++) {
        ret.append(retchannels[j]);
    }

    return ret;
}

void STRM64::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_waveform"), &STRM64::get_waveform);
    ClassDB::bind_method(D_METHOD("generate_filename"), &STRM64::generate_filename);
    ClassDB::bind_method(D_METHOD("set_input_file", "p_filename"), &STRM64::set_filename);
    ClassDB::bind_method(D_METHOD("set_output_file", "p_output"), &STRM64::set_output);

    ClassDB::bind_method(D_METHOD("set_sample_rate", "p_rate"), &STRM64::set__sample_rate);
    ClassDB::bind_method(D_METHOD("set_resample_rate", "p_rate"), &STRM64::set__resample_rate);
    ClassDB::bind_method(D_METHOD("set_loop", "b"), &STRM64::set_loop);

    ClassDB::bind_method(D_METHOD("set_channel_count", "p_count"), &STRM64::set_custom_channel_count);
    ClassDB::bind_method(D_METHOD("set_mute_scale", "p_vol"), &STRM64::set_mute_scale);
    ClassDB::bind_method(D_METHOD("set_master_volume", "p_vol"), &STRM64::set_master_volume);

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

void STRM64::set_loop(bool b) { set_enable_loop(b); }
void STRM64::set_mono(bool b) { forcedMono = b; }
void STRM64::set_stream(bool b) { generateStreams = b; }
void STRM64::set_seq(bool b) { generateSequence = b; }
void STRM64::set_bank(bool b) { generateSoundbank = b; }
void STRM64::set__sample_rate(int64_t p_rate) { set_sample_rate(p_rate); }
void STRM64::set__resample_rate(int64_t p_rate) { set_resample_rate(p_rate); }
void STRM64::set_custom_channel_count(const int64_t p_count) { 
    if (seq_set_num_channels(p_count)) {
        channelCountOverride = p_count;
    }
}
void STRM64::set_mute_scale(const int64_t p_vol) { seq_set_mute_scale(p_vol); }
void STRM64::set_master_volume(const int64_t p_vol) { seq_set_master_volume(p_vol); }

STRM64::STRM64() {
    inFileProperties = nullptr;
}

