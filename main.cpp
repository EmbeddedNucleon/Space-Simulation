/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Ryan Michael Thomas
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/


#include "dialog.h"
#include "widthvisitor.h"
#include "universebody.h"
#include "universecomponent.h"
#include <QApplication>

/*
 * Extensions:
 * 1. Show trajectory (green line) using cursor over body and pressing key "T"
 * 2. A RESET button to start the planets in their original position
 * 3. Dynamic background image which responds to zoom (advanced zoom feature)
 * 4. Catastrophe mode: bodies move quickly as time increases exponentially; press key "C"
 *
 */

//test UniverseBody
bool universeBodyTest1()
{
    std::string expected = "Earth";
    UniverseBody* universeBody = new UniverseBody(planet, expected, "SolarSystem");
    std::string actual = universeBody->getName();
    delete universeBody;
    return expected == actual;
}

bool universeBodyTest2()
{
    std::string expected = "Mars";
    UniverseBody* universeBody = new UniverseBody(planet, "Earth", "SolarSystem");
    universeBody->setName(expected);
    std::string actual = universeBody->getName();
    delete universeBody;
    return expected == actual;
}

bool universeBodyTest3()
{
    UniverseComponentType expected = cluster;
    UniverseBody* universeBody = new UniverseBody(expected, "Cluster1", "ClusterParent");
    UniverseComponentType actual = universeBody->getType();
    delete universeBody;
    return expected == actual;
}

// test zodiacs
bool zodiacTest()
{
    UniverseBody* universeBody1 = new UniverseBody(planet, "Earth", "SolarSystem");
    UniverseBody* universeBody2 = new UniverseBody(planet, "Mars", "SolarSystem");
    UniverseBody* universeBody3 = new UniverseBody(planet, "Sun", "SolarSystem");
    Zodiac* zodiac = new Zodiac();
    zodiac->add(universeBody1, universeBody2);
    zodiac->add(universeBody1, universeBody3);
    int zodiac1 = zodiac->getZodiacs().size();
    std::string zodiac2 = zodiac->getZodiacs().front().first->getName();
    std::string zodiac3 = zodiac->getZodiacs().back().second->getName();
    delete universeBody1;
    delete universeBody2;
    delete universeBody3;
    delete zodiac;
    return (zodiac1 == 2) && (zodiac2 == "Earth") && (zodiac3 == "Sun");
}

// test widthVisitor
bool widthVisitorTest(Dialog &dialog)
{
    WidthVisitor *widthVisitor = new WidthVisitor(50);
    int width = dialog.accept(*widthVisitor);
    delete widthVisitor;
    return (int) width == 400;
}

// test heightVisitor
bool heightVisitorTest(Dialog &dialog)
{
    HeightVisitor *heightVisitor = new HeightVisitor(50);
    int height = dialog.accept(*heightVisitor);
    delete heightVisitor;
    return (int) height == 400;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    // tests

    int numberOfTestsFailed = 0;

    if (!universeBodyTest1())
    {
        numberOfTestsFailed++;
    }
    if (!universeBodyTest2())
    {
        numberOfTestsFailed++;
    }
    if (!universeBodyTest3())
    {
        numberOfTestsFailed++;
    }
    if (!zodiacTest())
    {
        numberOfTestsFailed++;
    }
    if (!widthVisitorTest(w))
    {
        numberOfTestsFailed++;
    }
    if (!heightVisitorTest(w))
    {
        numberOfTestsFailed++;
    }
    if (0 == numberOfTestsFailed)
    {
        std::cout << "\nAll Tests Passed!\n" << std::endl;
    }
    else
    {
        std::cout << numberOfTestsFailed << " Tests Failed" << std::endl;
    }

    return a.exec();
}
