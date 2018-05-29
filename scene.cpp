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
    sounds.last()->setMedia(QUrl("qrc:/Sounds/exp.mp3"));

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
}

Scene::~Scene()
{
    delete ui;

    delete player_1;

    delete player_2;

    delete tim_hor;

    delete tim_ver;

    delete l1;

    delete l2;

    delete l3;

    delete l4;
}

void Scene::rec_open(bool is_multi)
{
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
            tim_cue->start(3);

            tacos.first()->set_position();

            sounds.first()->play();
        }
        else
        {
            tim_hor->stop();

            tacos.first()->setPos_y(585);

            tacos.first()->set_position();

            tim_cue->stop();

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

        planets.first()->setVel_x(-900);

        tim_cue->start(13);
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

        tim_ver->start(50);
    }

    if (planets.first()->getPos_x() <= 700)
    {
        is = true;
    }

    if (is)
    {
        // validamos si colisiono con una pared.
        bool resp = planets.first()->border(sounds.last());

        if (resp)
        {
            tim_cue->stop();

            delete_planet();
        }

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
                    // qDebug() << planets.first()->getPos_x() << " - " << tg->getPos_x() << ", " << planets.first()->getPos_y() << " - " << tg->getPos_y();

                    if ((tg->getPos_x() - planets.first()->getPos_x()) < 20 && (tg->getPos_y() - planets.first()->getPos_y()) < 20)
                    {
                        score.first() += tg->getScore();

                        gc->removeItem(tg); // Eliminamos el objetivo.

                        tarbar.removeOne(tg);
                    }
                }
                else
                {
                    sounds.at(2)->play();

                    score.first() -= tg->getScore();

                    // Rebote.
                }

                set_score();

                return;
            }
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
    int num_planets = levels.at(current_level)->getNum_planets();
    int x = 723;
    int l = 1;

    for (int i = 0; i < num_planets; i++)
    {
        x -= 37;

        QString root = ":/Img/";
        root.append(QString::number(l++).append(".png"));

        planets.append(new Planet(x, 582, 32, 32, root, 6, levels.at(current_level)->getCoe_vis()));
        gc->addItem(planets.last());
        planets.last()->set_position();

        if (l > 11)
        {
            l = 1;
        }
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

        if (s.toInt() != 3 && s.toInt() != 4)
        {
            tarbar.append(new TargetBarrier(x.toInt(), y.toInt() , 32, 32, ":/Img/14.png", s.toInt(), 2));
            gc->addItem(tarbar.last());
            tarbar.last()->set_position();
        }
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

        if (s.toInt() == 3 || s.toInt() == 4)
        {
            QString root = s.toInt() == 3 ? ":/Img/mov1.png" : ":/Img/mov2.png";

            tarbar.append(new TargetBarrier(x.toInt(), y.toInt() , 24, 24, root, 0, s.toInt()));
            gc->addItem(tarbar.last());
            tarbar.last()->set_position();
        }
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

            if (ind == 0)
            {
                QString level_name = "Level ";
                ui->lbl_level->setText(level_name.append(one));
            }

            one = data.at(1);
            levels.at(ind)->setCoe_res(one.toFloat());

            one = data.at(2);
            levels.at(ind)->setCoe_vis(one.toFloat());

            one = data.at(3);
            levels.at(ind)->setNum_planets(one.toInt());

            QVector<QString> obj;

            // Obtenemos los obstaculos.
            one = dta.at(1);
            data = one.split(',');

            QStringList::iterator i = data.begin();
            for(; i != data.end(); i++)
            {
                obj.append(*i);
            }

            levels.at(ind)->setTargets(obj);

            // Obtenemos los obstaculos.
            obj.clear();
            one = dta.at(2);
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
    sounds.at(3)->play();

    gc->removeItem(planets.first());

    planets.removeAt(0);

    ui->btn_start->setText("Start");

    is_start = false;

    limit = false;

    next = true;

    is = false;

    tim_ver->stop();

    // Ejecutar sonido.
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

            vel.setVel_y(0);

            // Inicializamos el timer del pin.
            QTimer * tim = vel.getTimer();
            tim->start(10);
            vel.exec();

            // Obtenemos la velocidad.
            planets.first()->setVel_y(vel.getVel_y());
            vel.setVel_y(0);
        }

        tim_hor->start(40);

        ui->btn_start->setText("Pause");

        if (planets.first()->getVel_x() != 0)
        {
            tim_cue->start(3);
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
