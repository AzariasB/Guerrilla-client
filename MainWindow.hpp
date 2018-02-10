/*
 * The MIT License
 *
 * Copyright 2017 azarias.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * File:   MainWindow.hpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QWebSocket>
 #include <QJsonObject>
#include <battlefield.hpp>
#include <QGridLayout>
#include <array>
#include <QPushButton>

/**
 * @brief The MainWindow class
 * MainWindow is the class used to represent
 * the main window of the application
 * it also handles the websocket communications with the server
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /**
     * @brief connected whenever
     * the socket is connected
     */
    void connected();

    /**
     * @brief messageReceived
     * whenever a message is received by the socket
     * @param msg
     */
    void messageReceived(QString msg);

    /**
     * @brief error
     * whenever an error happened with the socket
     * @param err
     */
    void error(QAbstractSocket::SocketError err);

private:
    /**
     * @brief updateBoard
     * updates the board, using the given json array
     * that is : updating the battlefield, and updating the buttons
     * @param arr
     */
    void updateBoard(QJsonArray arr);

    /**
     * @brief move whenever a move action is sent by the socket
     * performs the move, on the field, then update
     * the buttons
     * @param from
     * @param to
     */
    void move(const Coordinates &from, const Coordinates &to);

    /**
     * @brief attack whenever an attack action is sent by the socket
     * performs the attacke, on the field then update the buttons
     * @param from
     * @param to
     */
    void attack(const Coordinates &from, const Coordinates &to);

    /**
     * @brief play
     * when it's the player's turn to make a move
     * will calculate the best move and send it to the
     * server via websocket
     */
    void play();

    /**
     * @brief mWebSocket
     * the websocket used to communicate
     * with the server
     */
    QWebSocket mWebSocket;

    /**
     * @brief mBattleField the
     * battlefield
     */
    BattleField mBattleField;

    /**
     * @brief mLayout layout
     */
    QGridLayout *mLayout;

    /**
     * @brief mButtons a pointer to all the buttons
     * to display the battle field. Used to update them
     * whenever it's necessary.
     * When clicking on a button, the coordinate of it
     * will be printed out
     * all the pointers are automatically destroyed by Qt
     * so no need to handle their destruction
     */
    std::array<std::array<QPushButton*, 25>, 25> mButtons;

    /**
     * @brief mId id of the player
     */
    int mId;

    /**
     * @brief mPrevMsgId id of the previous message
     * to be sure the message arrive in order
     */
    int mPrevMsgId = -1;
};

#endif // MAINWINDOW_HPP
