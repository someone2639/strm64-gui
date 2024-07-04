extends MarginContainer

var strm = GStrm64.handle

func set_strm64_properties(strm):
	# TODO: validate the user put in correct options
	if get_node("layout/Content/entries/outputfile/onoff").toggled:
		strm.set_output_file(get_node("layout/Content/entries/outputfile").path)
	else:
		strm.generate_filename()

	var samplerate = get_node("layout/Content/entries/samplerate/buttons/option_text").text
	if len(samplerate) > 0:
		strm.set_sample_rate(int(samplerate))
	
	var resamplerate = get_node("layout/Content/entries/resamplerate/buttons/option_text").text
	if len(resamplerate) > 0:
		strm.set_resample_rate(int(resamplerate))
	
	var channelcount = get_node("layout/Content/entries/channelcount/buttons/option_text").text
	if len(channelcount) > 0:
		strm.set_channel_count(int(channelcount))
	
	var mutescale = get_node("layout/Content/entries/mutescale/option_text").text
	if len(mutescale) > 0:
		strm.set_mute_scale(int(mutescale))

	var vol = get_node("layout/Content/entries/mastervolume/option_text").text
	if len(vol) > 0:
		strm.set_master_volume(int(vol))
	

	strm.set_mix_to_mono(get_node("layout/Content/entries/mixmono").state)
	strm.set_generate_stream(not get_node("layout/Content/entries/skipstream").state)
	strm.set_generate_sequence(not get_node("layout/Content/entries/skipseq").state)
	strm.set_generate_soundbank(not get_node("layout/Content/entries/skipbnk").state)


func _on_export_button_pressed():
	set_strm64_properties(strm)
	strm.run()

# INPUT FILE signals
func _on_pick_input_file_pressed():
	get_node("layout/file_dialog").show()

func _on_file_dialog_file_selected(path):
	get_node("layout/Content/entries/inputfile/filename").text = path
	strm.set_input_file(path)
	GStrm64.audiobuf = strm.get_waveform()
	get_node("layout/Content/entries/audio_wave/VBoxContainer/waveform").queue_redraw()











# WAVEFORM signals
