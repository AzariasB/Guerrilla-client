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


/**
 * @brief The Unit class
 * the unit is
 */
class Unit
{
public:
    /**
     * @brief The COLOR enum color of the unit
     */
    enum COLOR {WHITE, BLACK};

    /**
     * @brief Unit
     * empty constructor
     */
    Unit();

    /**
     * @brief Unit used to construct a unit
     * with the given color and the given position
     * @param color
     * @param position
     */
    Unit(COLOR color, const Coordinates &position);

    /**
     * @brief getColor getter for the unit's color
     * @return
     */
    COLOR getColor() const{
        return mColor;
    }

    /**
     * @brief fromJson static function to get an instance
     * of a unit, from the given json object, positioned at
     * the given coordiantes
     * @param obj
     * @param position
     * @return
     */
    static std::shared_ptr<Unit> fromJson(const QJsonObject &obj, const Coordinates &position);

    void setColor(COLOR c){
        mColor = c;
    }

    /**
     * @brief clone virtual function that must
     * be overriden by the subclass in order
     * to be able to clone them
     * @return
     */
    virtual Unit *clone() const = 0;

    /**
     * @brief strType string representation
     * of the unit
     * @return
     */
    virtual QString strType() const = 0;

    /**
     * @brief possibleTurns
     * all the possible turns available for this
     * battlefield's own unit
     * @param field
     * @return
     */
    std::vector<Turn> possibleTurns(const BattleField &field);

    /**
     * @brief moves the unit at
     * the given position
     * @param nwPosition
     */
    void move(const Coordinates &nwPosition){
        mPosition = nwPosition;
    }


protected:

    virtual const std::vector<Coordinates> &possibleMoves() const;

    virtual const std::vector<Coordinates> &possibleAttacks() const = 0;

    /**
     * @brief possibleMoves
     * the possible coordinates
     * from where the unit can move
     * the vector is initialized by default
     * but can be changed
     */
    static const std::vector<Coordinates> mPossibleStandardMoves;

    /**
     * @brief mColor color of the unit
     */
    COLOR mColor;

    /**
     * @brief mPosition position of the unit
     */
    Coordinates mPosition;
};

/**
 * @brief The MobileTower class
 * a quit powerfull unit that moves "slowly"
 * but can attack at a very long range
 */
class MobileTower : public Unit
{
public:
    MobileTower(){}

    /**
     * @brief MobileTower full constructor
     * @param color
     * @param position
     */
    MobileTower(COLOR color, const Coordinates &position):
        Unit(color, position)
    {
    }

    const std::vector<Coordinates> &possibleAttacks() const override
    {
        return mPossibleAttacks;
    }

    /**
     * @brief clone overriden function
     * @return
     */
    Unit *clone() const override
    {
        return new MobileTower(mColor, mPosition);
    }

    /**
     * @brief strType
     * @return
     */
    QString strType() const override
    {
        return "S";
    }

private:
    static const std::vector<Coordinates> mPossibleAttacks;
};


/**
 * @brief The Infantery class
 * the infantery is a unit
 * quite vulnerable, but that
 * can move pretty fast compared
 * to the other units
 */
class Infantery : public Unit
{
public:
    Infantery(){}

    Infantery(COLOR color, const Coordinates& position):
        Unit(color, position)
    {

    }

    Unit *clone() const override
    {
        return new Infantery(mColor, mPosition);
    }

    const std::vector<Coordinates> &possibleMoves() const
    {
        return mPossibleMoves;
    }

    const std::vector<Coordinates> &possibleAttacks() const override
    {
        return mColor == WHITE ? mPossibleWhiteAttacks : mPossibleBlackAttacks;
    }


    QString strType() const override
    {
        return "L";
    }

private:
    static const std::vector<Coordinates> mPossibleMoves;

    static const std::vector<Coordinates> mPossibleWhiteAttacks;

    static const std::vector<Coordinates> mPossibleBlackAttacks;
};

/**
 * @brief The Gunner class
 * the gunner is as slow
 * as the mobile tower
 * but can shoot further in front of it
 */
class  Gunner : public Unit
{
public:
    Gunner(){}

    Gunner(COLOR color, const Coordinates &coordinates):
        Unit(color, coordinates)
    {
    }

    Unit *clone()  const override
    {
        return new Gunner(mColor, mPosition);
    }

    QString strType() const override
    {
        return "R";
    }

    const std::vector<Coordinates> &possibleAttacks() const override
    {
        return mColor == WHITE ? mPossibleWhiteAttacks : mPossibleBlackAttacks;
    }

private:
    static const std::vector<Coordinates> mPossibleWhiteAttacks;

    static const std::vector<Coordinates> mPossibleBlackAttacks;
};



inline QDebug operator<<(QDebug &debug,  const Unit &unit)
{
    return debug << unit.getColor() << unit.strType();
}


#endif // UNIT_HPP
