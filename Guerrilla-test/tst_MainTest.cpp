

#include <QString>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>

#include "tree.hpp"
#include "battlefield.hpp"
#include "unit.hpp"


#include <chrono>//perf test

int  main(void)
{
    QFile f(":/config.json");

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open file";
        return 1;
    }
    auto content = f.readAll();

    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(content);
    if(doc.isNull()){
        qDebug() << "Failed to parse json";
        return 1;
    }

    int treeDepth = doc.object().value("tree_depth").toInt(1);

    int numberOfUnits = doc.object().value("data").toArray().size();

    BattleField btf;
    btf.setId(0);

    QJsonArray arr = doc.object().value("data").toArray();
    btf.fillField(arr);


    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    Tree t;
    t.generate(treeDepth , btf);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();


    qDebug() << "Tree generation = " << duration << " microseconds";
    qDebug() << "Number of units on the board : " << numberOfUnits;

    return 0;

}
