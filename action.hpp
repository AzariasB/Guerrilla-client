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


class BattleField;

/**
 * @brief The Action class
 * This class represent a single action
 * (attack or move) to be done during a turn
 * An action has a weight that is evaludated by the battlefield
 * in order to determine the best move
 * with the minmax algorithm
 */
class Action
{
public:
    /**
     * @brief The ACTION_TYPE enum
     * the type of action
     * (unknown is used for the empty constructor)
     */
    enum ACTION_TYPE{ATTACK, MOVE, UNKNOWN};

    Action();//empty action for the minmax

    /**
     * @brief Action Constructor
     * @param type the type of action
     * @param from the origin coordinates
     * @param to the destination coordintaes
     */
    Action(ACTION_TYPE type, Coordinates from, Coordinates to);

    /**
     * @brief toJson serializes the action used
     * for the websockets
     * @return the serialized object
     */
    QJsonObject toJson() const;

    /**
     * @brief operator QString string tranformation
     * of the object
     */
    operator QString() const{
        return QString(QJsonDocument(toJson()).toJson(QJsonDocument::Compact));
    }

    /**
     * @brief toString method to get the json as a string
     * @return
     */
    QString toString() const
    {
        return this->operator QString();
    }

    /**
     * @brief getter for the type
     * @return
     */
    ACTION_TYPE getType() const{
        return mType;
    }

    /**
     * @brief getFrom getter for the origin
     * @return
     */
    const Coordinates &getFrom() const{
        return mFrom;
    }

    /**
     * @brief getTo getter for the destination
     * @return
     */
    const Coordinates &getTo() const{
        return mTo;
    }

    /**
     * @brief getWeight getter for the weight
     * @return
     */
    float getWeight() const {
        return mWeight;
    }

    /**
     * @brief setWeight setter for the weight
     * @param weight
     */
    void setWeight(float weight){
        mWeight = weight;
    }

private:
    /**
     * @brief mType the type of this action
     */
    ACTION_TYPE mType;

    /**
     * @brief mFrom the origin coordinates of the
     * action
     */
    Coordinates mFrom;

    /**
     * @brief mTo the destination of the action
     * (point to attack for an attack action,
     *  point where to move for a move action)
     */
    Coordinates mTo;

    /**
     * @brief mWeight the weight of the
     * action
     */
    float mWeight;
};


/**
 * @brief The Turn class
 * the turn class contains two actions,
 * because each turn, two actions can be
 * executed. A turn has a weight,
 * which is the sum of the two actions it contains
 * It may contain only one action because it may not
 * be possible to attack & move at the same turn
 */
class Turn{

public:
    Turn()
    {
    }

    /**
     * @brief Turn constructor, with only one action
     * @param firstAction the first and only action of this turn
     */
    Turn(const std::shared_ptr<Action> &firstAction):
        mActions(firstAction, {})
    {
    }

    /**
     * @brief Turn constructor, with two actions
     * @param firstAction
     * @param secondAction
     */
    Turn(const std::shared_ptr<Action> &firstAction,const std::shared_ptr<Action> &secondAction):
        mActions(firstAction, secondAction)
    {
    }

    /**
     * @brief applyActions execute all the actions on the given field
     * @param field
     */
    void applyActions(BattleField &field);

    /**
     * @brief sendToSocket send the actions to the given websocket
     * @param socket
     */
    void sendToSocket(QWebSocket &socket) const;

    /**
     * @brief addAction add an action to this turn,
     * if the turn is already full, does nothing
     * otherwise, add at the correct order
     * @param nwAction
     */
    void addAction(const std::shared_ptr<Action> &nwAction);

    /**
     * @brief getWeight the sum of the action's weight
     * @return
     */
    float getWeight() const;

    /**
     * @brief getAction get action at the given index (0 or 1)
     * @param index
     * @return
     */
    const std::shared_ptr<Action> &getAction(quint8 index);

    /**
     * @brief hasAttack wether this turn contains an attack action
     * @return
     */
    bool hasAttack() const;


private:
    /**
     * @brief mActions a pionter to both of the actions
     */
    std::pair<std::shared_ptr<Action>, std::shared_ptr<Action>> mActions;
};

#endif // ACION_HPP
