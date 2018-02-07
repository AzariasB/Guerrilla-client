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
 * File:   acion.hpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#ifndef ACION_HPP
#define ACION_HPP


#include <QJsonObject>
#include <QJsonDocument>
#include <qwebsocket.h>
#include "coordinates.hpp"
#include <memory>


class Action
{
public:
    enum ACTION_TYPE{ATTACK, MOVE, UNKNOWN};

    Action();//empty action for the minmax

    Action(ACTION_TYPE type, Coordinates from, Coordinates to);

    QJsonObject toJson() const;

    operator QString() const{
        return QString(QJsonDocument(toJson()).toJson(QJsonDocument::Compact));
    }

    QString toString() const
    {
        return this->operator QString();
    }

    ACTION_TYPE getType() const{
        return mType;
    }

    const Coordinates &getFrom() const{
        return mFrom;
    }

    const Coordinates &getTo() const{
        return mTo;
    }

    float getWeight() const {
        return mWeight;
    }

    void setWeight(float weight){
        mWeight = weight;
    }

private:
    ACTION_TYPE mType;
    Coordinates mFrom;
    Coordinates mTo;
    float mWeight;
};


class Turn{

public:
    Turn()
    {
    }

    Turn(const std::shared_ptr<Action> &firstAction):
        mActions({firstAction})
    {
    }

    Turn(const std::shared_ptr<Action> &firstAction,const std::shared_ptr<Action> &secondAction):
        mActions({firstAction, secondAction})
    {
    }

    void sendToSocket(QWebSocket &socket) const
    {
        if(!mActions[0])return;
        socket.sendTextMessage(mActions[0]->toString());

        if(!mActions[1])return;
        socket.sendTextMessage(mActions[1]->toString());

    }

    void addAction(const std::shared_ptr<Action> &nwAction)
    {
        if(mActions[1])return;//already full

        if(mActions[0]){
            mActions[1] = nwAction;
        }else{
            mActions[0] = nwAction;
        }
    }

    float getWeight() const
    {
        float tot = 0;

        if(mActions[0]) tot += mActions[0]->getWeight();
        if(mActions[1]) tot += mActions[1]->getWeight();

        return tot;
    }

    const std::shared_ptr<Action> &getAction(quint8 index)
    {
        if(index > 2)throw std::out_of_range("Greater than 2");

        return mActions[index];
    }

    bool hasAttack() const
    {
        return mActions[0] && mActions[0]->getType() == Action::ATTACK ||
                mActions[1] && mActions[1]->getType() == Action::ATTACK;

    }


private:
    std::array<std::shared_ptr<Action>, 2> mActions;
};

#endif // ACION_HPP
