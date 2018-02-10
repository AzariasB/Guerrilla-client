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
 * File:   action.cpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#include "action.hpp"
#include "battlefield.hpp"

Action::Action():
    mType(UNKNOWN),
    mFrom(),
    mTo()
{

}

Action::Action(ACTION_TYPE type, Coordinates from, Coordinates to):
    mType(type),
    mFrom(from),
    mTo(to)
{

}

QJsonObject Action::toJson() const
{
    QJsonObject res;
    res["type"] = mType == ATTACK ? "attack" : "move";

    QJsonObject data;
    data["from"] = mFrom.toJsonObjet();
    data["to"] = mTo.toJsonObjet();

    res["data"] = data;

    return res;
}



void Turn::applyActions(BattleField &field)
{
    if(mActions[0])field.applyAction(*mActions[0]);
    if(mActions[1])field.applyAction(*mActions[1]);
}

void Turn::sendToSocket(QWebSocket &socket) const
{
    if(!mActions[0])return;
    socket.sendTextMessage(mActions[0]->toString());

    if(!mActions[1])return;
    socket.sendTextMessage(mActions[1]->toString());

}

void Turn::addAction(const std::shared_ptr<Action> &nwAction)
{
    if(mActions[1])return;//already full

    if(mActions[0]){
        mActions[1] = nwAction;
    }else{
        mActions[0] = nwAction;
    }
}


float Turn::getWeight() const
{
    float tot = 0;

    if(mActions[0]) tot += mActions[0]->getWeight();
    if(mActions[1]) tot += mActions[1]->getWeight();

    return tot;
}

const std::shared_ptr<Action> &Turn::getAction(quint8 index)
{
    if(index > 2)throw std::out_of_range("Greater than 2");

    return mActions[index];
}

bool Turn::hasAttack() const
{
    return (mActions[0] && mActions[0]->getType() == Action::ATTACK) ||
            (mActions[1] && mActions[1]->getType() == Action::ATTACK);

}
