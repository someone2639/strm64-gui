extends HBoxContainer


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func _on_export_button_pressed():
	# get strm64
	var strm = GStrm64.handle

	strm.set_input_filename(get_node("inputfile").label)
	if len(get_node("outputfile").label) > 0: # TODO: change to checkbox
		strm.set_output_filename(get_node("outputfile").label)
	else:
		strm.generate_filename()
	

	strm.set_mix_mono(get_node("mixmono").state)
	strm.set_generate_stream(not get_node("skipstream").state)
	strm.set_generate_sequence(not get_node("skipseq").state)
	strm.set_generate_bank(not get_node("skipbank").state)

	strm.run()
	pass # Replace with function body.
