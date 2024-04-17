#pragma once

#include "Components.hpp"
#include "../Vector2D.hpp"


static const float DEFAULT_X_POSITION = 1288;
static const float DEFAULT_Y_POSITION = 432;


class TransformComponent : public Component {
    public:
        Vector2D position;
        Vector2D velocity;

        int height = 48;
        int width = 48;
        int scale = 1;

        float speed = 4;

        TransformComponent() {
            position.Zero();
        }

        TransformComponent(int sc, int sp) {
            position.x = DEFAULT_X_POSITION;
            position.y = DEFAULT_Y_POSITION;
            scale = sc;
            speed = sp;
        }
    
        TransformComponent(float x, float y) {
            position.Zero();
        }

        TransformComponent(float x, float y, int sc, float sp) {
            position.x = x;
            position.y = y;
            scale = sc;
            speed = sp;
        }

        void init() override {
            velocity.Zero();
        }

        void update() override {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
};
