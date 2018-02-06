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
 * File:   unit.cpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#include "unit.hpp"
#include "battlefield.hpp"

Unit::Unit():
    mColor(WHITE),
    mPosition(-1,-1)
{

}

Unit::Unit(COLOR color, const Coordinates &position):
    mColor(color),
    mPosition(position)
{

}

std::shared_ptr<Unit> Unit::fromJson(const QJsonObject &obj, const Coordinates &position)
{
    int type = obj.value("type").toInt();
    COLOR c = static_cast<COLOR>(obj.value("color").toInt());

    switch(type){
    case 0:
        return std::make_shared<MobileTower>(c, position);
    case 1:
        return std::make_shared<Infantery>(c, position);
    case 2:
        return std::make_shared<Gunner>(c, position);
    default:
        return {};
    }
}

std::vector<Turn> Gunner::possibleTurns(const BattleField &field) const
{
    std::vector<Turn> res;
    //check for moves
    for(int y = -1; y <= 1; ++y){
        for(int x = -1; x <= 1; ++x){
            if(y == 0 && x == 0)continue;
            Coordinates nwPos = mPosition + Coordinates(x,y);
            if(field.isAccessible(nwPos)){//no one here, possible to go
                res.emplace_back(std::make_unique<Action>(Action::MOVE, mPosition, nwPos));
            }
        }
    }

    //check for attacks
    int multiplier = mColor == WHITE ? 1 : -1;
    std::array<Coordinates, 7> directions = {
        {{-2, 0}, {-1, 0}, {1, 0}, {2, 0}, {0, multiplier}, {0, multiplier*2}, {0, multiplier*3} }
    };
    for(const Coordinates &c : directions){
        Coordinates nwPoint = mPosition + c;
        if(field.isAttackable(nwPoint, *this)){
            res.emplace_back(std::make_unique<Action>(Action::ATTACK, mPosition, nwPoint));
        }
    }


    return res;
}

std::vector<Turn> Infantery::possibleTurns(const BattleField &field) const
{
    std::vector<Turn> res;
    //check for moves
    for(int y = -1; y <= 1; ++y){
        for(int x = -1; x <= 1; ++x){
            if(y == 0 && x == 0)continue;
            Coordinates nwPos = mPosition + Coordinates(x,y);
            if(field.isAccessible(nwPos)){//no one here, possible to go
                res.emplace_back(std::make_unique<Action>(Action::MOVE, mPosition, nwPos));

                // two boxes move
                Coordinates extraPos = nwPos + Coordinates(x,y);
                if(field.isAccessible(extraPos))
                    res.emplace_back(std::make_unique<Action>(Action::MOVE, mPosition, nwPos));
            }
        }
    }


    //check for attacks
    Coordinates pos1 = mPosition + Coordinates(-1,0);
    if(field.isAttackable(pos1,*this)) res.emplace_back(std::make_unique<Action>(Action::ATTACK, mPosition, pos1));

    pos1 = mPosition + Coordinates(1, 0);
    if(field.isAttackable(pos1, *this)) res.emplace_back(std::make_unique<Action>(Action::ATTACK, mPosition, pos1));

    pos1 = mPosition + Coordinates(0, mColor == WHITE ? 1 : -1);
    if(field.isAttackable(pos1, *this)) res.emplace_back(std::make_unique<Action>(Action::ATTACK, mPosition, pos1));

    return res;
}

std::vector<Turn> MobileTower::possibleTurns(const BattleField &field) const
{
    std::vector<Turn> res;
    //check for moves
    for(int y = -1; y <= 1; ++y){
        for(int x = -1; x <= 1; ++x){
            if(y == 0 && x == 0)continue;
            Coordinates nwPos = mPosition + Coordinates(x,y);
            if(field.isAccessible(nwPos)){//no one here, possible to go
                res.emplace_back(std::make_unique<Action>(Action::MOVE, mPosition, nwPos));
            }
        }
    }

    //check for attacks
    for(int y = -1; y <= 1; ++y){
        for(int x = -1; x <= 1; ++x){
            if(y == 0 && x == 0)continue;
            Coordinates nwPos = mPosition + Coordinates(x,y);
            if(field.isAttackable(nwPos, *this)){//someone's here, attack !
                res.emplace_back(std::make_unique<Action>(Action::ATTACK, mPosition, nwPos));
            }

            // two boxes move
            Coordinates extraPos = nwPos + Coordinates(x,y);
            if(field.isAttackable(extraPos, *this))// attack !
                res.emplace_back(std::make_unique<Action>(Action::ATTACK, mPosition, extraPos));
        }
    }

    return res;
}
