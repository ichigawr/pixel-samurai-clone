#include "Menu.hpp"


void Menu::addText(int xpos, int ypos, std::string text, std::string fontFamily) {
    Entity& label(manager.addEntity());

    SDL_Color white = {255, 255, 255, 255};
    label.addComponent<UILabel>(xpos, ypos, text, fontFamily, white);

    if (xpos == -1) {
        int w = label.getComponent<UILabel>().getPosition().w;
        xpos = WINDOW_WIDTH / 2 - w / 2;
    }

    if (ypos == -1) {
        int h = label.getComponent<UILabel>().getPosition().h;
        ypos = WINDOW_HEIGHT / 2 - h / 2;
    }

    label.getComponent<UILabel>().setPosition(xpos, ypos);
    label.addGroup(Game::groupLabels);
}


void Menu::firstScene() {
    labels[0]->draw();
}


void Menu::secondScene() {
    player.draw();
    
    for (int i = 1; i < labels.size(); i++)
        labels[i]->draw();
}
