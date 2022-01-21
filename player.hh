/* Class player
 * ------------
 * COMP.CS.110 SPRING 2021
 * ------------
 * Kuvaus:
 * Luokka kuvaa muistipelin yksittäistä pelaajaa.
 * Pelaajaolioon on talennettun pelaajan nimi ja
 * löydetyt parit.
 *
 * Toiminnot:
 * get_name(); palauttaa pelaajan nimen.
 * add_pair(); Lisää pelaajalle parin.
 * get_pairs(); Palauttaa löytyneet parit QStringinä.
 * get_int_pairs(); Palauttaa löytyneet parit kokonaislukuna.
 *
 * Tekijä:
 * Nimi: William Irva
 * Opiskelijanumero: 50087565
 * Käyttäjätunnus: qcwiir
 * E-Mail: william.irva@tuni.fi
 * */
#ifndef PLAYER_HH
#define PLAYER_HH

#include <QMainWindow>
#include <string>

class Player
{
public:
    // Rakentaja, jonka jälkeen toimminnaltaan ilmiselvät funktiot
    Player(const QString& name);

    QString get_name();
    void add_pair();
    QString get_pairs();
    int get_int_pairs();


private:
    QString name_;
    int pairs_;
};

#endif // PLAYER_HH
