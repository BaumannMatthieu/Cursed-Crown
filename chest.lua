character = {
        animation = {
        name = "Chest",
        image = "data/animations/misci/chest.png",
        animation_time = 1000,
        width_sprite = 64,
        height_sprite = 80,

        width_texture = 576,
        height_texture = 240,

        origin_x = 32.0,
        origin_y = 60.0,
        
        tex_coords = {
            {
                name = "stance",
                repeated = false,
                data = {
                    {
                        dir = 3,
                        frames = 9,
                        first = 0
                    }
                }
            }
        }
    },
    interaction = {
        image_clic = "data/animations/misci/chest_clic.png",
        handler = function(chest, script)
            print("start interaction")
            
            playAnimation(chest, "stance", 3);

            print("end interaction")
        end
    }
}
