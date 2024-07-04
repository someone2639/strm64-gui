extends TextureRect

@export var channel : int
var sound : AudioStreamPlayer = AudioStreamPlayer.new()

func _ready():
	add_child(sound)

func _draw():
	var audiobuf = GStrm64.audiobuf
	var x = 0
	var numChannels = len(audiobuf)
	var yvals = MathUtil.interp_range(0, self.size.y, len(audiobuf))

	var colors = [Color(0,0,255,255), Color(255,0,0,255)]

	if len(audiobuf) >= channel + 1:
		var base = yvals[channel]
		var waves = MathUtil.interp_range(0, self.size.x, 4000)
		var half = self.size.y / 2
		for i in waves:
			var sample = audiobuf[channel][i]
			var height = MathUtil.change_range(sample + 16384, 0, 32767, base, base + 64) / 2
			draw_line(
				Vector2(i, half - height),
				Vector2(i, half + height),
				colors[channel],
				1.0,
				false
			)
			#x += 1
			#xpos = MathUtil.change_range(x, 0, len(audiobuf[channel]), 0, self.size.x)
			

func _on_gui_input(event):
	pass
	#if event is InputEventMouseButton:
		#if event.button_index == MOUSE_BUTTON_LEFT and event.pressed:
			#sound.bus = &"Master"
			#sound.stream = AudioStreamWAV.new()
			#sound.stream.data = audiobuf
			#sound.stream.format = 1
			#sound.play()
			#pass
