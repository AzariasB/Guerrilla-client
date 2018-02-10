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
 * File:   coordinates.hpp
 * Author: azarias
 *
 * Created on 30/1/2018
 */
#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <QJsonObject>

#include  <random>
#include  <iterator>

/**
 * @brief The Coordinates struct
 * very usefull class used to point out
 * a field of the battlefield
 * the Coordinate struct has a public
 * int x and y.
 * It has an overloaded "+" operator
 * can be tranformed into a jsonobject
 * can check for its own validity relatively to
 * the battlefield
 */
struct Coordinates{
    int x,y;

    /**
     * @brief Coordinates default constructor
     * (init all the valus at 0)
     */
    Coordinates():
        x(0),
        y(0)
    {

    }

    /**
     * @brief Coordinates constructs the coordinates
     * object using the given x and y
     * @param x_
     * @param y_
     */
    Coordinates(int x_, int y_):
        x(x_),
        y(y_)
    {

    }

    /**
     * @brief isValid
     * @return wether this coordinates is valid
     * relative to the battlefield
     */
    bool isValid() const{
        return x >= 0 && y >= 0 && x < 25 && y < 25;
    }

    /**
     * @brief Coordinates copy constructor
     * @param other
     */
    Coordinates(const Coordinates &other):
        x(other.x),
        y(other.y)
    {

    }

    /**
     * @brief operator + "add" overload, to easily add two coordinates together
     * @param other
     * @return
     */
    Coordinates operator+(const Coordinates &other) const
    {
        return Coordinates(x + other.x, y + other.y);
    }

    /**
     * @brief Coordinates creates a coordinate
     * from the given jsonobject
     * @param obj
     */
    Coordinates(const QJsonObject &obj):
        x(obj["x"].toInt()),
        y(obj["y"].toInt())
    {

    }

    /**
     * @brief toJsonObjet
     * @return the coordinate representation of this
     * coordinate
     */
    QJsonObject toJsonObjet() const{
        QJsonObject res;
        res["x"] = x;
        res["y"] = y;
        return res;
    }
};


#endif // COORDINATES_HPP
