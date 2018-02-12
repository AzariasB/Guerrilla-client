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
    if(mActions.first)field.applyAction(*mActions.first);
    if(mActions.second)field.applyAction(*mActions.second);
}

void Turn::sendToSocket(QWebSocket &socket) const
{
    if(!mActions.first)return;
    socket.sendTextMessage(mActions.first->toString());

    if(!mActions.second)return;
    socket.sendTextMessage(mActions.second->toString());

}

void Turn::addAction(const std::shared_ptr<Action> &nwAction)
{
    if(mActions.second)return;//already full

    if(mActions.first){
        mActions.second = nwAction;
    }else{
        mActions.first = nwAction;
    }
}


float Turn::getWeight() const
{
    float tot = 0;

    if(mActions.first) tot += mActions.first->getWeight();
    if(mActions.second) tot += mActions.second->getWeight();

    return tot;
}

const std::shared_ptr<Action> &Turn::getAction(quint8 index)
{
    if(index > 2)throw std::out_of_range("Greater than 2");

    return index == 0 ? mActions.first : mActions.second;
}

bool Turn::hasAttack() const
{
    return (mActions.first && mActions.first->getType() == Action::ATTACK) ||
            (mActions.second && mActions.second->getType() == Action::ATTACK);

}
