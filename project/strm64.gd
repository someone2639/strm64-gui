extends Node


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	GStrm64.handle.set_input_file("anime")
	GStrm64.handle.set_output_file("games")
	GStrm64.handle.run()
	#var t = GDExtensionTemplate.new()
	#s.filename = "test"
	#s.add(10)
	#s.add(20)
	#s.add(30)
	#print(s.get_total())
	#s.reset()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
