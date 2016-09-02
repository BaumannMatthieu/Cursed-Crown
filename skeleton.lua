local good, chaotic_good, neutral, chaotic_bad, bad = 0, 1, 2, 3, 4

character = {
    description = {
        name = "Skeleton Warrior",
        
        life = 100.0,
        mana = 0.0,
        
        defense = 3.0,
        touch = 18.0,
        esquive = 0.0,
        
        displacement_speed = 4.0,
        max_weight = 20.0,
        gold = math.random(10),

        -- alignment is a way to caracterize if the 
        -- character could attack you or if he is a friend.
        alignment = bad,
        -- force, discretion, spirit, charisma, chance, volonte
        caract = {5, 3, 0, 1, 2, 0}
    },
    animation = {
        name = "Skeleton animation",
        image = "data/animations/characters/skeleton_0.png",
        animation_time = 650,
        width_sprite = 128,
        height_sprite = 128,

        width_texture = 4096,
        height_texture = 1024,

        origin_x = 64.0,
        origin_y = 100.0,
        tex_coords = { 
            {
                name = "attack",
                repeated = true,
                data = {
                    {
                        dir = 5,
                        frames = 8,
                        first = 12
                    },
                    {
                        dir = 6,
                        frames = 8,
                        first = 44
                    },
                    {
                        dir = 7,
                        frames = 8,
                        first = 76
                    },
                    {
                        dir = 0,
                        frames = 8,
                        first = 108
                    },
                    {
                        dir = 1,
                        frames = 8,
                        first = 140
                    },
                    {
                        dir = 2,
                        frames = 8,
                        first = 172
                    },
                    {
                        dir = 3,
                        frames = 8,
                        first = 204
                    },
                    {
                        dir = 4,
                        frames = 8,
                        first = 236
                    }
                }
            },
            {
                name = "stance",
                repeated = true,
                data = {
                    {
                        dir = 5,
                        frames = 4,
                        first = 0
                    },
                    {
                        dir = 6,
                        frames = 4,
                        first = 32
                    },
                    {
                        dir = 7,
                        frames = 4,
                        first = 64
                    },
                    {
                        dir = 0,
                        frames = 4,
                        first = 96
                    },
                    {
                        dir = 1,
                        frames = 4,
                        first = 128
                    },
                    {
                        dir = 2,
                        frames = 4,
                        first = 160
                    },
                    {
                        dir = 3,
                        frames = 4,
                        first = 192
                    },
                    {
                        dir = 4,
                        frames = 4,
                        first = 224
                    }
                }
            },
            {
                name = "run",
                repeated = true,
                data = {
                    {
                        dir = 5,
                        frames = 8,
                        first = 4
                    },
                    {
                        dir = 6,
                        frames = 8,
                        first = 36
                    },
                    {
                        dir = 7,
                        frames = 8,
                        first = 68
                    },
                    {
                        dir = 0,
                        frames = 8,
                        first = 100
                    },
                    {
                        dir = 1,
                        frames = 8,
                        first = 132
                    },
                    {
                        dir = 2,
                        frames = 8,
                        first = 164
                    },
                    {
                        dir = 3,
                        frames = 8,
                        first = 196
                    },
                    {
                        dir = 4,
                        frames = 8,
                        first = 228
                    }
                }
            },
            {
                name = "death",
                repeated = false, 
                data = {
                    {
                        dir = 5,
                        frames = 6,
                        first = 22
                    },
                    {
                        dir = 6,
                        frames = 6,
                        first = 54
                    },
                    {
                        dir = 7,
                        frames = 6,
                        first = 86
                    },
                    {
                        dir = 0,
                        frames = 6,
                        first = 118
                    },
                    {
                        dir = 1,
                        frames = 6,
                        first = 150
                    },
                    {
                        dir = 2,
                        frames = 6,
                        first = 182
                    },
                    {
                        dir = 3,
                        frames = 6,
                        first = 214
                    },
                    {
                        dir = 4,
                        frames = 6,
                        first = 246
                    }
                }
            }
        } 
    }
}
