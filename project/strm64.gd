extends Node


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var s = STRM64.new()
	s.run()
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
