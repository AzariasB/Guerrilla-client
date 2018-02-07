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
 * File:   battlefield.hpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#ifndef BATTLEFIELD_HPP
#define BATTLEFIELD_HPP

#include <array>
#include <memory>
#include <unit.hpp>
#include <qdebug.h>
#include <qvector.h>
#include <vector>

#include "coordinates.hpp"
#include "action.hpp"

using battle_field = std::array<std::array<std::shared_ptr<Unit> ,25 >, 25>;

class BattleField
{
public:
    BattleField();

    void fillField(const QJsonArray &units);

    void clearField();

    const battle_field &getField() const
    {
        return mField;
    }

    const std::shared_ptr<Unit> &unitAt(const Coordinates &coords) const;

    void attack(const Coordinates &from, const Coordinates &to);

    void move(const Coordinates &from,const Coordinates &to);

    std::vector<Turn> possibleTurns() const;

    void applyAction(const Action &action);

    void setId(int nwId){
        myId = nwId;
    }

    bool isAccessible(const Coordinates &dest) const;

    bool isAttackable(const Coordinates &target, const Unit & attacker) const;

    bool isAttackable(const Coordinates &target, Unit::COLOR originColor) const;

private:
    std::shared_ptr<Unit> mEmpty = {};

   battle_field mField;
   QVector<std::shared_ptr<Unit>> mAllUnits;//Contains all the units present on the board
   QVector<std::shared_ptr<Unit>> mMyUnits;// contains only the units of the client

   int myId = -1;
};


inline QDebug operator<<(QDebug debug, const BattleField &field)
{
    for(int y = 0; y < 25; ++y){
        for(int x = 0; x < 25; ++x){
            if(field.getField()[y][x]){
                debug << *field.getField()[y][x];
            }else{
                debug << "--";
            }
        }
        debug << "\n";
    }
    return debug;
}

#endif // BATTLEFIELD_HPP
