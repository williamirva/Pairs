#include "mainwindow.hh"
#include "player.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    create_players();
    calculate_factors(NUMBER_OF_CARDS, FACTOR_1, FACTOR_2);
    create_char_vector();
    init_cards();
    init_update_button();
    init_in_turn_display();
    init_text_browser();
    init_quit_button();
    in_turn_toggle(player_in_turn);

    setWindowTitle(tr("Muistipeli"));

}

MainWindow::~MainWindow()
{
    delete ui;
}
// Luo pelaaja oliot, tässä funktiossa ei mitään erikoista.
void MainWindow::create_players()
{
    for(int i=0; i<NUMBER_OF_PLAYERS; ++i)
    {
        QString id = "Player";
        id += QString::number(i+1);
        Player* new_player = new Player(id);
        players_.push_back(new_player);
    }
    player_in_turn = players_.at(in_turn_index);

}
/* Luo vektorin, jossa on kaksi paria kirjaimia korttien lukumäärän
 * mukaan lopuksi vektorin sisältö sekoitetaan tietokoneen kellon
 * määräämän siemenluvun avulla.
 * */

void MainWindow::create_char_vector()
{
    char c = 'A';
    for (int i=0; i<NUMBER_OF_CARDS; i+=2)
    {
        card_chars_.push_back(c);
        card_chars_.push_back(c);
        c++;
    }
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    shuffle(card_chars_.begin(), card_chars_.end(), gen);
}
/* Laskee korttien lukumääräälle lähimmät tekijät,
 * jotta pelilaudan muodostama grid saadaan järkevän malliseksi
 * */
void MainWindow::calculate_factors(int product, int smaller_factor, int bigger_factor)
{
    for(int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
    FACTOR_1 = smaller_factor;
    FACTOR_2 = bigger_factor;
}
/* Luo pelilautana toimivan gridlayoutin ja asettaa painikkeina
 * toimivat kortit pelilaudalle calculate_factors funktion
 * laskemien tekijöiden mukaisesti.
 * Funktio luo siis myös painikkeet, asettaa niille tausta värin,
 * sekä lisää kortin ja sitä vastaavan kirjaimen car_char_pairs mappiin.
 * painikkeet myös yhdistetään handle_card_click slottiin.
 * */
void MainWindow::init_cards()
{
    QWidget* game_board = new QWidget(this);
    QGridLayout* gLayout = new QGridLayout(game_board);
    int i=0;

    for (int y=0; y<FACTOR_1; y++)
        for (int x=0; x<FACTOR_2; x++)
        {
            QString letter = "";
            letter += card_chars_.at(i);
            QPushButton* card = new QPushButton(this);
            card->setFixedSize(CARD_WIDTH, CARD_HEIGHT);
            card_char_pairs.insert({card, letter});
            card->setStyleSheet("background: #000000;");
            gLayout->addWidget(card, y, x);

            connect(card,&QPushButton::clicked,this,
                    &MainWindow::handle_card_click);

            ++i;

        }
    game_board->setGeometry(0,50,800,600);
}
/* Luo pelilaudan päivittävän ja kortit takaisin kääntävän
 * painikkeen.
 * */
void MainWindow::init_update_button()
{
    update_board = new QPushButton("flip cards",this);
    update_board->setGeometry(100,640,100,50);
    update_board->setDisabled(true);

    connect(update_board,&QPushButton::clicked,this,
            &MainWindow::handle_update_click);
}
/* Luo Widgetin, joka sisältää pelaajien vuoron osoittavat
 * radiobuttonit.
 * */
void MainWindow::init_in_turn_display()
{

    QWidget* in_turn_players = new QWidget(this);
    QVBoxLayout* in_turnLayout = new QVBoxLayout(in_turn_players);
    for(auto player : players_)
    {
        QRadioButton* in_turn = new QRadioButton(player->get_name(),this);
        in_turn_displays.insert({player,in_turn});
        in_turnLayout->addWidget(in_turn);
        in_turn->setDisabled(true);
    }
    in_turn_players->setGeometry(FACTOR_2 + MARGIN,
                                 FACTOR_1, 100, 600);
    in_turn_players->show();

}
/* Luo infotekstit näyttävän textbrowserin
 * */
void MainWindow::init_text_browser()
{
    textbrowser = new QTextBrowser(this);
    textbrowser->setGeometry(0,
                             0, 300, 50);
}

void MainWindow::init_quit_button()
{
    quit_button = new QPushButton("QUIT",this);
    quit_button->setGeometry(500,640,100,50);

    connect(quit_button,&QPushButton::clicked,this,
            &MainWindow::handle_quit_click);
}
/* Käsittelee korttien klikkaukset. Estää kortin klikkaamisen
 * uudelleen ja lisää kortin tarkistusvektoriin.
 * Kun toinen kortti käännetään, kaikkien korttien
 * kääntäminen estetään ja asetetaan pelipöydän päivittävä nappi
 * käytettäväksi. Kun kortti käännetään siihen asetetaan näkyville,
 * sitä vastaava teksti, joka haetaan card_char_pairs mapista.
 * Myös kortin väri vaihdetaan "kuvapuolta" vastaavaksi.
 * */
void MainWindow::handle_card_click()
{
    QPushButton* card = static_cast<QPushButton*>(sender());

    card->setText(card_char_pairs.at(card));
    card->setStyleSheet("background: #0000ff;");
    pair_check_.push_back(card);
    card->setDisabled(true);
    if (pair_check_.size() == 2)
    {
        for (auto pair : card_char_pairs)
        {
            pair.first->setDisabled(true);
        }
        update_board->setDisabled(false);
    }
}
/* Tarkistaa parin is_pair funktion avulla. Jos pari löytyy
 * Lisätään se vuorossa olevalle pelaajalle ja piilotetaan ne
 * pelilaudalta. Tämän jälkeen tarkistetaan onko pelilauta tyhjä.
 * Jos pelilauta on tyhjä, ajetaan declare_winner funktio, joka päättää
 * pelin. Mikäli paria ei löydy kortit käännetään ja vaihdetaan vuoro
 * seuraavalle pelaajalle.
 * */
void MainWindow::handle_update_click()
{
    if(is_pair(pair_check_))
    {
     player_in_turn->add_pair();
     pair_check_.at(0)->hide();
     pair_check_.at(1)->hide();
     pair_check_.clear();
     if (is_table_empty())
     {
         update_board->setDisabled(true);
         declare_winner();
         return;
     }
     update_text_browser(true);
     for (auto pair : card_char_pairs)
     {
         pair.first->setDisabled(false);
     }
     update_board->setDisabled(true);
     return;
     }
     update_text_browser(false);
     pair_check_.at(0)->setDisabled(false);
     pair_check_.at(1)->setDisabled(false);
     pair_check_.at(0)->setStyleSheet("background: #000000;");
     pair_check_.at(1)->setStyleSheet("background: #000000;");
     pair_check_.at(0)->setText("");
     pair_check_.at(1)->setText("");
     pair_check_.clear();
     if (in_turn_index+1 >= NUMBER_OF_PLAYERS)
     {
     in_turn_index=0;
     }
     else
     {
     in_turn_index+=1;
     }
     player_in_turn = players_.at(in_turn_index);
     in_turn_toggle(player_in_turn);
     for (auto pair : card_char_pairs)
     {
         pair.first->setDisabled(false);
     }
     update_board->setDisabled(true);
}
// Poistaa pelaajaolito ja päättää ohjelman suorituksen
void MainWindow::handle_quit_click()
{
    for (auto player : players_)
    {
        delete player;
    }
    close();
}
// Tarkistaa ovatko tarkistusvektorin sisältämät kortit pareja.
bool MainWindow::is_pair(std::vector<QPushButton*> cards)
{
    if (cards.at(0)->text() == cards.at(1)->text())
    {
        return true;
    }
    return false;
}
/* Tulostaa textbrowseriin infotekstin, riippuen
 * bool arvosta isfound, eli löytyikö pari vai ei.
 * */
void MainWindow::update_text_browser(bool isfound)
{
    if (isfound)
    {
        QString text = "";
        text += PAIR_FOUND;
        text += player_in_turn->get_name();
        text += " has ";
        text += player_in_turn->get_pairs();
        text += " pairs!";
        textbrowser->setText(text);
        return;
    }
    textbrowser->setText(PAIR_NOT_FOUND);
}
/*Tarkistaa on onko pelilauta tyhjä laskemalla
 * kaikkien pelaajien parit ja vertaamalla niitä koko
 * pelin pareihin.
 * */
bool MainWindow::is_table_empty()
{
    int total_pairs = NUMBER_OF_CARDS / 2;
    int found_pairs = 0;
    for (Player* player : players_)
    {
        found_pairs += player->get_int_pairs();
    }
    if (found_pairs == total_pairs)
    {
        return true;
    }
    return false;
}
/* Selvittää kuka pelaajista on löytänyt eniten pareja
 * ja tulostaa textbrowseriin voittajan julistuksen.
 * */
void MainWindow::declare_winner()
{
    Player* winner = players_.at(0);
    std::vector<Player*> winners;
    // Tarkistetaan kellä on eniten pareja.
    for (Player* player : players_)
    {
        if (player->get_int_pairs() > winner->get_int_pairs())
        {
            winner = player;
        }
    }
    // Tarkistetaan on kellään yhtä paljon pareja, kuin voittajalla
    for (Player* player : players_)
    {
        if (player->get_int_pairs() == winner->get_int_pairs())
        {
            winners.push_back(player);
        }
    }
    // Jos voittajia on enemmän kuin yksi, julistetaan tasapeli.
    // näiden pelaajien kesken.
    // Muussa tapauksessa julistetaan voittaja.
    if (winners.size() > 1)
    {
        QString text = "";
        text += "Tie between ";
        for (Player* player : winners)
        {
            text += player->get_name();
            text += " ";
        }
        text += "with ";
        text += winner->get_pairs();
        text += " pairs!";
        textbrowser->setText(text);
        return;
    }
    else
    {
        QString text = "";
        text += winner->get_name();
        text += " has won, with ";
        text += winner->get_pairs();
        text += " pairs!";
        textbrowser->setText(text);
        return;
    }
}
// Vaihtaa vuorossa olevan pelaajan radiobuttonin päälle.
void MainWindow::in_turn_toggle(Player* player)
{
    in_turn_displays.at(player)->toggle();
}


