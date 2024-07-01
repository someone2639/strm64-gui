extends HBoxContainer

@export var label : String

func _on_pickfile_pressed():
	get_node("diag").show()

func _on_diag_file_selected(path):
	get_node("filename").text = path

func _on_epiclabel_tree_entered():
	get_node("epiclabel").text = label
