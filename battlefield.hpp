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

/**
 * @brief The BattleField class
 * class to represent the battlefield, where all the units are
 * it is used to calculate the weight of each actions
 */
class BattleField
{
public:
    /**
     * @brief BattleField default constructor
     */
    BattleField();

    /**
     * @brief BattleField copy constructor
     * used for the minmax, to simulate the next
     * battlefield state
     * @param other
     */
    BattleField(const BattleField &other);

    /**
     * @brief actionWeight calculates the weight of an action
     * @param action
     * @return
     */
    float actionWeight(const Action &action) const;

    /**
     * @brief fillField fills field with the units
     * given as the json array (sent by the server)
     * @param units
     */
    void fillField(const QJsonArray &units);

    /**
     * @brief clearField removes all the units
     * from the field
     */
    void clearField();

    /**
     * @brief getField getter for the field
     * @return
     */
    const battle_field &getField() const
    {
        return mField;
    }

    /**
     * @brief unitAt returns the unit at the given coordinates
     * if the field is empty, or the coordinates are invalid
     * return a reference to an empty pointer
     * @param coords
     * @return
     */
    const std::shared_ptr<Unit> &unitAt(const Coordinates &coords) const;

    /**
     * @brief attack performs an attack from the given coordinate
     * to the target coordinates
     * (removes the unit from the target coordinate)
     * @param from
     * @param to
     */
    void attack(const Coordinates &from, const Coordinates &to);

    /**
     * @brief move moves the unit at the coordinates "from"
     * to the coordinates "to"
     *
     * @param from
     * @param to
     */
    void move(const Coordinates &from,const Coordinates &to);

    /**
     * @brief possibleTurns calculates all the possible turns
     * using the units of the battlefield that are to the player
     * @return
     */
    std::vector<Turn> possibleTurns() const;

    /**
     * @brief applyAction apply the given action
     * on the battlefield
     * @param action
     */
    void applyAction(const Action &action);

    /**
     * @brief setId setter for the id
     * @param nwId
     */
    void setId(int nwId){
        myId = nwId;
    }

    /**
     * @brief isAccessible
     * @param dest
     * @return wether the field at the given coordinates is accessible,
     * that is : valid coordinates, and an empty field
     */
    bool isAccessible(const Coordinates &dest) const;

    /**
     * @brief isAttackable
     * @param target
     * @param attacker
     * @return wether the field at the given position can be
     * attacked by the given unit
     */
    bool isAttackable(const Coordinates &target, const Unit & attacker) const;

    /**
     * @brief isAttackable
     * @param target
     * @param originColor
     * @return  wether the field at the given position can be
     * attacked by the given color.
     * That is : a valid coordinate, an non-empty field, and a different
     * color than the attacker's
     */
    bool isAttackable(const Coordinates &target, Unit::COLOR originColor) const;

    /**
     * @brief getId getter for the id
     * @return
     */
    int getId() const
    {
        return myId;
    }

    /**
     * @brief numberOfUnits the number of the player's unit
     * alive on the field
     * @return
     */
    int numberOfUnits() const
    {
        return mAllUnits.size();
    }

private:

    /**
     * @brief mEmpty
     * empty pointer, when an invalid unit
     * is requested
     */
    const std::shared_ptr<Unit> mEmpty = {};

    /**
     * @brief mField
     * the array containing all the units
     */
    battle_field mField;

    /**
     * @brief mAllUnits all the units alive on the field
     * this is used to have a kick access to the units
     * instead of having to parse each lines of the field
     * to find them
     */
    QVector<std::shared_ptr<Unit>> mAllUnits;//Contains all the units present on the board

    /**
     * @brief mMyUnits all the units of the player
     */
    QVector<std::shared_ptr<Unit>> mMyUnits;// contains only the units of the client

    /**
    * @brief myId id of ther player
    */
    int myId = -1;
};

/**
 * @brief operator << prints out the field
 * @param debug
 * @param field
 * @return
 */
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
