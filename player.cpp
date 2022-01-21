#include "player.hh"
#include <iostream>

Player::Player(const QString& name):
    name_(name), pairs_(0)
{

}

QString Player::get_name()
{
    return name_;
}

void Player::add_pair()
{
    pairs_ += 1;
}

QString Player::get_pairs()
{
    QString pairs = QString::number(pairs_);
    return pairs;
}

int Player::get_int_pairs()
{
    int pairs = pairs_;
    return pairs;
}

