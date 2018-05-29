#ifndef SCENE_H
#define SCENE_H

#include <QDialog>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <time.h>
#include <fstream>
#include "user.h"
#include "planet.h"
#include "level.h"
#include "velocity.h"
#include "targetbarrier.h"
#include "taco.h"

#define LEVEL_NAME "Level_doc.txt"

namespace Ui
{
    class Scene;
}

class Scene : public QDialog
{
    Q_OBJECT

    public:

        explicit Scene(QWidget *parent = 0);

        ~Scene();

        User *getPlayer_1() const;

        void setPlayer_1(User *value);

        User *getPlayer_2() const;

        void setPlayer_2(User *value);

        void set_tacos();

        void set_planets();

        void set_barriers();

        void set_targets();

        void read_level();

        void move_list();

        void delete_planet();

    signals:

        void open(bool is_multi);

    public slots:

        void rec_open(bool is_multi);

    private slots:

        void on_btn_start_clicked();

        void hor_move();

        void ver_move();

        void mov_planet();

    private:

        int id_scene;

        bool is_multiplayer;

        bool is_start; // Inicio el juego o lo pausó.

        bool next; // Siguiente objeto.

        bool limit; // Llego al limite.

        bool is; // Entró al scene.

        int current_level;

        QVector<int> score; // Puntos.

        User *player_1;

        User *player_2;

        QGraphicsLineItem *l1;

        QGraphicsLineItem *l2;

        QGraphicsLineItem *l3;

        QGraphicsLineItem *l4;

        QVector<Taco *> tacos;

        QVector<Planet *> planets;

        QVector<TargetBarrier *> tarbar;

        QVector<TargetBarrier *> targets;

        QVector<Level *> levels;

        QGraphicsScene *gc;

        QTimer *tim_hor;

        QTimer *tim_ver;

        QTimer *tim_cue;

        Velocity vel;

        Ui::Scene *ui;
};

#endif // SCENE_H
