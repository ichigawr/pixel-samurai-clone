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

        int speed = 7;

        TransformComponent() {
            position.Zero();
        }

        TransformComponent(int sc) {
            position.x = DEFAULT_X_POSITION;
            position.y = DEFAULT_Y_POSITION;
            scale = sc;
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

        TransformComponent(float x, float y, int w, int h, int s) {
            position.x = x;
            position.y = y;
            width = w;
            height = h;
            scale = s;
        }

        void init() override {
            velocity.Zero();
        }

        void update() override {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
};
