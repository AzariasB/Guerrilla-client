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

//standart
const std::vector<Coordinates> Unit::mPossibleStandardMoves = {{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0}};

// Mobile tower
const std::vector<Coordinates> MobileTower::mPossibleAttacks =  {
    {1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},//all around
    {2,2},{0,2},{-2,2},{-2,0},{-2,-2},{0,-2},{2,-2},{2,0}//double range
};

//Infantery
const std::vector<Coordinates> Infantery::mPossibleBlackAttacks = {{1,0},{-1,0},{0,-1}};

const std::vector<Coordinates> Infantery::mPossibleWhiteAttacks = {{1,0},{-1,0},{0,1}};

const std::vector<Coordinates> Infantery::mPossibleMoves = {
    {1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},//all around
    {2,2},{0,2},{-2,2},{-2,0},{-2,-2},{0,-2},{2,-2},{2,0}//double range
};

//Gunner
const std::vector<Coordinates> Gunner::mPossibleBlackAttacks = {
    {1,0},{2,0},{-1,0},{-2,0},//side
    {0,-1},{0,-2},{0,-3}//front
};

const std::vector<Coordinates> Gunner::mPossibleWhiteAttacks = {
    {1,0},{2,0},{-1,0},{-2,0},//side
    {0,1},{0,2},{0,3}//front
};



Unit::Unit():
    mColor(WHITE),
    mPosition(-1,-1)
{

}

const std::vector<Coordinates> &Unit::possibleMoves() const
{
    return mPossibleStandardMoves;
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

std::vector<Turn> Unit::possibleTurns(const BattleField &field)
{
    std::vector<Turn> possibleTurns;

    for(const auto& move : possibleMoves()){
        Coordinates nwPos = mPosition + move;
        if(field.isAccessible(nwPos)){
            std::shared_ptr<Action> moveAction = std::make_shared<Action>(Action::MOVE, mPosition, nwPos);

            possibleTurns.emplace_back(moveAction);

            for(const auto& attack : possibleAttacks()){
                Coordinates attackPos = nwPos + attack;
                if(field.isAttackable(attackPos,*this))
                    possibleTurns.emplace_back(moveAction, std::make_shared<Action>(Action::ATTACK, nwPos, attackPos));
            }

        }

    }

    return possibleTurns;
}


