extends Node

@export var optname : String
@export var state : bool
@export var value : String

func _on_option_name_tree_entered():
	get_node("option_name").text = optname

func _on_option_text_tree_entered():
	var toEdit = get_node("buttons/option_text") as LineEdit
	toEdit.text = value
	toEdit.editable = state

func _on_option_on_tree_entered():
	get_node("buttons/option_on").button_pressed = state

func _on_option_on_toggled(toggled_on):
	var toEdit = get_node("buttons/option_text") as LineEdit
	toEdit.editable = toggled_on

