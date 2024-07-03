extends Entry

@export var optname : String
@export var state : bool

func _on_option_name_tree_entered():
	get_node("option_name").text = optname


func _on_option_state_toggled(toggled_on):
	state = toggled_on
