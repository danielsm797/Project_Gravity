#ifndef SCENE_H
#define SCENE_H

#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QMediaPlayer>
#include <fstream>
#include <time.h>
#include <QToolTip>
#include <QFontDatabase>
#include <qmediaplaylist.h>
#include "user.h"
#include "planet.h"
#include "level.h"
#include "velocity.h"
#include "targetbarrier.h"
#include "detail.h"
#include "taco.h"

#define LEVEL_NAME "../Project_Gravity/Level_doc.txt"
#define GAME_NAME "../Project_Gravity/Game_doc.txt"

namespace Ui{
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

    QString remove_game();

    void set_tacos();

    void set_planets();

    void set_barriers(QVector<QString> dta_);

    void set_powers(QVector<QString> dta_, bool fl);

    void read_level();

    void move_list();

    void set_score();

    void delete_planet();

    void set_targets(QVector<QString> dta_);

    void clear_scene();

    void load_game();

    bool write_file(QString dta);

    void get_lastId();

    int getId_scene() const;

    void setId_scene(int value);

    int targets_count();

    float aleatorio(float LO, float HI);

    void is_multi(int id_s);

    void next_level();

signals:

    void open(bool is_multi);

public slots:

    void rec_open(bool is_multi);

private slots:

    void on_btn_start_clicked();

    void hor_move();

    void ver_move();

    void pow_move();

    void mov_planet();

    void rot_ele();

    void on_btn_save_clicked();

    void on_btn_load_clicked();

private:

    int id_scene;

    bool is_multiplayer;

    bool is_second; // Juega el segundo.

    bool is_start; // Inicio el juego o lo pausó.

    bool next; // Siguiente objeto.

    bool limit; // Llego al limite.

    bool is; // Entró al scene.

    int current_level; // Id del nivel actual.

    QVector<int> ids_game; // Id de los juegos.

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

    QVector<Level *> levels;

    QVector<QMediaPlayer *> sounds;

    QMediaPlayer *sou_back;

    QGraphicsScene *gc;

    QTimer *tim_poder;

    QTimer *tim_hor;

    QTimer *tim_ver;

    QTimer *tim_cue;

    QTimer *tim_rot;

    Velocity vel;

    Detail det;

    Ui::Scene *ui;
};

#endif // SCENE_H
