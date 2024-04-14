#pragma once


struct Animation {
    int index;
    int frames;
    int speed;
    int frameWidth;
    int frameHeight;
    int padding;
    int anchor;

    Animation() {
        index = 0;
        frames = 0;
        speed = 100;
        frameWidth = 48;
        frameHeight = 48;
    }

    Animation(int f, int s, int w, int h, int p, int a) {
        frames = f;
        speed = s;
        frameWidth = w;
        frameHeight = h;
        padding = p;
        anchor = a;
    }
};
