require("const")

update = function(delta_time)
    p = get_player_pos()
    print("Delta time = "..delta_time)
    glfw_poll_events()
    if get_key(keys["l"]) == 1 then
        place_block_look()
    end
    if get_key(keys["f"]) == 1 then
        set_torch_fuel(120)
    end
end