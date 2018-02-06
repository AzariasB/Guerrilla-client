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
 * File:   battlefield.cpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#include "battlefield.hpp"
#include <qjsonarray.h>
#include <qjsonvalue.h>

BattleField::BattleField()
{

}

void BattleField::fillField(const QJsonArray &units)
{
    for(QJsonValue val : units){
        QJsonObject obj = val.toObject();
        QJsonObject coordinates = obj["coordinates"].toObject();
        int x = coordinates["x"].toInt();
        int y = coordinates["y"].toInt();

        std::shared_ptr<Unit> shU = Unit::fromJson(obj.value("pawn").toObject(), Coordinates(x,y));
        mField[y][x] = shU;

        if(shU->getColor() == myId){
            mMyUnits << shU;
        }

        mAllUnits << shU;
    }
}


void BattleField::clearField()
{
    mField = battle_field();
}


void BattleField::move(const Coordinates &from, const Coordinates &to)
{
    mField[from.y][from.x]->move(to);
    mField[to.y][to.x] = mField[from.y][from.x];
    mField[from.y][from.x] = {};
}

void BattleField::attack(const Coordinates &from, const Coordinates &to)
{
    Q_UNUSED(from);
    const std::shared_ptr<Unit> &killed = mField[to.y][to.x];
    mAllUnits.removeAll(killed);//delete reference of the pointer
    mMyUnits.removeAll(killed);
    mField[to.y][to.x] = {};
}

const std::shared_ptr<Unit>& BattleField::unitAt(const Coordinates &coords) const
{
    if(!coords.isValid())return mEmpty;
    return mField[coords.y][coords.x];
}


std::vector<Turn> BattleField::possibleTurns() const
{
    std::vector<Turn> vec;
    for(std::shared_ptr<Unit> munit : mMyUnits){
        auto res = munit->possibleTurns(*this);
        vec.insert(vec.end(), res.begin(), res.end());
    }
    return vec;
}

void BattleField::applyAction(const Action &action)
{
    if(action.getType() == Action::ATTACK){
        attack(action.getFrom(), action.getTo());
    }else if(action.getType() == Action::MOVE){
        move(action.getFrom(), action.getTo());
    }
}


bool BattleField::isAttackable(const Coordinates &target, const Unit &attacker) const
{
    if(!target.isValid())return false;
    auto unit = unitAt(target);
    return unit && unit->getColor() != attacker.getColor();
}

bool BattleField::isAccessible(const Coordinates &dest) const
{
    return dest.isValid() && !unitAt(dest);
}
