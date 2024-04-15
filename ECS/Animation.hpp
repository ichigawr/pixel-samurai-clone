#pragma once


struct Animation {
    int index = 0;
    int frames;
    int speed;
    int frameWidth;
    int frameHeight;
    int marginBottom;
    int marginLeft;
    int anchor;

    Animation() {
        frames = 0;
        speed = 100;
        frameWidth = 48;
        frameHeight = 48;
        marginBottom = 0;
        marginLeft = 0;
        anchor = 0;
    }

    Animation(int f, int s, int w, int h, int mB, int mL) {
        frames = f;
        speed = s;
        frameWidth = w;
        frameHeight = h;
        marginBottom = mB;
        marginLeft = mL;
        anchor = frameWidth / 2 + frameWidth % 2;
    }

    Animation(int f, int s, int w, int h, int mB, int mL, int a) {
        frames = f;
        speed = s;
        frameWidth = w;
        frameHeight = h;
        marginBottom = mB;
        marginLeft = mL;
        anchor = a;
    }
};
