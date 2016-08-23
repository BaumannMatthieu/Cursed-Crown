character = {
    animation = {
        name = "Chest",
        image = "data/animations/misci/chest.png",
        animation_time = 1000,
        width_sprite = 64,
        height_sprite = 80,

        repeated = false,

        width_texture = 576,
        height_texture = 240,

        origin_x = 32.0,
        origin_y = 60.0,
        
        data = {
            {
                name = "stance",
                dir = 3,
                frames = 9,
                first = 0
            },
            {
                name = "open_right",
                dir = 3,
                frames = 9,
                first = 9
            },
            {
                name = "open_left",
                dir = 3,
                frames = 9,
                first = 18
            }
        }
    },
    interaction = {
        image_clic = "data/animations/misci/chest_clic.png",
        handler = function(chest, script)
            print("start interaction")
            
            script:playAnimation(chest, "stance", 3);

            print("end interaction")
        end
    }
}
