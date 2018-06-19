#ifndef WOLF_GRAPH_DEFINED_H
#define WOLF_GRAPH_DEFINED_H

#include "src/DistrhoDefines.h"

START_NAMESPACE_DISTRHO

namespace wolf
{
/**
   * The max number of vertices that can be in the graph at the same time.
   */
const int maxVertices = 99;

enum CurveType
{
  SingleCurve = 0,
  DoubleCurve,
  StairsCurve,
  WaveCurve
};

enum WarpType
{
  None = 0,
  BendPlus,
  BendMinus,
  BendPlusMinus,
  SkewPlus,
  SkewMinus,
  SkewPlusMinus
};

class Vertex
{
public:
  friend class Graph;

  float getX() const;
  float getY() const;
  float getTension() const;
  CurveType getType() const;

  void setX(float x);
  void setY(float y);
  void setPosition(float x, float y);
  void setTension(float tension);
  void setType(CurveType type);

protected:
  Vertex();
  Vertex(float posX, float posY, float tension, CurveType type, float *warpAmountPtr, WarpType *warpTypePtr);

private:
  float x;
  float y;
  float tension;
  CurveType type;

  float *warpAmountPtr;
  WarpType *warpTypePtr;
};

class Graph
{
public:
  Graph();

  void insertVertex(float x, float y, float tension = 0.0f, CurveType type = CurveType::SingleCurve);
  void removeVertex(int index);
  Vertex *getVertexAtIndex(int index);

  void setTensionAtIndex(int index, float tension);

  /**
   * Return the number of vertices contained in the graph.
   */
  int getVertexCount();

  static float getOutValue(float input, float tension, float p1x, float p1y, float p2x, float p2y, CurveType type);

  /**
   * Get the y value at x in the graph. 
   */
  float getValueAt(float x);

  /**
   * Empty the graph.
   */
  void clear();

  /**
   * Save the graph into a string.
   */
  const char *serialize();

  bool getBipolarMode();
  void setBipolarMode(bool bipolarMode);

  void setWarpAmount(float warp);
  void setWarpType(WarpType warpType);

  /**
   * Rebuild the graph from a string.
   */
  void rebuildFromString(const char *serializedGraph);

private:
  Vertex vertices[maxVertices];
  int vertexCount;

  float warpAmount;
  WarpType warpType;

  bool bipolarMode;

  //format: x,y,tension,type;
  char serializationBuffer[(sizeof(char) * 256 + 4) * maxVertices + 1];
};

} // namespace wolf

END_NAMESPACE_DISTRHO

#endif