extends HBoxContainer

@export var label : String
@export var path : String

func _on_pickfile_pressed():
	get_node("diag").show()

func _on_diag_file_selected(p):
	path = p
	get_node("filename").text = path
	get_node("../audio_prev").set_file(p)

func _on_epiclabel_tree_entered():
	get_node("epiclabel").text = label

func export() -> String:
	return label
