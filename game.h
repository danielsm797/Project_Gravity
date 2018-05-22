#ifndef GAME_H
#define GAME_H


class Game
{
    private:

        bool is_multiplayer;

    public:

        Game();

        bool getIs_multiplayer() const;

        void setIs_multiplayer(bool value);
};

#endif // GAME_H
