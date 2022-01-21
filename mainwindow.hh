/* Class mainwindow
 * ----------------
 * COMP.SC.110. SPRING 2021
 * ----------------
 * Kuvaus:
 * Luokka ja gui-ikkuna, jossa koko peli tapahtuu.
 * Ikkunassa näkyvät radiobuttonit osoittavat kenen vuoro
 * on kääntää kortteja. Jos pelaaja ei löydä paria, siirtyy
 * vuoro seuraavalle. Jos pari löytyy vuoro pysyy pelaajalla
 * ja hänelle lisätään yksi pari ja löytynyt pari poistetaan
 * laudalta. Text browser ilmoittaa löytyikö paria ja jos löytyy,
 * niin sen kuinka monta paria pelaajalla on. Lisäksi textbrowser
 * ilmoittaa voittajan tai voittajat, kun pöytä on tyhjä.
 * Pelaajan on käännettävä kortit takaisin piiloon flip cards napilla.
 * Pelaaja ei voi kääntää kuin kaksi korttia, eikä jo käännettyä
 * korttia voi kääntää takaisin. Pelin voi sulkea quit-painikkeella
 * milloin tahansa.
 * ------------------
 * Tietosäiliöt:
 * Vector players: Sisältää pelaajaoliot.
 * map card_char_pairs: Sisältää kortit ja niitä vastaavat kirjaimet.
 * vector pair_check: tänne siirretään käännetyt kortit tarkistusta varten.
 * map in_turn_displays: Tallennettuna pelaaja ja sen radiobutton, jonka
 *                       avulla osoitetaan, kun pelaaja on vuorossa.
 *-------------------
 * Tekijä:
 * Nimi: William Irva
 * Opiskelijanumero: 50087565
 * Käyttäjätunnus: qcwiir
 * E-Mail: william.irva@tuni.fi
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "player.hh"

#include <vector>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QTextBrowser>
#include <QStatusBar>
#include <algorithm>
#include <random>
#include <chrono>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    std::vector<char> card_chars_;
    ~MainWindow();

private slots:
    // Ottaa vastaan kortin klikkauksen
    void handle_card_click();
    void handle_update_click();
    void handle_quit_click();
    void in_turn_toggle(Player*);

private:
    Ui::MainWindow *ui;
    QTextBrowser* textbrowser;
    QPushButton* update_board;
    QPushButton* quit_button;

    // QString ja int vakiot korttien asettelun helpottamiseksi
    QString PAIR_FOUND = "Pair found! ";
    QString PAIR_NOT_FOUND = "Pair not found, unlucky";

    // Korkeintaan 50 korttia ja korttien määrä oltava parillinen.
    int const NUMBER_OF_CARDS = 8;

    int const NUMBER_OF_PLAYERS = 3;
    int FACTOR_1 = 0;
    int FACTOR_2 = 0;
    int const CARD_WIDTH = 50;
    int const CARD_HEIGHT = 80;
    int const GAME_BOARD_X = 0;
    int const GAME_BOARD_Y = 100;
    int const MARGIN = 700;
    // Numerovakio pelaajien vuoron vaihtamisen.
    int in_turn_index = 0;

    std::vector<Player*> players_;
    std::map<QPushButton*, QString> card_char_pairs;
    std::vector<QPushButton*> pair_check_;
    std::map<Player*, QRadioButton*> in_turn_displays;

    // Tähän asetetaan vuorossa oleva pelaajaolio
    Player* player_in_turn;
    // Satunnaislukugeneraattori kirjainvektorin sekoittamiseen.
    std::default_random_engine gen;

    // Pelaajaoliot luova funktio
    void create_players();
    // Luodaan vektori korttien kirjaimille
    void create_char_vector();
    // Laskee tekijät korttien määrälle pelilautaa varten
    void calculate_factors(int NUMBER_OF_CARDS, int FACTOR_1, int FACTOR_2);
    // Asettaa kortit pelilaudalle
    void init_cards();
    // Luo pöydän päivittävän painikkeen.
    void init_update_button();
    // Luo vuoron osoittavat radiobuttonit.
    void init_in_turn_display();
    // Luo tekstinäytön.
    void init_text_browser();
    // Luo quit-napin.
    void init_quit_button();
    // Tarkistaa onko käännetyt kortit pareja
    bool is_pair(std::vector<QPushButton*>);
    // Päivittää tekstin parin löytymisen perusteella.
    void update_text_browser(bool isfound);
    // Tarkistaa voittajan.
    bool is_table_empty();
    // Julistaa voittajan ja päättää pelin.
    void declare_winner();

};
#endif // MAINWINDOW_HH
