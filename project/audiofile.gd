extends MarginContainer

@export var filename : String
var drawn : bool = false
var effect : AudioEffectCapture

var audiobuf : Array
var soundlen : int

const MainPage = preload("res://mainpage.gd")

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func set_file(fname):
	var strm = GStrm64.handle
	#MainPage.set_strm64_properties(strm)
	
	audiobuf = strm.get_waveform()
	queue_redraw()
	

func _draw():
	var x = 0
	var xpos = 0
	var numChannels = len(audiobuf)
	var maxX = len(audiobuf[0])
	print_debug("draw, ", numChannels)
	for channel in len(audiobuf):
		for sample in audiobuf[channel]:
			draw_line(Vector2(xpos, 100 * channel), Vector2(xpos, sample / 100), Color(0,0,255,255), 1.0, false)
			x += 1
			xpos += 1

func _on_sound_finished():
	queue_redraw()
