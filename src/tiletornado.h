#ifndef URPLUSPLUS_TILETORNADO_H_
#define URPLUSPLUS_TILETORNADO_H_
#include "tile.h"

class TileTornado : public Tile {
  public:
    void acceptVisitor(GameViewer&) const override;
    TileTornado();
};

#endif
