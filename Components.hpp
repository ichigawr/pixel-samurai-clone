#pragma once

#include "ECS.hpp"


class PositionComponent : public Component {
    private:
        int xpos;
        int ypos;

    public:
        int x() {
            return xpos;
        }

        int y() {
            return ypos;
        }

        void init() override {
            xpos = 0;
            ypos = 0;
        }

        void update() override {
            xpos++;
            ypos++;

            std::cout << xpos << ", " << ypos << std::endl;
        }

        void setPos(int x, int y) {
            xpos = x;
            ypos = y;
        }
};