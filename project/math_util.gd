extends Node

# funny stack overflow
func change_range(OldValue, OldMin, OldMax, NewMin, NewMax):
	var OldRange = (OldMax - OldMin)  
	var NewRange = (NewMax - NewMin)  
	return (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

func interp_range(x, y, n):
	var step = (y - x) / (n - 1)
	var ret = Array()
	for i in range(n):
		ret.append(x + i*step)
	return ret
