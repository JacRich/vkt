require("const")
lp = get_player_pos()
delay = 0
update = function(delta_time)
    p = get_player_pos()
	-- check if player has moved and print new pos
	delay = delay + delta_time
	
	if delay>1 and (p.x ~= lp.x or p.y ~= lp.y or p.z ~= lp.z) then
		print("x = "..p.x.." y = "..p.y.." z = "..p.z)
		delay = 0
	end
    glfw_poll_events()
    if get_key(keys["l"]) == 1 then
        destroy_block_look()
    end
    if get_key(keys["f"]) == 1 then
        set_torch_fuel(120)
    end
	lp = p
end