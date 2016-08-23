local good, chaotic_good, neutral, chaotic_bad, bad = 0, 1, 2, 3, 4

character = {
    description = {
        name = "Orc Heavy",
        
        life = 100.0,
        mana = 0.0,
        
        defense = 3.0,
        touch = 18.0,
        esquive = 0.0,
        
        displacement_speed = 2.5,
        max_weight = 20.0,
        gold = math.random(10),

        -- alignment is a way to caracterize if the 
        -- character could attack you or if he is a friend.
        alignment = bad,
        -- force, discretion, spirit, charisma, chance, volonte
        caract = {5, 3, 0, 1, 2, 0}
    },
    animation = {
        name = "Orc Heavy",
        image = "data/animations/characters/orc_heavy_1.png",
        animation_time = 650,
        width_sprite = 128,
        height_sprite = 128,

        repeated = true, 

        width_texture = 4096,
        height_texture = 1024,

        origin_x = 64.0,
        origin_y = 100.0,
        data = {
            {
                name = "stance",
                dir = 5,
                frames = 4,
                first = 0
            },
            {
                name = "stance",
                dir = 6,
                frames = 4,
                first = 32
            },
            {
                name = "stance",
                dir = 7,
                frames = 4,
                first = 64
            },
            {
                name = "stance",
                dir = 0,
                frames = 4,
                first = 96
            },
            {
                name = "stance",
                dir = 1,
                frames = 4,
                first = 128
            },
            {
                name = "stance",
                dir = 2,
                frames = 4,
                first = 160
            },
            {
                name = "stance",
                dir = 3,
                frames = 4,
                first = 192
            },
            {
                name = "stance",
                dir = 4,
                frames = 4,
                first = 224
            },
            {
                name = "run",
                dir = 5,
                frames = 8,
                first = 4
            },
            {
                name = "run",
                dir = 6,
                frames = 8,
                first = 36
            },
            {
                name = "run",
                dir = 7,
                frames = 8,
                first = 68
            },
            {
                name = "run",
                dir = 0,
                frames = 8,
                first = 100
            },
            {
                name = "run",
                dir = 1,
                frames = 8,
                first = 132
            },
            {
                name = "run",
                dir = 2,
                frames = 8,
                first = 164
            },
            {
                name = "run",
                dir = 3,
                frames = 8,
                first = 196
            },
            {
                name = "run",
                dir = 4,
                frames = 8,
                first = 228
            }
        }
    }, 
    attack = {
        image_clic = "data/animations/characters/orc_heavy_1_clic.png",
        attacking = function(orc, script)
            print("start attack")
            
            --script:playAnimation("start");
            script:deplace(orc, Position(100.0, 100.0))
            wait(4000)
            script:deplace(orc, Position(100.0, 200.0))
            wait(4000)
            script:deplace(orc, Position(200.0, 200.0))
            wait(4000)
            script:deplace(orc, Position(200.0, 100.0))
            wait(4000)

            print("end attack")
        end
    }
}
