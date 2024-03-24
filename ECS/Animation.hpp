#pragma once


struct Animation {
    int index;
    int frames;
    int speed;

    Animation() {
        index = 0;
        frames = 0;
        speed = 100;
    }

    Animation(int i, int f, int s) {
        index = i;
        frames = f;
        speed = s;
    }
};
