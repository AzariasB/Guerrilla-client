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
 * File:   MainWindow.cpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QPushButton>
#include <QMessageBox>
#include "math.h"

#include "tree.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mWebSocket(),
    mLayout(new QGridLayout())
{
    QWidget *center = new QWidget();
    center->setLayout(mLayout);

    setCentralWidget(center);



    connect(&mWebSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(&mWebSocket, &QWebSocket::connected, this, &MainWindow::connected);

    mWebSocket.open(QUrl("ws://localhost:5000"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::error(QAbstractSocket::SocketError err)
{
    qDebug() << "An error occured" << err;
}

void MainWindow::connected()
{
    qDebug() << "Socket connected";
    connect(&mWebSocket, &QWebSocket::textMessageReceived, this, &MainWindow::messageReceived);
    mWebSocket.sendTextMessage("{\"type\":\"get_board\"}");
}

void MainWindow::messageReceived(QString msg)
{
   // qDebug() << msg;


    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    if(doc.isNull()){
        qDebug() << "Error while parsing json";
        return;
    }

    QJsonValue v = doc.object()["type"];

    int msgId = doc.object()["id"].toInt();

    if(mPrevMsgId > -1){
        if(msgId > mPrevMsgId +1){
            qWarning() << "Skipped a message prev = " << mPrevMsgId << " new = " << msgId << "\n";
        }else{
            mPrevMsgId = msgId;
        }
    }

    if(!v.isString())return;
    QString str = v.toString();

    if(str == "get_board"){
        updateBoard(doc.object()["data"].toArray());
    }else if(str == "your_id"){
        mId = doc.object()["data"].toInt();
        mPrevMsgId = msgId;
        qDebug() << "Setting mPrevMsgId\n";
        mBattleField.setId(mId);
    }else if(str == "your_turn"){
        play();
    }else if(str == "move"){
        //apply move
        QJsonObject data = doc.object()["data"].toObject();
        Coordinates from(data["from"].toObject());
        Coordinates to(data["to"].toObject());


        move(from, to);

    }else if(str == "attack"){
        //apply attack
        QJsonObject data = doc.object()["data"].toObject();
        Coordinates from(data["from"].toObject());
        Coordinates to(data["to"].toObject());

        attack(from, to);
    }else if(str == "you_win"){
        QMessageBox::information(this,"You win","You win this game, congratulations", QMessageBox::Ok);
    }else if(str == "you_loose"){
        QMessageBox::information(this,"You loose", "You lost this game, sad :(",QMessageBox::Ok);
    }

}

void MainWindow::move(const Coordinates &from, const Coordinates &to)
{
    mBattleField.move(from, to);

    mButtons[from.y][from.x]->setText("-");
    mButtons[from.y][from.x]->setStyleSheet("");

    mButtons[to.y][to.x]->setText(mBattleField.unitAt(to)->strType());
    if(mBattleField.unitAt(to)->getColor() == mId){
        mButtons[to.y][to.x]->setStyleSheet("QPushButton{color:blue;}");
    }
}

void MainWindow::play()
{
    Tree decisionTree;

    std::size_t treeDepth = 2/log10(mBattleField.numberOfUnits());

    treeDepth = std::min(treeDepth, (std::size_t)5);//max treedepth is five

    qDebug() << treeDepth;

    decisionTree.generate(treeDepth, mBattleField);

    decisionTree.getBestAction().sendToSocket(mWebSocket);

    mWebSocket.sendTextMessage("{\"type\":\"end_turn\"}");
}

void MainWindow::attack(const Coordinates &from, const Coordinates &to)
{
    mBattleField.attack(from, to);

    mButtons[to.y][to.x]->setText("-");
    mButtons[to.y][to.x]->setStyleSheet("");
}

void MainWindow::updateBoard(QJsonArray arr)
{
    mBattleField.fillField(arr);

    qDebug().noquote().nospace() << mBattleField;


    for(int y = 0; y < 25; ++y){
        for(int x = 0; x < 25; ++x){
            QPushButton *button = new QPushButton("-");

            connect(button, &QPushButton::clicked, [=](){
                qDebug() << "(x:"<<x<<", y "<<y<<")\n";//print the coordinates of a button when clicked
            });

            mButtons[y][x] = button;
            button->setMinimumWidth(30);
            auto &ptr = mBattleField.getField()[y][x];
            if(ptr){
                button->setText(ptr->strType());
                if(ptr->getColor() == mId){
                    button->setStyleSheet("QPushButton{color : blue;}");
                }
            }
            mLayout->addWidget(button, y, x);
        }
    }
    mWebSocket.sendTextMessage("{\"type\":\"is_ready\"}");
}
