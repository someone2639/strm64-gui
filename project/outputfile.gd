extends "res://UI_Entries/fileselect/fileselect.gd"


# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("filename").hide()
	get_node("pickfile").hide()
	get_node("l2").hide()
	get_node("soundname").hide()
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func _on_onoff_toggled(toggled_on):
	if toggled_on:
		get_node("filename").show()
		get_node("pickfile").show()
		get_node("l2").show()
		get_node("soundname").show()
	else:
		get_node("filename").hide()
		get_node("pickfile").hide()
		get_node("l2").hide()
		get_node("soundname").hide()

func _on_diag_dir_selected(dir):
	self.path = dir + "/" + get_node("soundname").text
	get_node("filename").text = dir
	print(self.path)

func _on_soundname_text_changed(new_text):
	self.path = get_node("filename").text + "/" + new_text
