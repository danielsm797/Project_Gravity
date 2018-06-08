#include "scene.h"
#include "ui_scene.h"

int angulo = 0;

Scene::Scene(QWidget *parent) : QDialog(parent), ui(new Ui::Scene)
{
    ui->setupUi(this);

    srand (time(NULL));
    player_1 = NULL;
    player_2 = NULL;

    score.append(0);
    score.append(0);

    id_scene = 0;

    is_start = false;
    limit = false;
    next = true;
    is = false;
    is_second = false;

    current_level = 0;

    connect(this, SIGNAL(open(bool)), this, SLOT(rec_open(bool)));

    gc = new QGraphicsScene(this);
    ui->graphicsView->setScene(gc);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    gc->setSceneRect(0, 0, ui->graphicsView->width() - 8, ui->graphicsView->height() - 8);

    // Agregamos los sonidos.
    sounds.append(new QMediaPlayer());
    sounds.last()->setMedia(QUrl("qrc:/Sounds/tac.mp3"));

    sounds.append(new QMediaPlayer());
    sounds.last()->setMedia(QUrl("qrc:/Sounds/tac.mp3"));

    sounds.append(new QMediaPlayer());
    sounds.last()->setMedia(QUrl("qrc:/Sounds/abs.mp3"));

    sounds.append(new QMediaPlayer());
    sounds.last()->setMedia(QUrl("qrc:/Sounds/par.mp3"));

    QPen myPen = QPen(Qt::white);
    myPen.setStyle(Qt::SolidLine);

    QLineF topLine(gc->sceneRect().topLeft(), gc->sceneRect().topRight());

    QLineF leftLine(gc->sceneRect().topLeft(), gc->sceneRect().bottomLeft());

    QLineF rightLine(gc->sceneRect().topRight(), gc->sceneRect().bottomRight());

    QLineF bottomLine(gc->sceneRect().bottomLeft(), gc->sceneRect().bottomRight());

    QLineF separator(QPointF(gc->sceneRect().bottomRight().x() - 50, gc->sceneRect().topRight().y()), QPointF(gc->sceneRect().bottomRight().x() - 50, gc->sceneRect().bottomRight().y()));

    QLineF separator_(QPointF(gc->sceneRect().bottomLeft().x(), gc->sceneRect().bottomLeft().y() - 50), QPointF(gc->sceneRect().bottomRight().x() - 50, gc->sceneRect().bottomRight().y() - 50));

    // Agregamos las lineas.
    gc->addLine(rightLine, myPen);
    gc->addLine(bottomLine, myPen);
    gc->addLine(leftLine, myPen);
    gc->addLine(topLine, myPen);
    gc->addLine(separator, myPen);
    gc->addLine(separator_, myPen);

    set_tacos();
    read_level();

    set_planets();
    set_barriers(levels.at(current_level)->getBarrier());
    set_targets(levels.at(current_level)->getTargets());
    set_powers(levels.at(current_level)->getPowers(), true);

    tim_hor = new QTimer();
    connect(tim_hor, SIGNAL(timeout()), this, SLOT(hor_move()));

    tim_ver = new QTimer();
    connect(tim_ver, SIGNAL(timeout()), this, SLOT(ver_move()));

    tim_cue = new QTimer();
    connect(tim_cue, SIGNAL(timeout()), this, SLOT(mov_planet()));

    tim_poder = new QTimer();
    connect(tim_poder, SIGNAL(timeout()), this, SLOT(pow_move()));
    tim_poder->start(70);

    tim_rot = new QTimer();
    connect(tim_rot, SIGNAL(timeout()), this, SLOT(rot_ele()));
    tim_rot->start(200);

    vel.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    vel.setAttribute(Qt::WA_TranslucentBackground);

    det.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // Sonido de fondo.
    QMediaPlaylist * playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/Sounds/bak.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    sou_back = new QMediaPlayer();
    sou_back->setPlaylist(playlist);
}

Scene::~Scene()
{
    delete ui;

    delete player_1;

    delete player_2;

    delete tim_hor;

    delete tim_ver;

    delete tim_poder;

    delete tim_rot;

    delete sou_back;

    delete l1;

    delete l2;

    delete l3;

    delete l4;
}

void Scene::rec_open(bool is_multi)
{
    sou_back->setVolume(40);
    sou_back->play();

    is_multiplayer = is_multi;

    // Configuramos los jugadores en la pantalla.
    ui->lbl_namePlayer1->setText(player_1->getUser_name());

    ui->lbl_score1->setText(QString::number(score.first()));

    if (is_multiplayer)
    {
        ui->lbl_namePlayer2->setText(player_2->getUser_name());

        ui->lbl_score2->setText(QString::number(score.last()));
    }
}

void Scene::hor_move()
{
    tacos.first()->hor_move();

    // Validamos si el objeto colisionó con el planeta.
    if (tacos.first()->collidesWithItem(planets.first()))
    {
        if (planets.first()->getVel_x() == 0)
        {
            tim_cue->start(8);

            tacos.first()->set_position();

            sounds.first()->play();
        }
        else
        {
            tim_ver->stop();

            tim_cue->stop();

            tim_hor->stop();

            tacos.first()->setPos_y(585);

            tacos.first()->set_position();

            delete_planet();
        }
    }
}

void Scene::ver_move()
{
    tacos.last()->ver_move();

    // Validamos si el objeto colisionó con el planeta.
    if (tacos.last()->collidesWithItem(planets.first()))
    {
        tim_ver->stop();
        tim_hor->stop();

        tacos.last()->setPos_x(781);
        tacos.last()->set_position();

        tacos.first()->setPos_y(585);
        tacos.first()->set_position();

        sounds.at(1)->play();

        tim_cue->start(8);
    }
}

void Scene::pow_move()
{
    QVector<TargetBarrier *>::iterator it = tarbar.begin();
    for (; it != tarbar.end(); it++)
    {
        TargetBarrier *tb = *(it);

        if (tb->getType() > 2)
        {
            tb->move_cir();
        }
    }
}

void Scene::mov_planet()
{
    if (planets.first()->getMode() == 1)
    {
        planets.first()->mov_vertical(is);
    }
    else if (planets.first()->getMode() == 2)
    {
        planets.first()->mov_circular();
    }
    else
    {
        planets.first()->mov_ondulatorio();
    }

    if (planets.first()->getPos_y() <= 74 && !limit) // Validamos si llegó al limite.
    {
        limit = true;

        tim_cue->stop();

        QTimer * tim = vel.getTimer();
        tim->start(10);
        vel.exec();

        planets.first()->setVel_x(vel.getVel_x());
        planets.first()->setVel_y(vel.getVel_y());
        vel.setVel_x(0);

        tim_ver->start(50);
    }

    if (planets.first()->getPos_x() <= 700)
    {
        is = true;
    }

    if (is)
    {
        // validamos si colisionó con un obstaculo.
        QVector<TargetBarrier *>::iterator it;
        it = tarbar.begin();

        for (; it != tarbar.end(); it++)
        {
            TargetBarrier *tg = *(it);

            if (tg->collidesWithItem(planets.first()))
            {
                // Restamos o sumamos los puntos.
                if (tg->getType() == 2)
                {
                    if ((tg->getPos_x() - planets.first()->getPos_x()) < 20 && (tg->getPos_y() - planets.first()->getPos_y()) < 20)
                    {
                        tg->play_sound();

                        if (!is_second)
                        {
                            score.first() += tg->getScore();
                        }
                        else
                        {
                            score.last() += tg->getScore();
                        }

                        gc->removeItem(tg); // Eliminamos el objetivo.

                        tarbar.removeOne(tg);
                    }
                }
                else if (tg->getType() == 1 && planets.first()->getMode() == 1)
                {
                    tg->play_sound();

                    tim_cue->stop();

                    // planets.first()->setMode(1);

                    if (!is_second)
                    {
                        score.first() -= tg->getScore();
                    }
                    else
                    {
                        score.last() -= tg->getScore();
                    }

                    // Obtenemos un número aleatorio del 1 al 4.
                    int ale = 1 + qrand() % 4;

                    if (ale == 1 || ale == 4) // Rebota hacia arriba.
                    {
                        if (ale == 1)
                        {
                            planets.first()->setPos_x(tg->getPos_x() - 22);
                        }
                        else
                        {
                            planets.first()->setPos_x(tg->getPos_x() + 22);
                        }

                        planets.first()->setVel_x(planets.first()->getVel_x() * -1);
                    }
                    else
                    {
                        if (ale == 2)
                        {
                            planets.first()->setPos_y(tg->getPos_y() - 19);
                        }
                        else
                        {
                            planets.first()->setPos_y(tg->getPos_y() + 32);
                        }

                        planets.first()->setVel_y(planets.first()->getVel_y() * -1);
                    }

                    planets.first()->set_position();
                    tim_cue->start(8);
                }
                else if (planets.first()->getMode() == 1)
                {
                    tim_cue->stop();

                    tg->play_sound();

                    tim_poder->stop();

                    if (tg->getType() == 3)
                    {
                        planets.first()->setMode(2);

                        // Obtenemos una posición aleatoria.
                        planets.first()->setOrigin_x(aleatorio(200, 500));
                        planets.first()->setOrigin_y(aleatorio(150, 300));
                    }
                    else
                    {
                        planets.first()->setMode(3);
                    }

                    gc->removeItem(tg); // Eliminamos el poder.

                    tarbar.removeOne(tg);

                    tim_poder->start(70);

                    tim_cue->start(8);
                }

                set_score();

                break;
            }
        }

        // validamos si colisiono con una pared.
        bool resp = planets.first()->border(sounds.last());

        if (resp)
        {
            tim_ver->stop();

            tim_cue->stop();

            tim_hor->stop();

            delete_planet();
        }
    }
}

void Scene::rot_ele()
{
    QVector<TargetBarrier *>::iterator it = tarbar.begin();
    for (; it != tarbar.end(); it++)
    {
        if (angulo == 360)
        {
            angulo = 0;
        }

        angulo += 1;

        TargetBarrier *tb = *(it);

        if (tb->getType() <= 2)
        {
            tb->setRotation(angulo);
            tb->setOpacity(angulo < 180 ? 1 : 0.5);
        }
    }
}

User *Scene::getPlayer_2() const
{
    return player_2;
}

void Scene::setPlayer_2(User *value)
{
    player_2 = value;
}

void Scene::set_tacos()
{
    tacos.append(new Taco(750, 585, 21, 64, ":/Img/taco.png"));

    gc->addItem(tacos.last());

    tacos.last()->set_position();

    tacos.append(new Taco(779, 80, 64, 21, ":/Img/taco_h.png"));

    gc->addItem(tacos.last());

    tacos.last()->set_position();
}

void Scene::set_planets()
{
    QVector<QString> plan = levels.at(current_level)->getPlanets();

    int x = 723;
    int y = 582;

    for (int i = 0; i < plan.length(); i++)
    {
        x -= 37;

        QStringList dta = plan.at(i).split(';');

        QString t = dta.at(0);

        QString g = dta.at(1);

        QString root = ":/Img/";
        root.append(t.append(".png"));

        planets.append(new Planet(x, y, 32, 32, root, g.toFloat(), levels.at(current_level)->getCoe_res()));
        gc->addItem(planets.last());
        planets.last()->set_position();
    }
}

void Scene::set_barriers(QVector<QString> dta_)
{
    QVector<QString> barr = dta_;

    for (int i = 0; i < barr.length(); i++)
    {
        QStringList dta = barr.at(i).split('-');

        QString x = dta.at(0);
        QString y = dta.at(1);
        QString s = dta.at(2);

        tarbar.append(new TargetBarrier(x.toInt(), y.toInt() , 24, 24, ":/Img/asteroid.png", s.toInt(), 1, 0));
        gc->addItem(tarbar.last());
        tarbar.last()->set_position();
    }
}

void Scene::set_targets(QVector<QString> dta_)
{
    QVector<QString> targets = dta_;

    for (int i = 0; i < targets.length(); i++)
    {
        QStringList dta = targets.at(i).split('-');

        QString x = dta.at(0);
        QString y = dta.at(1);
        QString s = dta.at(2);

        tarbar.append(new TargetBarrier(x.toInt(), y.toInt() , 32, 32, ":/Img/14.png", s.toInt(), 2, 0));

        s.append("</b></font>").push_front("Puntos: <font color='red'><b>");

        tarbar.last()->setToolTip(s);
        gc->addItem(tarbar.last());
        tarbar.last()->set_position();
    }
}

void Scene::set_powers(QVector<QString> dta_, bool fl)
{
    QVector<QString> powers = dta_;

    for (int i = 0; i < powers.length(); i++)
    {
        QStringList dta = powers.at(i).split('-');

        QString x = dta.at(0);
        QString y = dta.at(1);
        QString s = dta.at(2);
        QString a = "90";

        if (!fl)
        {
            a = dta.at(3);
        }

        QString root = s.toInt() == 3 ? ":/Img/mov1.png" : ":/Img/mov2.png";

        tarbar.append(new TargetBarrier(x.toInt(), y.toInt() , 24, 24, root, 0, s.toInt(), a.toFloat()));
        gc->addItem(tarbar.last());

        if (!fl)
        {
            QString px = dta.at(4);
            QString py = dta.at(5);
            tarbar.last()->setOrigen_x(px.toFloat());
            tarbar.last()->setOrigen_y(py.toFloat());
        }

        tarbar.last()->set_position();
    }
}

void Scene::clear_scene()
{
    // Limpiamos los planetas.
    int lp = planets.length();
    for (int i = 0; i < lp; i++)
    {
        gc->removeItem(planets.at(i));
    }

    planets.clear();

    // Limpiamos los obstaculos y objetivos.
    lp = tarbar.length();
    for (int i = 0; i < lp; i++)
    {
        gc->removeItem(tarbar.at(i));
    }

    tarbar.clear();

    // Ponemos los tacos en su posición inicial.
    tacos.first()->setPos_x(750);
    tacos.first()->setPos_y(585);
    tacos.first()->set_position();

    tacos.last()->setPos_x(779);
    tacos.last()->setPos_y(80);
    tacos.last()->set_position();

    // Limpiamos los puntos.
    ui->lbl_score1->setText(0);
    ui->lbl_score2->setText(0);

    // Reiniciamos las variables.
    is_start = false;
    limit = false;
    next = true;
    is = false;

    vel.setY(true);
    id_scene = 0;
}

void Scene::load_game()
{
    // Leemos el archivo.
    ifstream ifs;
    ifs.open(GAME_NAME);

    QString dta = "";
    if (ifs.is_open())
    {
        char c;
        while (ifs.get(c))
        {
            dta.append(c);
        }
    }

    ifs.close();

    // Obtenemos los datos.
    QStringList dta_list = dta.split("|");

    QStringList::iterator it = dta_list.begin();
    for (; it != dta_list.end(); it++)
    {
        QString data = *it;
        QStringList data_list = data.split("@");

        // Id de la partida y del nivel.
        QString fir = data_list.first();
        QStringList fir_list = fir.split(";");

        QString id_gm = fir_list.first();

        if (id_gm.toInt() == id_scene)
        {
            QString id_lv = fir_list.at(1); // Id del nivel.

            current_level = id_lv.toInt(); // Ponemos el id del nivel.

            // Ponemos los puntos
            ui->lbl_score1->setText(fir_list.at(4));
            ui->lbl_score2->setText(fir_list.at(5));

            // Iniciamos los timers.
            if (fir_list.at(2) == "1")
            {
                tim_hor->start(40);
            }

            if (fir_list.at(4) == "1")
            {
                tim_ver->start(50);
            }

            // Ponemos el nombre del nivel.
            QString llv = "Level ";
            llv.append(QString::number(current_level + 1));
            ui->lbl_level->setText(llv);

            // Variables.
            next     = fir_list.at(6) == "1";
            limit    = fir_list.at(7) == "1";
            is       = fir_list.at(8) == "1";
            is_start = false;
            // is_start = fir_list.at(9) == "1";

            vel.setY(fir_list.at(10) == "1");
            ui->btn_start->setText("Continue");

            // Cargamos los planetas.
            QVector<QString> dta_vector;
            QString subdta = data_list.at(1);
            QStringList info_list;

            info_list = subdta.split(",");
            QStringList::iterator f = info_list.begin();
            for (; f != info_list.end(); f++)
            {
                QString pd = *f;
                QStringList pd_list = pd.split(";");

                QString x_  = pd_list.at(0);
                QString y_  = pd_list.at(1);
                QString vx_ = pd_list.at(2);
                QString vy_ = pd_list.at(3);
                QString g_  = pd_list.at(4);
                QString ay_ = pd_list.at(5);
                QString mo_ = pd_list.at(7);
                QString an_ = pd_list.at(8);
                QString ox_ = pd_list.at(9);
                QString oy_ = pd_list.at(10);

                planets.append(new Planet(x_.toFloat(), y_.toFloat(), 32, 32, pd_list.at(6), g_.toFloat(), levels.at(current_level)->getCoe_res()));
                gc->addItem(planets.last());

                planets.last()->setMode(mo_.toInt());

                planets.last()->setAngle(an_.toFloat());

                planets.last()->setOrigin_x(ox_.toFloat());

                planets.last()->setOrigin_y(oy_.toFloat());

                planets.last()->setVel_x(vx_.toFloat());

                planets.last()->setVel_y(vy_.toFloat());

                planets.last()->setY(ay_.toFloat());

                planets.last()->set_position();
            }

            // Ponemos la posición de los tacos.
            subdta = data_list.at(2);
            int ind = 0;
            info_list = subdta.split(",");
            QStringList::iterator t = info_list.begin();
            for (; t != info_list.end(); t++)
            {
                QString d = *t;
                QStringList d_l = d.split(";");

                QString x = d_l.first();
                QString y = d_l.last();

                tacos.at(ind)->setPos_x(x.toFloat());
                tacos.at(ind)->setPos_y(y.toFloat());
                tacos.at(ind)->set_position();

                ind++;
            }

            // Ponemos los obstaculos.
            subdta = data_list.at(3);
            subdta.replace(';', '-');

            info_list = subdta.split(",");
            dta_vector.clear();
            dta_vector = info_list.toVector();
            set_barriers(dta_vector);

            // Ponemos los objetivos.
            subdta = data_list.at(4);
            subdta.replace(';', '-');

            info_list = subdta.split(",");
            dta_vector.clear();
            dta_vector = info_list.toVector();
            set_targets(dta_vector);

            // Ponemos los poderes.
            subdta = data_list.at(5);
            subdta.replace(';', '-');

            info_list = subdta.split(",");
            dta_vector.clear();
            dta_vector = info_list.toVector();
            set_powers(dta_vector, false);

            break;
        }
    }
}

void Scene::read_level()
{
    levels.clear();

    QString dataf = "";

    ifstream ifs;
    ifs.open(LEVEL_NAME);

    if (ifs.is_open())
    {
        char c;
        while (ifs.get(c))
        {
            dataf.append(c);
        }
    }

    ifs.close();

    if (!dataf.isEmpty())
    {
        QStringList level_list = dataf.split('|');

        int ind = 0;
        QStringList::iterator it = level_list.begin();
        for (; it != level_list.end(); it++)
        {
            levels.append(new Level());
            ind = levels.indexOf(levels.last());

            QString str = *it;
            QStringList dta = str.split('@');

            QString one;
            QStringList data;

            // Obtnemos el id y los coeficientes.
            one = dta.at(0);
            data = one.split(',');

            one = data.at(0);
            levels.at(ind)->setId_level(one.toInt());

            if (ind == current_level + 1)
            {
                QString level_name = "Level ";
                ui->lbl_level->setText(level_name.append(one));
            }

            one = data.at(1);
            levels.at(ind)->setCoe_res(one.toFloat());

            one = data.at(2);
            levels.at(ind)->setCoe_vis(one.toFloat());

            QVector<QString> obj;

            // Obtenemos los planetas.
            one = dta.at(1);
            data = one.split(',');

            QStringList::iterator l = data.begin();
            for(; l != data.end(); l++)
            {
                obj.append(*l);
            }

            levels.at(ind)->setPlanets(obj);


            // Obtenemos los objetivos.
            obj.clear();
            one = dta.at(2);
            data = one.split(',');

            QStringList::iterator i = data.begin();
            for(; i != data.end(); i++)
            {
                obj.append(*i);
            }

            levels.at(ind)->setTargets(obj);


            // Obtenemos los obstaculos.
            obj.clear();
            one = dta.at(3);
            data = one.split(',');

            QStringList::iterator j = data.begin();
            for(; j != data.end(); j++)
            {
                obj.append(*j);
            }

            levels.at(ind)->setBarrier(obj);


            // Obtenemos los poderes.
            obj.clear();
            one = dta.last();
            data = one.split(',');

            QStringList::iterator k = data.begin();
            for(; k != data.end(); k++)
            {
                obj.append(*k);
            }

            levels.at(ind)->setPowers(obj);
        }
    }
}

void Scene::move_list()
{
    for (int i = 1; i < planets.length(); i++)
    {
        planets.at(i)->setPos_x(planets.at(i)->getPos_x() + 37);

        planets.at(i)->set_position();
    }
}

void Scene::set_score()
{
    ui->lbl_score1->setText(QString::number(score.first()));

    ui->lbl_score2->setText(QString::number(score.last()));
}

void Scene::delete_planet()
{
    sounds.at(2)->play();

    ui->btn_start->setText("Start");

    is_start = false;

    limit = false;

    next = true;

    is = false;

    vel.setY(true);

    gc->removeItem(planets.first());

    planets.removeAt(0);

    tacos.first()->setPos_x(750);
    tacos.first()->setPos_y(585);
    tacos.first()->set_position();

    tacos.last()->setPos_x(779);
    tacos.last()->setPos_y(80);
    tacos.last()->set_position();

    if (targets_count() == 0 || planets.isEmpty())
    {
        qDebug() << is_multiplayer << " - " << is_second;

        if (is_multiplayer && !is_second)
        {
            clear_scene();

            set_planets();

            set_tacos();

            set_targets(levels.at(current_level)->getTargets());

            set_barriers(levels.at(current_level)->getBarrier());

            set_powers(levels.at(current_level)->getPowers(), true);

            set_score();

            is_second = true;
        }
        else
        {
            tim_cue->stop();

            det.set_score(score.first(), score.last());

            det.exec();

            if (!det.getNext())
            {
                this->close();
            }

            id_scene = 0;
        }
    }
}

bool Scene::write_file(QString dta)
{
    bool resp = false;

    ofstream ofs;
    ofs.open(GAME_NAME, ios::app);

    if (ofs.is_open())
    {
        ofs << dta.toStdString();

        resp = true;
    }

    ofs.close();

    return resp;
}

void Scene::get_lastId()
{
    ifstream ifs;
    ifs.open(GAME_NAME);

    QString str = "";
    if (ifs.is_open())
    {
        char c;
        while (ifs.get(c))
        {
            str.append(c);
        }
    }

    ifs.close();

    if (str.isEmpty())
    {
        id_scene = 1;

        // Ponemos el id de la partida.
        player_1->update_idGame(id_scene);

        if (is_multiplayer)
        {
            player_2->update_idGame(id_scene);
        }

        return;
    }

    ids_game.clear();

    QStringList list = str.split("|"); // Todos los registros.

    for (int i = 0; i < list.length(); i++)
    {
        QString dta = list.at(i);
        QStringList list_dta = dta.split("@");

        QString subdta = list_dta.first();
        QStringList list_subdta = subdta.split(";");

        QString id = list_subdta.first();

        ids_game.append(id.toInt());
    }

    // Obtenemos el mayor.
    int id_mayor = 0;
    for (int j = 0; j < ids_game.length(); j++)
    {
        if (ids_game.at(j) > id_mayor)
        {
            id_mayor = ids_game.at(j);
        }
    }

    if (id_scene == 0)
    {
        id_scene = id_mayor + 1;

        player_1->update_idGame(id_scene);

        if (is_multiplayer)
        {
            player_2->update_idGame(id_scene);
        }
    }
}

User *Scene::getPlayer_1() const
{
    return player_1;
}

void Scene::setPlayer_1(User *value)
{
    player_1 = value;
}

void Scene::on_btn_start_clicked()
{
    if (planets.length() == 0)
    {
        return;
    }

    if (!is_start)
    {
        if (next)
        {
            next = false;

            planets.first()->setPos_x(744);
            planets.first()->setPos_y(550);
            planets.first()->set_position();

            move_list();

            // Inicializamos el timer del pin.
            QTimer * tim = vel.getTimer();
            tim->start(10);
            vel.exec();

            vel.setY(false);

            // Obtenemos la velocidad.
            planets.first()->setVel_y(vel.getVel_y());
            vel.setVel_y(0);
        }

        tim_hor->start(40);

        ui->btn_start->setText("Pause");

        if (planets.first()->getVel_x() != 0)
        {
            tim_ver->start(50);

            tim_cue->start(8);

            tim_poder->start(70);
        }
    }
    else
    {
        tim_cue->stop();

        tim_hor->stop();

        tim_ver->stop();

        tim_poder->stop();

        ui->btn_start->setText("Continue");
    }

    is_start = !is_start;
}

void Scene::on_btn_save_clicked()
{
    if (!is_start)
    {
        QMessageBox msg(QMessageBox::Critical, "Gravity", "No se puede guardar la partida, primero se tiene que empezar", QMessageBox::Ok, this);
        msg.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        msg.exec();

        return;
    }

    get_lastId(); // Obtenemos el último id.

    tim_hor->stop();
    tim_ver->stop();
    tim_cue->stop();
    tim_poder->stop();

    /*
        Creamos el archivo y reiniciamos el juego.
    */

    // Validamos que si el id existe.
    int over = 1;

    QVector<int>::iterator p;
    p = ids_game.begin();
    for (; p != ids_game.end(); p++)
    {
        if (*p == id_scene)
        {
            // Preguntar si quiere sobreescribir el juego.
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Gravity", "Esta partida ya se encuentra guardada, ¿Desea sobreescribira?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                over = 2;
            }
            else
            {
                over = 3;
            }
        }
    }

    if (over == 3)
    {
        tim_hor->start(40);
        tim_ver->start(50);
        tim_poder->start(70);
        tim_cue->start(8);

        return;
    }

    QString dta = "";

    if (id_scene > 1)
    {
        dta.append("|");
    }

    // Encabezado
    dta.append(QString::number(id_scene));
    dta.append(";");
    dta.append(QString::number(current_level));
    dta.append(";");
    dta.append(tim_hor->isActive() ? "1" : "0"); // 1 si el taco 1 está moviendose.
    dta.append(";");
    dta.append(tim_ver->isActive() ? "1" : "0"); // 1 si el taco 2 está moviendose.
    dta.append(";");
    dta.append(ui->lbl_score1->text()); // Puntos del jugador 1
    dta.append(";");
    dta.append(ui->lbl_score2->text()); // Puntos del jugador 2
    dta.append(";");
    dta.append(next ? "1" : "0"); // Variable next
    dta.append(";");
    dta.append(limit ? "1" : "0"); // Variable limit
    dta.append(";");
    dta.append(is ? "1" : "0"); // Variable is
    dta.append(";");
    dta.append(is_start ? "1" : "0"); // Variable is_start
    dta.append(";");
    dta.append(vel.getY() ? "1" : "0"); // Variable vel del velocimetro
    dta.append("@");

    // Insertamos los planetas.
    QVector<Planet *>::iterator it;
    it = planets.begin();
    for (; it != planets.end(); it++)
    {
        Planet *p = *it;

        dta.append(QString::number(p->getPos_x()).append(";"));   // Posición en X

        dta.append(QString::number(p->getPos_y()).append(";"));   // Posición en Y

        dta.append(QString::number(p->getVel_x()).append(";"));   // Velocidad en X

        dta.append(QString::number(p->getVel_y()).append(";"));   // Velocidad en Y

        dta.append(QString::number(p->getAce_gra()).append(";")); // Aceleración de la graverdad.

        dta.append(QString::number(p->getY_aux()).append((";"))); // Posición Y auxiliar.

        dta.append(p->getRoot().append(";"));                     // Tipo

        dta.append(QString::number(p->getMode()).append(";"));    // Modo

        dta.append(QString::number(p->getAngle()).append(";"));   // Angulo

        dta.append(QString::number(p->getOrigin_x()).append(";"));   // Posición original en X

        dta.append(QString::number(p->getOrigin_y()));   // Posición original en Y

        dta.append(",");
    }

    dta.remove(dta.length() - 1, 1);

    dta.append("@");

    // Insertamos los tacos.
    QVector<Taco *>::iterator i;
    i = tacos.begin();
    for (; i != tacos.end(); i++)
    {
        Taco *t = *i;

        dta.append(QString::number(t->getPos_x()).append(";")); // Posición en X
        dta.append(QString::number(t->getPos_y())); // Posición en Y

        dta.append(",");

        // delete t;
    }

    dta.remove(dta.length() - 1, 1);

    dta.append("@");

    QString tar_s = "";
    QString bar_s = "";
    QString pow_s = "";

    // Insertamos los objetivos.
    QVector<TargetBarrier *>::iterator j;
    j = tarbar.begin();
    for (; j != tarbar.end(); j++)
    {
        TargetBarrier *tb = *j;

        if (tb->getType() == 1)
        {
            tar_s.append(QString::number(tb->getPos_x()).append(";")); // Posición en X
            tar_s.append(QString::number(tb->getPos_y()).append(";")); // Posición en Y
            tar_s.append(QString::number(tb->getScore())); // Puntos

            tar_s.append(",");
        }
        else if (tb->getType() == 2)
        {
            bar_s.append(QString::number(tb->getPos_x()).append(";")); // Posición en X
            bar_s.append(QString::number(tb->getPos_y()).append(";")); // Posición en Y
            bar_s.append(QString::number(tb->getScore())); // Puntos

            bar_s.append(",");
        }
        else
        {
            pow_s.append(QString::number(tb->getPos_x()).append(";"));      // Posición en X
            pow_s.append(QString::number(tb->getPos_y()).append(";"));      // Posición en Y
            pow_s.append(QString::number(tb->getType()).append(";"));       // Tipo
            pow_s.append(QString::number(tb->getAngulo()).append(";"));     // Angulo
            pow_s.append(QString::number(tb->getOrigen_x()).append(";"));   // Posición inicial en X
            pow_s.append(QString::number(tb->getOrigen_y()));               // Posición inicial en Y

            pow_s.append(",");
        }
    }

    tar_s.remove(tar_s.length() - 1, 1);
    bar_s.remove(bar_s.length() - 1, 1);
    pow_s.remove(pow_s.length() - 1, 1);

    dta.append(tar_s.append("@"));
    dta.append(bar_s.append("@"));
    dta.append(pow_s);

    // Si estamos sobreescribiendo, elimamos el que ya está.
    if (over == 2)
    {
        if (dta.at(0) == '|')
        {
            dta.remove(0, 1);
        }

        QString d = remove_game();

        if (!d.isEmpty())
        {
            d.push_front('|');
        }

        dta.append(d);
    }

    bool resp = write_file(dta);

    if (resp)
    {
        QMessageBox msg(QMessageBox::Information, "Gravity", "Partida almacenada correctamente.", QMessageBox::Ok, this);
        msg.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        msg.exec();
    }

    tim_hor->start(40);
    tim_ver->start(50);
    tim_poder->start(70);
    tim_cue->start(8);
}

void Scene::on_btn_load_clicked()
{
    QString id_lg = QString::number(player_1->getId_lastGame());

    if (id_lg == 0)
    {
        QMessageBox msg(QMessageBox::Warning, "Gravity", "No tiene ninguna partida almacenada.", QMessageBox::Ok, this);
        msg.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        msg.exec();

        return;
    }

    id_lg.push_front("Se cargará la partida número ");

    tim_cue->stop();
    tim_hor->stop();
    tim_ver->stop();
    tim_poder->stop();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Gravity", id_lg, QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        clear_scene();

        id_scene = player_1->getId_lastGame();

        load_game();
    }
    else
    {
        tim_hor->start(40);
        tim_ver->start(50);
        tim_cue->start(8);
    }
}

int Scene::getId_scene() const
{
    return id_scene;
}

void Scene::setId_scene(int value)
{
    id_scene = value;
}

int Scene::targets_count()
{
    int resp = 0;

    QVector<TargetBarrier *>::iterator it = tarbar.begin();
    for (; it != tarbar.end(); it++)
    {
        TargetBarrier * tb = *it;

        if (tb->getType() == 2)
        {
            resp++;
        }
    }

    return resp;
}

float Scene::aleatorio(float LO, float HI)
{
    float resp = 0;

    resp = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

    return resp;
}

QString Scene::remove_game()
{
    QString resp = "";

    ifstream ifs;
    ifs.open(GAME_NAME);

    QString str = "";
    if (ifs.is_open())
    {
        char c;
        while (ifs.get(c))
        {
            str.append(c);
        }
    }

    ifs.close();

    QStringList list = str.split("|");

    for (int i = 0; i < list.length(); i++)
    {
        QString dta = list.at(i);

        QStringList list_dta = dta.split("@");

        QString id = list_dta.first();

        if (id.toInt() != id_scene)
        {
            resp.append(dta);
        }
    }

    // Limpiamos el archivo.
    ofstream ofs;
    ofs.open(GAME_NAME, ofstream::out | ofstream::trunc);
    ofs.close();

    return resp;
}
