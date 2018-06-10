#include "Graph.hpp"
#include "Mathf.hpp"

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>

START_NAMESPACE_DISTRHO

namespace wolf
{

Vertex::Vertex() : x(0),
                   y(0),
                   tension(0),
                   type(SingleCurve),
                   warpAmount(0),
                   warpType(None)
{
}

Vertex::Vertex(float posX, float posY, float tension, CurveType type) : x(posX),
                                                                        y(posY),
                                                                        tension(tension),
                                                                        type(type),
                                                                        warpAmount(0.0f),
                                                                        warpType(None)
{
}

static float powerScale(float input, float tension, float maxExponent, float p1x, float p1y, float p2x, float p2y, bool inverse)
{
    assert(maxExponent >= 1);

    const float inputSign = input >= 0 ? 1 : -1;
    const bool tensionIsPositive = tension >= 0.0f;

    tension = std::abs(tension);

    const float deltaX = p2x - p1x;
    const float deltaY = p2y - p1y;

    input = std::abs(input);

    float exponent = 1 + tension * (maxExponent - 1);

    if (inverse)
    {
        exponent = 1.0f / exponent;
    }

    float result;

    if (tensionIsPositive)
    {
        result = deltaY * std::pow((input - p1x) / deltaX, exponent) + p1y;
    }
    else
    {
        result = 1 - (deltaY * std::pow(1 - (input - p1x) / deltaX, exponent) + p1y) + p2y - (1 - p1y);
    }

    return inputSign * result;
}

static float skewPlus(float x, float warpAmount)
{
    return 1 - std::pow(1 - x, warpAmount * 2 + 1);
}

static float invSkewPlus(float x, float warpAmount)
{
    return 1 - std::pow(1 - x, 1.0f / (warpAmount * 2 + 1));
}

static float skewMinus(float x, float warpAmount)
{
    return std::pow(x, warpAmount * 2 + 1);
}

static float invSkewMinus(float x, float warpAmount)
{
    return std::pow(x, 1.0f / (warpAmount * 2 + 1));
}

static float bendPlus(float x, float warpAmount, bool inverse)
{
    if (x < 0.5f)
    {
        return powerScale(x, -warpAmount, 3, 0.0f, 0.0f, 0.5f, 0.5f, inverse);
    }
    else if (x > 0.5f)
    {
        return powerScale(x, warpAmount, 3, 0.5f, 0.5f, 1.0f, 1.0f, inverse);
    }
    else
    {
        return x;
    }
}

static float bendMinus(float x, float warpAmount, bool inverse)
{
    if (x < 0.5f)
    {
        return powerScale(x, warpAmount, 3, 0.0f, 0.0f, 0.5f, 0.5f, inverse);
    }
    else if (x > 0.5f)
    {
        return powerScale(x, -warpAmount, 3, 0.5f, 0.5f, 1.0f, 1.0f, inverse);
    }
    else
    {
        return x;
    }
}

float Vertex::getX() const
{
    switch (warpType)
    {
    case None:
        return x;
    case BendPlus:
        return bendPlus(x, warpAmount, false);
    case BendMinus:
        return bendMinus(x, warpAmount, false);
    case BendPlusMinus:
    {
        if (warpAmount < 0.5f)
        {
            return bendPlus(x, (0.5f - warpAmount) * 2, false);
        }
        else if (warpAmount > 0.5f)
        {
            return bendMinus(x, (warpAmount - 0.5f) * 2, false);
        }
        else
        {
            return x;
        }
    }
    case SkewPlus:
        return skewPlus(x, warpAmount);
    case SkewMinus:
        return skewMinus(x, warpAmount);
    case SkewPlusMinus:
    {
        if (warpAmount < 0.5f)
        {
            return skewPlus(x, (0.5f - warpAmount) * 2);
        }
        else if (warpAmount > 0.5f)
        {
            return skewMinus(x, (warpAmount - 0.5f) * 2);
        }
        else
        {
            return x;
        }
    }
    default:
        return x;
    }
}

float Vertex::getY() const
{
    //TODO: warp
    return y;
}

float Vertex::getTension() const
{
    return tension;
}

CurveType Vertex::getType() const
{
    return type;
}

void Vertex::setX(float x)
{
    //we revert the effects of warp to set the correct value
    switch (warpType)
    {
    case None:
        this->x = x;
        break;
    case BendPlus:
        this->x = bendPlus(x, warpAmount, true);
        break;
    case BendMinus:
        this->x = bendMinus(x, warpAmount, true);
        break;
    case BendPlusMinus:
        if (warpAmount < 0.5f)
        {
            this->x = bendPlus(x, (0.5f - warpAmount) * 2, true);
        }
        else if (warpAmount > 0.5f)
        {
            this->x = bendMinus(x, (warpAmount - 0.5f) * 2, true);
        }
        else
        {
            this->x = x;
        }
        break;
    case SkewPlus:
        this->x = invSkewPlus(x, warpAmount);
        break;
    case SkewMinus:
        this->x = invSkewMinus(x, warpAmount);
        break;
    case SkewPlusMinus:
    {
        if (warpAmount < 0.5f)
        {
            this->x = invSkewPlus(x, (0.5f - warpAmount) * 2);
        }
        else if (warpAmount > 0.5f)
        {
            this->x = invSkewMinus(x, (warpAmount - 0.5f) * 2);
        }
        else
        {
            this->x = x;
        }
        break;
    }
    default:
        this->x = x;
        break;
    }
}

void Vertex::setY(float y)
{
    //TODO: see above
    this->y = y;
}

void Vertex::setPosition(float x, float y)
{
    setX(x);
    setY(y);
}

void Vertex::setTension(float tension)
{
    this->tension = tension;
}

void Vertex::setType(CurveType type)
{
    this->type = type;
}

void Vertex::setWarpAmount(float warp)
{
    this->warpAmount = warp;
}

void Vertex::setWarpType(WarpType warpType)
{
    this->warpType = warpType;
}

Graph::Graph() : vertexCount(0),
                 bipolarMode(false),
                 warpAmount(0.0f),
                 warpType(None)
{
    insertVertex(0.0f, 0.0f);
    insertVertex(1.0f, 1.0f);
}

float Graph::getOutValue(float input, float tension, float p1x, float p1y, float p2x, float p2y, CurveType type)
{
    const float inputSign = input >= 0 ? 1 : -1;

    if (p1x == p2x)
    {
        return inputSign * p2y;
    }

    //should probably be stored as a normalized value instead
    tension /= 100.0f;

    const bool tensionIsPositive = tension >= 0.0f;

    //make the curve bend more slowly when the tension is near 0
    if (tensionIsPositive)
    {
        tension = std::pow(tension, 1.2f);
    }
    else
    {
        tension = -std::pow(-tension, 1.2f);
    }

    const float deltaX = p2x - p1x;
    const float deltaY = p2y - p1y;

    switch (type)
    {
    case SingleCurve:
    {
        return powerScale(input, tension, 15.0f, p1x, p1y, p2x, p2y, false);
    }
    case DoubleCurve:
    {
        const float middleX = p1x + deltaX / 2.0f;
        const float middleY = p1y + deltaY / 2.0f;

        if (input > middleX)
        {
            return powerScale(input, -tension, 15.0f, middleX, middleY, p2x, p2y, false);
        }
        else
        {
            return powerScale(input, tension, 15.0f, p1x, p1y, middleX, middleY, false);
        }
    }
    case StairsCurve:
    {
        if (tension == 0.0f) //straight line, y = mx+b
        {
            const float slope = deltaY / deltaX;
            const float b = p1y - slope * p1x;

            return slope * input + b;
        }

        input = std::abs(input);

        int numSteps = std::floor(2.0f / std::pow(tension, 2.0f));

        const float stepX = deltaX / (tensionIsPositive ? numSteps : numSteps - 1);
        const float stepY = deltaY / (tensionIsPositive ? numSteps - 1 : numSteps);

        float result;

        if (tensionIsPositive)
        {
            result = std::floor((input - p1x) / stepX) * stepY + p1y;
        }
        else
        {
            result = std::floor((input - p1x) / stepX + 1) * stepY + p1y;
        }

        //clamped to avoid some overshoot, might not be necessary
        const float minY = std::min(p1y, p2y);
        const float maxY = std::max(p1y, p2y);

        return inputSign * wolf::clamp(result, minY, maxY);
    }
    case WaveCurve:
    {
        tension = std::floor(tension * 100.f);

        const float frequency = (0.5f + tension) / deltaX;
        const float phase = p1x * frequency * 2.0f * M_PI;

        float wave = -std::cos(frequency * M_PI * 2.0f * input - phase) / 2.0f + 0.5f;

        if (!tensionIsPositive)
        {
            wave = M_2_PI * std::asin(wave);
        }

        return inputSign * (wave * deltaY + p1y);
    }
    }
}

float Graph::getValueAt(float x)
{
    const float absX = std::abs(x);

    if (absX > 1.0f)
    {
        if (bipolarMode)
        {
            const bool positiveInput = x >= 0.0f;
            const float vertexY = vertices[positiveInput ? getVertexCount() - 1 : 0].getY();

            return absX * (-1.0f + vertexY * 2.0f);
        }
        else
        {
            return x * vertices[getVertexCount() - 1].getY();
        }
    }

    //binary search
    int left = 0;
    int right = vertexCount - 1;
    int mid = 0;

    while (left <= right)
    {
        mid = left + (right - left) / 2;

        if (vertices[mid].getX() < absX)
            left = mid + 1;
        else if (vertices[mid].getX() > absX)
            right = mid - 1;
        else
            return x >= 0 ? vertices[mid].getY() : -vertices[mid].getY();
    }

    const float p1x = vertices[left - 1].getX();
    const float p1y = vertices[left - 1].getY();

    const float p2x = vertices[left].getX();
    const float p2y = vertices[left].getY();

    return getOutValue(x, vertices[left - 1].getTension(), p1x, p1y, p2x, p2y, vertices[left - 1].getType());
}

void Graph::setWarpAmount(float warp)
{
    warpAmount = warp;

    for (int i = 0; i < vertexCount; ++i)
    {
        vertices[i].setWarpAmount(warp);
    }
}

void Graph::setWarpType(WarpType warpType)
{
    this->warpType = warpType;

    for (int i = 0; i < vertexCount; ++i)
    {
        vertices[i].setWarpType(warpType);
    }
}

void Graph::insertVertex(float x, float y, float tension, CurveType type)
{
    if (vertexCount == maxVertices)
        return;

    int i = vertexCount;

    while ((i > 0) && (x < vertices[i - 1].getX()))
    {
        vertices[i] = vertices[i - 1];
        --i;
    }

    Vertex vertex = Vertex(x, y, tension, type);

    vertex.setWarpAmount(warpAmount);
    vertex.setWarpType(warpType);
    vertex.setPosition(x, y);

    vertices[i] = vertex;

    ++vertexCount;
}

void Graph::removeVertex(int index)
{
    --vertexCount;

    for (int i = index; i < vertexCount; ++i)
    {
        vertices[i] = vertices[i + 1];
    }
}

void Graph::setTensionAtIndex(int index, float tension)
{
    vertices[index].setTension(tension);
}

Vertex *Graph::getVertexAtIndex(int index)
{
    assert(index < vertexCount);

    return &vertices[index];
}

int Graph::getVertexCount()
{
    return vertexCount;
}

bool Graph::getBipolarMode()
{
    return bipolarMode;
}

void Graph::setBipolarMode(bool bipolarMode)
{
    this->bipolarMode = bipolarMode;
}

const char *Graph::serialize()
{
    Vertex vertex;

    int length = 0;

    for (int i = 0; i < vertexCount; ++i)
    {
        vertex = vertices[i];

        length += wolf::toHexFloat(serializationBuffer + length, vertex.x);
        length += std::sprintf(serializationBuffer + length, ",");
        length += wolf::toHexFloat(serializationBuffer + length, vertex.y);
        length += std::sprintf(serializationBuffer + length, ",");
        length += wolf::toHexFloat(serializationBuffer + length, vertex.tension);
        length += std::sprintf(serializationBuffer + length, ",%d;", vertex.type);
    }

    return serializationBuffer;
}

void Graph::clear()
{
    vertexCount = 0;
}

void Graph::rebuildFromString(const char *serializedGraph)
{
    char *rest = (char *const)serializedGraph;

    int i = 0;

    do
    {
        const float x = wolf::parseHexFloat(rest, &rest);
        const float y = wolf::parseHexFloat(++rest, &rest);
        const float tension = wolf::parseHexFloat(++rest, &rest);
        const CurveType type = static_cast<CurveType>(std::strtol(++rest, &rest, 10));

        Vertex vertex = Vertex(x, y, tension, type);
        vertex.setWarpAmount(warpAmount);
        vertex.setWarpType(warpType);

        vertices[i++] = vertex;

    } while (strcmp(++rest, "\0") != 0);

    vertexCount = i;
}
} // namespace wolf

END_NAMESPACE_DISTRHO
