#pragma once

#include <memory>
#include <vector>

#include "../Core/Entity.h"

#include "../Core/MovementComponent.h"
#include "../Core/PositionComponent.h"
#include "../Core/AnimatedComponent.h"
#include "../Core/CaracteristicsComponent.h"

#include "../Core/Log.h"
#include "../Core/System.h"

#include "../Core/Maths.h"

class Movement {
    public:
    void run(std::vector<EntityPtr>& entitys) {
        for(auto entity : entitys) {
            std::vector<ComponentPtr> components;
            if(System<Movement>::findComponent<MovementComponent,
                                               PositionComponent,
                                               SpriteComponent, 
                                               AnimatedComponent,
                                               CaracteristicsComponent>(entity, components)) {
                MovementComponentPtr movement = std::static_pointer_cast<MovementComponent>(components[0]);
                PositionComponentPtr position = std::static_pointer_cast<PositionComponent>(components[1]);
                SpriteComponentPtr sprite = std::static_pointer_cast<SpriteComponent>(components[2]);
                AnimatedComponentPtr animation = std::static_pointer_cast<AnimatedComponent>(components[3]);
                CaracteristicsComponentPtr caract = std::static_pointer_cast<CaracteristicsComponent>(components[4]);

                if(norme(movement->m_goal - position->m_position) <= (1 << 3)) {
                    animation->m_animation_key = std::pair<std::string, MovementComponent::Direction>("stance", movement->m_dir2);
                    //animation->m_animation_key = getStanceAnimationId(movement->m_dir2);

                    entity->deleteComponent<MovementComponent>();
                } else {
                    if(norme(movement->m_d1) == 0 && norme(movement->m_d2) == 0) {
                        sf::Vector2f direction(movement->m_goal - position->m_position);
                        changeDirectionAnimation(animation, movement, direction);
                    }

                    sf::Vector2f direction;
 
                    if(norme(movement->m_d1) <= (1 << 3)) {
                        direction = caract->m_displacement_speed*movement->m_d2/norme(movement->m_d2);
                        movement->m_d2 = movement->m_d2 - direction; 

                        //animation->m_animation_key = getRunAnimationId(movement->m_dir2);
                        animation->m_animation_key = std::pair<std::string, MovementComponent::Direction>("run", movement->m_dir2);
                    } else {
                        direction = caract->m_displacement_speed*movement->m_d1/norme(movement->m_d1);
                        movement->m_d1 = movement->m_d1 - direction; 
                        
                        //animation->m_animation_key = getRunAnimationId(movement->m_dir1);
                        animation->m_animation_key = std::pair<std::string, MovementComponent::Direction>("run", movement->m_dir1);
                    }

                    position->m_position += direction;

                    AnimationPtr current_anim = animation->m_animations[animation->m_animation_key];

                    moveAnimation(animation, cart(direction));

                    if(System<Movement>::findComponent<PlayerComponent>(entity, components)) {
                        PlayerComponentPtr player = std::static_pointer_cast<PlayerComponent>(components[5]);

                        player->m_view.move(cart(direction));
                        player->m_window->setView(player->m_view);
                    }
                }

                unsigned int id_frame = animation->m_animations[animation->m_animation_key]->m_frame_id;
                //std::cout << "id_frame " << id_frame << std::endl;
                sprite->m_vertex_array = &(animation->m_animations[animation->m_animation_key]->m_vertex_arrays[id_frame]);
            } 
        }
    }

    static void moveAnimation(AnimatedComponentPtr animation, const sf::Vector2f& delta_position) {
        for(std::map<std::pair<std::string, MovementComponent::Direction>, AnimationPtr>::iterator it = animation->m_animations.begin(); it != animation->m_animations.end(); ++it) {
            AnimationPtr current_anim = it->second;
            for(unsigned int i = 0; i < current_anim->m_vertex_arrays.size(); ++i) {
     
                current_anim->m_vertex_arrays[i][0].position += delta_position;
                current_anim->m_vertex_arrays[i][1].position += delta_position;
                current_anim->m_vertex_arrays[i][2].position += delta_position;
                current_anim->m_vertex_arrays[i][3].position += delta_position;
            }
        }
    }

    static void changeDirectionAnimation(AnimatedComponentPtr anim, MovementComponentPtr movement, sf::Vector2f& direction) {
        float angle = getAngle(direction, sf::Vector2f(1.f, 0.f));
        float d = norme(direction);

        float alpha, beta;

        MovementComponent::Direction dir_1, dir_2;

        if(angle <= PI/4) {
            alpha = PI/4.f - angle;

            dir_1 = MovementComponent::Direction::NE;
            dir_2 = MovementComponent::Direction::E;
        }

        if(angle <= PI/2 && angle > PI/4) {
            alpha = PI/2.f - angle;

            dir_1 = MovementComponent::Direction::N;
            dir_2 = MovementComponent::Direction::NE;
        }

        if(angle <= 3*PI/4 && angle > PI/2) {
            alpha = 3*PI/4.f - angle;

            dir_1 = MovementComponent::Direction::NW;
            dir_2 = MovementComponent::Direction::N;
        }

        if(angle <= PI && angle > 3*PI/4.f) {
            alpha = PI - angle;

            dir_1 = MovementComponent::Direction::W;
            dir_2 = MovementComponent::Direction::NW;
        }

        if(angle <= 5*PI/4 && angle > PI) {
            alpha = 5*PI/4.f - angle;

            dir_1 = MovementComponent::Direction::SW;
            dir_2 = MovementComponent::Direction::W;
        }

        if(angle <= 3*PI/2 && angle > 5*PI/4) {
            alpha = 3*PI/2.f - angle;

            dir_1 = MovementComponent::Direction::S;
            dir_2 = MovementComponent::Direction::SW;
        }

        if(angle <= 7*PI/4 && angle > 3*PI/2) {
            alpha = 7*PI/4.f - angle;

            dir_1 = MovementComponent::Direction::SE;
            dir_2 = MovementComponent::Direction::S;
        }

        if(angle > 7*PI/4) {
            alpha = 2*PI - angle;

            dir_1 = MovementComponent::Direction::E;
            dir_2 = MovementComponent::Direction::SE;
        }

        beta = PI - alpha - 3.f*PI/4.f;

        float d1 = std::sin(beta)*std::sqrt(2.f)*d;
        float d2 = std::sin(alpha)*std::sqrt(2.f)*d;

        movement->m_d1 = getDirectionVector(dir_1)*d1;
        movement->m_d2 = getDirectionVector(dir_2)*d2;

        movement->m_dir1 = dir_1;
        movement->m_dir2 = dir_2;
    }

    static MovementComponent::Direction getDirection(const sf::Vector2f& vec) {
        float angle = getAngle(vec, sf::Vector2f(1.f, 0.f));
        if(angle <= PI/8.f||angle > 15*PI/8.f) {
            return MovementComponent::Direction::E;
        }

        if(angle <= 3*PI/8.f) {
            return MovementComponent::Direction::NE;
        }

        if(angle <= 5*PI/8.f) {
            return MovementComponent::Direction::N;
        }

        if(angle <= 7*PI/8.f) {
            return MovementComponent::Direction::NW;
        }

        if(angle <= 9*PI/8.f) {
            return MovementComponent::Direction::W;
        }

        if(angle <= 11*PI/8.f) {
            return MovementComponent::Direction::SW;
        }

        if(angle <= 13*PI/8.f) {
            return MovementComponent::Direction::S;
        }

        return MovementComponent::Direction::SE;
    }

    static sf::Vector2f getDirectionVector(MovementComponent::Direction direction) {
        sf::Vector2f vec;

        switch(direction) {
            case MovementComponent::Direction::N:
                    vec = sf::Vector2f(0.f, -1.f);
                break;
            case MovementComponent::Direction::NW:
                    vec = sf::Vector2f(-1.f, -1.f);
                break;
            case MovementComponent::Direction::W:
                    vec = sf::Vector2f(-1.f, 0.f);
                break;
            case MovementComponent::Direction::SW:
                    vec = sf::Vector2f(-1.f, 1.f);
                break;
            case MovementComponent::Direction::S:
                    vec = sf::Vector2f(0.f, 1.f);
                break;
            case MovementComponent::Direction::SE:
                    vec = sf::Vector2f(1.f, 1.f);
                break;
            case MovementComponent::Direction::E:
                    vec = sf::Vector2f(1.f, 0.f);
                break;
            case MovementComponent::Direction::NE:
                    vec = sf::Vector2f(1.f, -1.f);
                break;
            default:
                break;
        }

        normalize(vec);
        return vec;
    }
/*
    static std::string getRunAnimationId(MovementComponent::Direction direction) {
        std::string id;

        switch(direction) {
            case MovementComponent::Direction::N:
                    id = "run_ne";
                break;
            case MovementComponent::Direction::NW:
                    id = "run_n";
                break;
            case MovementComponent::Direction::W:
                    id = "run_nw";
                break;
            case MovementComponent::Direction::SW:
                    id = "run_w";
                break;
            case MovementComponent::Direction::S:
                    id = "run_sw";
                break;
            case MovementComponent::Direction::SE:
                    id = "run_s";
                break;
            case MovementComponent::Direction::E:
                    id = "run_se";
                break;
            case MovementComponent::Direction::NE:
                    id = "run_e";
                break;
            default:
                break;
        }

        return id;
    }

    static std::string getStanceAnimationId(MovementComponent::Direction direction) {
        std::string id;

        switch(direction) {
            case MovementComponent::Direction::N:
                    id = "stance_ne";
                break;
            case MovementComponent::Direction::NW:
                    id = "stance_n";
                break;
            case MovementComponent::Direction::W:
                    id = "stance_nw";
                break;
            case MovementComponent::Direction::SW:
                    id = "stance_w";
                break;
            case MovementComponent::Direction::S:
                    id = "stance_sw";
                break;
            case MovementComponent::Direction::SE:
                    id = "stance_s";
                break;
            case MovementComponent::Direction::E:
                    id = "start";
                break;
            case MovementComponent::Direction::NE:
                    id = "stance_e";
                break;
            default:
                break;
        }

        return id;
    }*/
};

