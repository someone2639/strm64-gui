extends MarginContainer

func _on_export_button_pressed():
	# get strm64
	var strm = GStrm64.handle

	strm.set_input_file(get_node("layout/Content/entries/inputfile").path)
	if get_node("layout/Content/entries/outputfile/onoff").toggled:
		strm.set_output_file(get_node("layout/Content/entries/outputfile").path)
	else:
		strm.generate_filename()

	strm.set_mix_to_mono(get_node("layout/Content/entries/mixmono").state)
	strm.set_generate_stream(not get_node("layout/Content/entries/skipstream").state)
	strm.set_generate_sequence(not get_node("layout/Content/entries/skipseq").state)
	strm.set_generate_soundbank(not get_node("layout/Content/entries/skipbnk").state)

	print_debug("Running...")
	strm.run()
	print_debug("Complete...")
	pass # Replace with function body.
