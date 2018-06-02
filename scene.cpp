#include "scene.h"
#include "ui_scene.h"

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

    current_level = 0;

    connect(this, SIGNAL(open(bool)), this, SLOT(rec_open(bool)));

    gc = new QGraphicsScene(this);
    ui->graphicsView->setScene(gc);
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
    myPen.setStyle(Qt::DashLine);

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
    set_barriers();
    set_targets();
    set_powers();

    tim_hor = new QTimer();
    connect(tim_hor, SIGNAL(timeout()), this, SLOT(hor_move()));

    tim_ver = new QTimer();
    connect(tim_ver, SIGNAL(timeout()), this, SLOT(ver_move()));

    tim_cue = new QTimer();
    connect(tim_cue, SIGNAL(timeout()), this, SLOT(mov_planet()));

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

    delete sou_back;

    delete l1;

    delete l2;

    delete l3;

    delete l4;
}

void Scene::rec_open(bool is_multi)
{
    sou_back->setVolume(35);
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
            tim_cue->start(5);

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

        tim_cue->start(5);
    }
}

void Scene::mov_planet()
{
    planets.first()->mov_vertical(is);

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
                    qDebug() << planets.first()->getPos_x() << " - " << tg->getPos_x() << ", " << planets.first()->getPos_y() << " - " << tg->getPos_y();

                    if ((tg->getPos_x() - planets.first()->getPos_x()) < 20 && (tg->getPos_y() - planets.first()->getPos_y()) < 20)
                    {
                        tg->play_sound();

                        score.first() += tg->getScore();

                        gc->removeItem(tg); // Eliminamos el objetivo.

                        tarbar.removeOne(tg);
                    }
                }
                else
                {
                    tg->play_sound();

                    score.first() -= tg->getScore();

                    // Rebote.
                }

                set_score();

                return;
            }
        }

        // validamos si colisiono con una pared.
        bool resp = planets.first()->border(sounds.last());

        if (resp)
        {
            tim_cue->stop();

            delete_planet();
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
    for (int i = 0; i < plan.length(); i++)
    {
        x -= 37;

        QStringList dta = plan.at(i).split('-');

        QString t = dta.at(0);
        QString g = dta.at(1);

        QString root = ":/Img/";
        root.append(t.append(".png"));

        planets.append(new Planet(x, 582, 32, 32, root, g.toFloat(), levels.at(current_level)->getCoe_res()));
        gc->addItem(planets.last());
        planets.last()->set_position();
    }
}

void Scene::set_barriers()
{
    QVector<QString> barr = levels.at(current_level)->getBarrier();

    for (int i = 0; i < barr.length(); i++)
    {
        QStringList dta = barr.at(i).split('-');

        QString x = dta.at(0);
        QString y = dta.at(1);
        QString s = dta.at(2);

        tarbar.append(new TargetBarrier(x.toInt(), y.toInt() , 24, 24, ":/Img/asteroid.png", s.toInt(), 1));
        gc->addItem(tarbar.last());
        tarbar.last()->set_position();
    }
}

void Scene::set_targets()
{
    QVector<QString> targets = levels.at(current_level)->getTargets();

    for (int i = 0; i < targets.length(); i++)
    {
        QStringList dta = targets.at(i).split('-');

        QString x = dta.at(0);
        QString y = dta.at(1);
        QString s = dta.at(2);

        tarbar.append(new TargetBarrier(x.toInt(), y.toInt() , 32, 32, ":/Img/14.png", s.toInt(), 2));
        gc->addItem(tarbar.last());
        tarbar.last()->set_position();
    }
}

void Scene::set_powers()
{
    QVector<QString> powers = levels.at(current_level)->getPowers();

    for (int i = 0; i < powers.length(); i++)
    {
        QStringList dta = powers.at(i).split('-');

        QString x = dta.at(0);
        QString y = dta.at(1);
        QString s = dta.at(2);

        QString root = s.toInt() == 3 ? ":/Img/mov1.png" : ":/Img/mov2.png";

        tarbar.append(new TargetBarrier(x.toInt(), y.toInt() , 24, 24, root, 0, s.toInt()));
        gc->addItem(tarbar.last());
        tarbar.last()->set_position();
    }
}

void Scene::read_level()
{
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

            if (ind == 1)
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

    tim_hor->stop();
    tim_ver->stop();

    ui->btn_start->setText("Start");

    is_start = false;

    limit = false;

    next = true;

    is = false;

    vel.setY(true);

    gc->removeItem(planets.first());

    planets.removeAt(0);

    if (planets.isEmpty() || tarbar.isEmpty())
    {
        det.set_score(score.first(), score.last());

        det.exec();

        if (!det.getNext())
        {
            this->close();
        }

        id_scene = 0;
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

        return;
    }

    QStringList list = str.split("|"); // Todos los registros.
    QString dta = list.last();

    QStringList list_dta = dta.split("@");
    QString id = list_dta.first();

    id_scene = id.toInt() + 1;
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
            tim_cue->start(5);
        }
    }
    else
    {
        tim_cue->stop();

        tim_hor->stop();

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

    if (id_scene == 0)
    {
        get_lastId(); // Obtenemos el último id.
    }

    tim_hor->stop();
    tim_ver->stop();
    tim_cue->stop();

    /*
        Creamos el archivo y reiniciamos el juego.
    */

    // Validamos que si el id existe.

    // Insertamos los planetas.
    QString dta = "";

    if (id_scene > 1)
    {
        dta.append("|");
    }

    dta.append(QString::number(id_scene));

    dta.append("@");

    QVector<Planet *>::iterator it;
    it = planets.begin();
    for (; it != planets.end(); it++)
    {
        Planet *p = *it;

        dta.append(QString::number(p->getPos_x()).append("-")); // Posición en X
        dta.append(QString::number(p->getPos_x()).append("-")); // Posición en Y
        dta.append(QString::number(p->getVel_x()).append("-")); // Velocidad en X
        dta.append(QString::number(p->getVel_y()).append("-")); // Velocidad en Y
        dta.append(QString::number(p->getCoe_fr()).append("-")); // Coeficiente de restitución
        dta.append(QString::number(p->getAce_gra()).append("-")); // Aceleración de la graverdad.
        dta.append(QString::number(p->getY_aux())); // Posición Y auxiliar.z

        dta.append(",");

        // delete p;
    }

    dta.remove(dta.length() - 1, 1);

    dta.append("@");

    // Insertamos los tacos.
    QVector<Taco *>::iterator i;
    i = tacos.begin();
    for (; i != tacos.end(); i++)
    {
        Taco *t = *i;

        dta.append(QString::number(t->getPos_x()).append("-")); // Posición en X
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
            tar_s.append(QString::number(tb->getPos_x()).append("-")); // Posición en X
            tar_s.append(QString::number(tb->getPos_y()).append("-")); // Posición en Y
            tar_s.append(QString::number(tb->getScore())); // Puntos

            tar_s.append(",");
        }
        else if (tb->getType() == 2)
        {
            bar_s.append(QString::number(tb->getPos_x()).append("-")); // Posición en X
            bar_s.append(QString::number(tb->getPos_y()).append("-")); // Posición en Y
            bar_s.append(QString::number(tb->getScore())); // Puntos

            bar_s.append(",");
        }
        else
        {
            pow_s.append(QString::number(tb->getPos_x()).append("-")); // Posición en X
            pow_s.append(QString::number(tb->getPos_y()).append("-")); // Posición en Y
            pow_s.append(QString::number(tb->getType())); // Tipo

            pow_s.append(",");
        }
    }

    tar_s.remove(tar_s.length() - 1, 1);
    bar_s.remove(bar_s.length() - 1, 1);
    pow_s.remove(pow_s.length() - 1, 1);

    dta.append(tar_s.append("@"));
    dta.append(bar_s.append("@"));
    dta.append(pow_s);

    bool resp = write_file(dta);

    if (resp)
    {
        QMessageBox msg(QMessageBox::Information, "Gravity", "Partida almacenada correctamente.", QMessageBox::Ok, this);
        msg.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
        msg.exec();
    }

    tim_hor->start(40);
    tim_ver->start(50);
    tim_cue->start(5);
}

int Scene::getId_scene() const
{
    return id_scene;
}

void Scene::setId_scene(int value)
{
    id_scene = value;
}
