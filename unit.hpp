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
 * File:   unit.hpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#ifndef UNIT_HPP
#define UNIT_HPP

#include <QJsonObject>
#include <QDebug>
#include <vector>

#include "coordinates.hpp"
#include "action.hpp"

class BattleField;


class Unit
{
public:
    enum COLOR {WHITE, BLACK};

    Unit();

    Unit(COLOR color, const Coordinates &position);

    COLOR getColor() const{
        return mColor;
    }

    static std::shared_ptr<Unit> fromJson(const QJsonObject &obj, const Coordinates &position);

    void setColor(COLOR c){
        mColor = c;
    }


    virtual QString strType() const = 0;


    virtual std::vector<Turn> possibleTurns(const BattleField &field) const = 0;

    void move(Coordinates nwPosition){
        mPosition = nwPosition;
    }


protected:
    Coordinates mPosition;
    COLOR mColor;
};

class MobileTower : public Unit
{
public:
    MobileTower(){}

    MobileTower(COLOR color, const Coordinates &position):
        Unit(color, position)
    {
    }

    QString strType() const override
    {
        return "S";
    }

    std::vector<Turn> possibleTurns(const BattleField &field) const override;
};


class Infantery : public Unit
{
public:
    Infantery(){}

    Infantery(COLOR color, const Coordinates& position):
        Unit(color, position)
    {
    }

    QString strType() const override
    {
        return "R";
    }

    std::vector<Turn> possibleTurns(const BattleField &field) const override;

};

class  Gunner : public Unit
{
public:
    Gunner(){}

    Gunner(COLOR color, const Coordinates &coordinates):
        Unit(color, coordinates)
    {
    }

    QString strType() const override
    {
        return "L";
    }


    std::vector<Turn> possibleTurns(const BattleField &field) const override;

};



inline QDebug operator<<(QDebug &debug,  const Unit &unit)
{
    return debug << unit.getColor() << unit.strType();
}


#endif // UNIT_HPP
