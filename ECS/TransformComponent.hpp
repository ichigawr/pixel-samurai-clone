#pragma once

#include "Components.hpp"
#include "../Vector2D.hpp"


class TransformComponent : public Component {
    public:
        Vector2D position;
        Vector2D velocity;

        int height = 64;
        int width = 64;
        int scale = 1;

        int speed = 7;

        TransformComponent() {
            position.Zero();
        }

        TransformComponent(int sc) {
            position.x = 427;
            position.y = 240;
            scale = sc;
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
