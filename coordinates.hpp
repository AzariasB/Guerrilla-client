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

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

struct Coordinates{
    int x,y;

    Coordinates():
        x(0),
        y(0)
    {

    }

    Coordinates(int x_, int y_):
        x(x_),
        y(y_)
    {

    }

    bool isValid() const{
        return x >= 0 && y >= 0 && x < 25 && y < 25;
    }

    Coordinates(const Coordinates &other):
        x(other.x),
        y(other.y)
    {

    }

    Coordinates operator+(const Coordinates &other) const
    {
        return Coordinates(x + other.x, y + other.y);
    }

    Coordinates(const QJsonObject &obj):
        x(obj["x"].toInt()),
        y(obj["y"].toInt())
    {

    }

    QJsonObject toJsonObjet() const{
        QJsonObject res;
        res["x"] = x;
        res["y"] = y;
        return res;
    }
};


#endif // COORDINATES_HPP
